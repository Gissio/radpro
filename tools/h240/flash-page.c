/* Rad Pro H240 SRAM application page loader. License: MIT */

#include <stdbool.h>
#include <stdint.h>

#ifndef FLASH_PAGE_READ
#define FLASH_PAGE_READ(address) (*(volatile const uint32_t *)(uintptr_t)(address))
#define FLASH_PAGE_WRITE(address, value) (*(volatile uint32_t *)(uintptr_t)(address) = (value))
#endif

#define PAGE_SIZE 4096u
#define APP_START 0x08000000u
#define APP_END 0x0800a000u
#define PAGE_COUNT ((APP_END - APP_START) / PAGE_SIZE)
#define INPUT_START 0x20001000u
#define PREPARE_TAG 0xa4b86310u
#define COMMIT_TAG 0x731ef2c0u

#define FCI_AR 0x40020400u
#define FCI_KEY 0x40020404u
#define FCI_SR 0x4002040cu
#define FCI_CR 0x40020410u
#define FCI_BUSY 0x01u
#define FCI_ERRORS 0x0au
#define FCI_CLEAR_FLAGS 0x2au
#define FCI_PROGRAM 0x01u
#define FCI_PAGE_ERASE 0x02u
#define FCI_START 0x40u
#define FCI_LOCK 0x400u
#define FCI_IDLE_RESET 0x1800u
#define FCI_IDLE_CHARGE 0x1900u
#define FCI_POLL_COUNT 0x200000u

enum
{
    READY = 1,
    CHECKING,
    PREPARED,
    ERASING,
    PROGRAMMING,
    PAGE_COMPLETE,
    BAD_MAILBOX = 0x80000001u,
    BAD_COMMAND,
    BAD_SEQUENCE,
    BAD_PAGE,
    CONVERTER_ACTIVE,
    CONTROLLER_STATE,
    INPUT_CRC_MISMATCH,
    TARGET_CRC_MISMATCH,
    UNLOCK_FAILED,
    ERASE_TIMEOUT,
    ERASE_ERROR,
    ERASE_VERIFY,
    PROGRAM_TIMEOUT,
    PROGRAM_ERROR,
    PROGRAM_VERIFY,
    PAGE_VERIFY,
    CONTROL_MISMATCH,
    RELOCK_FAILED,
    PAGE_CRC_MISMATCH,
};

volatile struct
{
    uint32_t challenge;
    uint32_t response;
    uint32_t command;
    uint32_t sequence;
    uint32_t page;
    uint32_t expected_before_crc;
    uint32_t expected_input_crc;
    uint32_t status;
    uint32_t detail;
    uint32_t observed_before_crc;
    uint32_t observed_input_crc;
    uint32_t final_control;
} flash_page_mailbox __attribute__((section(".mailbox"), used));

struct Request
{
    uint32_t sequence;
    uint32_t page;
    uint32_t before_crc;
    uint32_t input_crc;
};

static uint32_t fail(uint32_t status, uint32_t detail)
{
    flash_page_mailbox.detail = detail;
    return status;
}

static bool converterOff(void)
{
    return (FLASH_PAGE_READ(0x4002203cu) & 0xffffu) == 0x4302u &&
           !((FLASH_PAGE_READ(0x400220c0u) | FLASH_PAGE_READ(0x400220d0u)) & 0x8000u) &&
           !(FLASH_PAGE_READ(0x4002101cu) & 0x100u);
}

static uint32_t pageCrc(uint32_t address)
{
    uint32_t crc = UINT32_MAX;
    for (uint32_t offset = 0; offset < PAGE_SIZE; offset += sizeof(uint32_t))
    {
        crc ^= FLASH_PAGE_READ(address + offset);
        for (unsigned bit = 0; bit < 32; ++bit)
            crc = (crc >> 1) ^ (0xedb88320u & (0u - (crc & 1u)));
    }
    return ~crc;
}

static uint32_t precheck(const struct Request *request, uint32_t *idle_control)
{
    if (!converterOff())
        return CONVERTER_ACTIVE;

    uint32_t status = FLASH_PAGE_READ(FCI_SR);
    uint32_t control = FLASH_PAGE_READ(FCI_CR);
    flash_page_mailbox.final_control = control;
    uint32_t idle = control & ~FCI_LOCK;
    if (status || (idle != FCI_IDLE_RESET && idle != FCI_IDLE_CHARGE))
        return fail(CONTROLLER_STATE, status ? status : control);
    *idle_control = idle;

    uint32_t before_crc = pageCrc(request->page);
    uint32_t input_crc = pageCrc(INPUT_START);
    flash_page_mailbox.observed_before_crc = before_crc;
    flash_page_mailbox.observed_input_crc = input_crc;
    if (input_crc != request->input_crc)
        return fail(INPUT_CRC_MISMATCH, input_crc);
    if (before_crc != request->before_crc)
        return fail(TARGET_CRC_MISMATCH, before_crc);
    return 0;
}

static bool sameRequest(uint32_t challenge, const struct Request *request)
{
    return flash_page_mailbox.challenge == challenge && flash_page_mailbox.response == ~challenge &&
           flash_page_mailbox.command == 0 && flash_page_mailbox.sequence == request->sequence &&
           flash_page_mailbox.page == request->page && flash_page_mailbox.expected_before_crc == request->before_crc &&
           flash_page_mailbox.expected_input_crc == request->input_crc;
}

static uint32_t preparePage(uint32_t challenge, uint32_t sequence, uint32_t seen,
                            uint32_t command, const struct Request *request)
{
    if (command != (challenge ^ sequence ^ PREPARE_TAG))
        return fail(BAD_COMMAND, command);
    if (!sameRequest(challenge, request))
        return BAD_MAILBOX;
    if (request->sequence != sequence)
        return fail(BAD_SEQUENCE, request->sequence);
    if (request->page < APP_START || request->page >= APP_END || (request->page & (PAGE_SIZE - 1u)) ||
        (seen & (1u << ((request->page - APP_START) / PAGE_SIZE))))
        return fail(BAD_PAGE, request->page);

    flash_page_mailbox.detail = 0;
    flash_page_mailbox.observed_before_crc = 0;
    flash_page_mailbox.observed_input_crc = 0;
    flash_page_mailbox.final_control = 0;
    flash_page_mailbox.status = CHECKING;
    uint32_t idle_control;
    return precheck(request, &idle_control);
}

static uint32_t waitIdle(void)
{
    uint32_t status = 0;
    for (uint32_t remaining = FCI_POLL_COUNT; remaining != 0; --remaining)
    {
        status = FLASH_PAGE_READ(FCI_SR);
        if (!(status & FCI_BUSY))
            break;
    }
    return status;
}

static uint32_t erasePage(uint32_t page, uint32_t idle_control)
{
    FLASH_PAGE_WRITE(FCI_SR, FCI_CLEAR_FLAGS);
    uint32_t status = FLASH_PAGE_READ(FCI_SR);
    if (status & (FCI_BUSY | FCI_CLEAR_FLAGS))
        return fail(ERASE_ERROR, status);

    FLASH_PAGE_WRITE(FCI_CR, idle_control | FCI_PAGE_ERASE);
    uint32_t control = FLASH_PAGE_READ(FCI_CR);
    if (control != (idle_control | FCI_PAGE_ERASE))
        return fail(CONTROL_MISMATCH, control);
    FLASH_PAGE_WRITE(FCI_AR, page);
    FLASH_PAGE_WRITE(FCI_CR, control | FCI_START);
    status = waitIdle();
    if (status & FCI_BUSY)
        return fail(ERASE_TIMEOUT, status);
    control = FLASH_PAGE_READ(FCI_CR);
    if (control != (idle_control | FCI_PAGE_ERASE))
        return fail(CONTROL_MISMATCH, control);
    FLASH_PAGE_WRITE(FCI_CR, idle_control);
    control = FLASH_PAGE_READ(FCI_CR);
    if (control != idle_control)
        return fail(CONTROL_MISMATCH, control);
    if (status & FCI_ERRORS)
        return fail(ERASE_ERROR, status);

    for (uint32_t offset = 0; offset < PAGE_SIZE; offset += sizeof(uint32_t))
        if (FLASH_PAGE_READ(page + offset) != UINT32_MAX)
            return fail(ERASE_VERIFY, page + offset);
    return 0;
}

static uint32_t programPage(const struct Request *request, uint32_t idle_control)
{
    uint32_t page = request->page;
    for (uint32_t offset = 0; offset < PAGE_SIZE; offset += sizeof(uint32_t))
    {
        uint32_t value = FLASH_PAGE_READ(INPUT_START + offset);
        if (value == UINT32_MAX)
            continue;

        FLASH_PAGE_WRITE(FCI_CR, idle_control | FCI_PROGRAM);
        uint32_t control = FLASH_PAGE_READ(FCI_CR);
        if (control != (idle_control | FCI_PROGRAM))
            return fail(CONTROL_MISMATCH, control);
        FLASH_PAGE_WRITE(page + offset, value);
        uint32_t status = waitIdle();
        if (status & FCI_BUSY)
            return fail(PROGRAM_TIMEOUT, status);
        control = FLASH_PAGE_READ(FCI_CR);
        if (control != (idle_control | FCI_PROGRAM))
            return fail(CONTROL_MISMATCH, control);
        FLASH_PAGE_WRITE(FCI_CR, idle_control);
        control = FLASH_PAGE_READ(FCI_CR);
        if (control != idle_control)
            return fail(CONTROL_MISMATCH, control);
        if (status & FCI_ERRORS)
            return fail(PROGRAM_ERROR, status);
        if (FLASH_PAGE_READ(page + offset) != value)
            return fail(PROGRAM_VERIFY, page + offset);
    }

    for (uint32_t offset = 0; offset < PAGE_SIZE; offset += sizeof(uint32_t))
        if (FLASH_PAGE_READ(page + offset) != FLASH_PAGE_READ(INPUT_START + offset))
            return fail(PAGE_VERIFY, page + offset);
    uint32_t crc = pageCrc(page);
    return crc == request->input_crc ? 0 : fail(PAGE_CRC_MISMATCH, crc);
}

static uint32_t finishPage(uint32_t result, uint32_t idle_control)
{
    uint32_t status = FLASH_PAGE_READ(FCI_SR);
    uint32_t control = FLASH_PAGE_READ(FCI_CR);
    if (!result && (status & FCI_ERRORS))
        result = fail(CONTROLLER_STATE, status);
    if (control == idle_control && !(status & FCI_BUSY))
    {
        if (!result)
        {
            FLASH_PAGE_WRITE(FCI_SR, FCI_CLEAR_FLAGS);
            status = FLASH_PAGE_READ(FCI_SR);
            if (status)
                result = fail(CONTROLLER_STATE, status);
        }
        if (!(status & FCI_BUSY))
        {
            FLASH_PAGE_WRITE(FCI_CR, control | FCI_LOCK);
            control = FLASH_PAGE_READ(FCI_CR);
        }
    }
    flash_page_mailbox.final_control = control;
    status = FLASH_PAGE_READ(FCI_SR);
    if (!result && status)
        return fail(CONTROLLER_STATE, status);
    if (!result && control != (idle_control | FCI_LOCK))
        return fail(RELOCK_FAILED, control);
    return result;
}

static uint32_t commitPage(uint32_t challenge, uint32_t command, const struct Request *request)
{
    if (command != (challenge ^ request->sequence ^ COMMIT_TAG))
        return fail(BAD_COMMAND, command);
    if (flash_page_mailbox.status != PREPARED || !sameRequest(challenge, request))
        return BAD_MAILBOX;
    uint32_t idle_control;
    uint32_t result = precheck(request, &idle_control);
    if (result)
        return result;
    if (!sameRequest(challenge, request))
        return BAD_MAILBOX;
    if (!converterOff())
        return CONVERTER_ACTIVE;

    FLASH_PAGE_WRITE(FCI_KEY, 0x123455aau);
    FLASH_PAGE_WRITE(FCI_KEY, 0x56786699u);
    uint32_t status = FLASH_PAGE_READ(FCI_SR);
    uint32_t control = FLASH_PAGE_READ(FCI_CR);
    if (control != idle_control || status)
        return finishPage(fail(UNLOCK_FAILED, status ? status : control), idle_control);

    flash_page_mailbox.status = ERASING;
    result = erasePage(request->page, idle_control);
    if (!result && !converterOff())
        result = CONVERTER_ACTIVE;
    if (!result)
    {
        flash_page_mailbox.status = PROGRAMMING;
        result = programPage(request, idle_control);
    }
    return finishPage(result, idle_control);
}

static bool armLoader(uint32_t challenge)
{
    if (!challenge || challenge == UINT32_MAX || (challenge & ~0xfu) == PREPARE_TAG ||
        (challenge & ~0xfu) == COMMIT_TAG || flash_page_mailbox.response || flash_page_mailbox.command ||
        flash_page_mailbox.sequence || flash_page_mailbox.page ||
        flash_page_mailbox.expected_before_crc || flash_page_mailbox.expected_input_crc ||
        flash_page_mailbox.status || flash_page_mailbox.detail || flash_page_mailbox.observed_before_crc ||
        flash_page_mailbox.observed_input_crc || flash_page_mailbox.final_control)
        return false;
    flash_page_mailbox.response = ~challenge;
    flash_page_mailbox.status = READY;
    return true;
}

#ifndef FLASH_PAGE_TEST
__attribute__((naked, noreturn, used)) void flash_page_done(void)
{
    __asm__ volatile("b .");
}

__attribute__((noinline, used)) uint32_t flash_page_wait_command(void)
{
    uint32_t command;
    do
        command = flash_page_mailbox.command;
    while (!command);
    flash_page_mailbox.command = 0;
    return command;
}

__attribute__((noreturn, used)) void flash_page_main(void)
{
    uint32_t challenge = flash_page_mailbox.challenge;
    uint32_t result = BAD_MAILBOX;
    uint32_t seen = 0;
    if (armLoader(challenge))
    {
        for (uint32_t sequence = 1; sequence <= PAGE_COUNT; ++sequence)
        {
            uint32_t command = flash_page_wait_command();
            struct Request request = {
                flash_page_mailbox.sequence,
                flash_page_mailbox.page,
                flash_page_mailbox.expected_before_crc,
                flash_page_mailbox.expected_input_crc,
            };
            result = preparePage(challenge, sequence, seen, command, &request);
            if (result)
                break;

            flash_page_mailbox.status = PREPARED;
            command = flash_page_wait_command();
            result = commitPage(challenge, command, &request);
            if (result)
                break;

            seen |= 1u << ((request.page - APP_START) / PAGE_SIZE);
            flash_page_mailbox.status = PAGE_COMPLETE;
        }
    }
    if (result)
        flash_page_mailbox.status = result;
    flash_page_done();
}

__attribute__((naked, noreturn, section(".text.flash_page_start"), used)) void flash_page_start(void)
{
    __asm__("cpsid i\n"
            "ldr r0, =flash_page_stack_top\n"
            "mov sp, r0\n"
            "bl flash_page_main\n"
            "b .\n"
            ".ltorg\n");
}
#endif
