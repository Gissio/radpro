/* Rad Pro H240 page-loader tests with modeled MMIO. License: MIT */

#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

static uint32_t readMock(uint32_t address);
static void writeMock(uint32_t address, uint32_t value);

#define FLASH_PAGE_TEST
#define FLASH_PAGE_READ readMock
#define FLASH_PAGE_WRITE writeMock
#include "flash-page.c"

#define WORDS (PAGE_SIZE / sizeof(uint32_t))

enum Fault
{
    NONE,
    ERASE_STUCK,
    ERASE_INCOMPLETE,
    PROGRAM_STUCK,
    PROGRAM_PROTECTED,
    INPUT_MUTATED_DURING_PROGRAM,
};

static struct
{
    uint32_t app[PAGE_COUNT * WORDS];
    uint32_t input[WORDS];
    uint32_t idle_control;
    uint32_t control;
    uint32_t status;
    uint32_t erase_address;
    unsigned busy_reads;
    unsigned writes;
    unsigned keys;
    unsigned erases;
    unsigned programs;
    unsigned locks;
    enum Fault fault;
    bool converter_active;
} mock;

static uint32_t readMock(uint32_t address)
{
    switch (address)
    {
    case FCI_CR:
        return mock.control;
    case FCI_SR:
        if (mock.busy_reads && --mock.busy_reads == 0)
            mock.status = 0x20u;
        return mock.status;
    case 0x4002203cu:
        return 0x4302u;
    case 0x400220c0u:
    case 0x400220d0u:
        return 0;
    case 0x4002101cu:
        return mock.converter_active ? 0x100u : 0;
    default:
        assert(!(address & 3u));
        if (address >= INPUT_START && address < INPUT_START + PAGE_SIZE)
        {
            if (mock.fault == INPUT_MUTATED_DURING_PROGRAM && flash_page_mailbox.status == PROGRAMMING)
            {
                mock.input[0] ^= 1u;
                mock.fault = NONE;
            }
            return mock.input[(address - INPUT_START) / sizeof(uint32_t)];
        }
        assert(address >= APP_START && address < APP_END && !(mock.status & FCI_BUSY));
        return mock.app[(address - APP_START) / sizeof(uint32_t)];
    }
}

static void writeMock(uint32_t address, uint32_t value)
{
    assert(!(mock.status & FCI_BUSY));
    ++mock.writes;
    switch (address)
    {
    case FCI_KEY:
        assert(mock.control == mock.idle_control || mock.control == (mock.idle_control | FCI_LOCK));
        assert(value == (mock.keys % 2 == 0 ? 0x123455aau : 0x56786699u));
        if (++mock.keys % 2 == 0)
            mock.control = mock.idle_control;
        break;
    case FCI_SR:
        assert(value == FCI_CLEAR_FLAGS && mock.control == mock.idle_control);
        mock.status &= ~value;
        break;
    case FCI_AR:
        assert(mock.control == (mock.idle_control | FCI_PAGE_ERASE));
        assert(value >= APP_START && value < APP_END && !(value & (PAGE_SIZE - 1u)));
        mock.erase_address = value;
        break;
    case FCI_CR:
        assert(mock.keys && mock.keys % 2 == 0);
        if (value & FCI_START)
        {
            assert(mock.erase_address == flash_page_mailbox.page && mock.erase_address >= APP_START);
            assert(mock.control == (mock.idle_control | FCI_PAGE_ERASE));
            assert(value == (mock.idle_control | FCI_PAGE_ERASE | FCI_START));
            ++mock.erases;
            uint32_t *page = &mock.app[(mock.erase_address - APP_START) / sizeof(uint32_t)];
            memset(page, 0xff, PAGE_SIZE);
            if (mock.fault == ERASE_INCOMPLETE)
                page[WORDS - 1] = 0;
            mock.status = FCI_BUSY;
            mock.busy_reads = mock.fault == ERASE_STUCK ? 0 : 3;
            value &= ~FCI_START;
        }
        else if (value & FCI_LOCK)
        {
            assert(mock.control == mock.idle_control && value == (mock.idle_control | FCI_LOCK));
            ++mock.locks;
        }
        else
            assert(value == mock.idle_control || value == (mock.idle_control | FCI_PAGE_ERASE) ||
                   value == (mock.idle_control | FCI_PROGRAM));
        mock.control = value;
        break;
    default:
        assert(mock.control == (mock.idle_control | FCI_PROGRAM) && value != UINT32_MAX);
        assert(address >= mock.erase_address && address < mock.erase_address + PAGE_SIZE && !(address & 3u));
        assert(mock.erases > 0);
        ++mock.programs;
        if (mock.fault == PROGRAM_PROTECTED)
        {
            mock.status = 8;
            break;
        }
        mock.app[(address - APP_START) / sizeof(uint32_t)] &= value;
        mock.status = FCI_BUSY;
        mock.busy_reads = mock.fault == PROGRAM_STUCK ? 0 : 3;
        break;
    }
}

static void begin(void)
{
    memset(&mock, 0, sizeof(mock));
    memset((void *)&flash_page_mailbox, 0, sizeof(flash_page_mailbox));
    memset(mock.app, 0xff, sizeof(mock.app));
    memset(mock.input, 0xff, sizeof(mock.input));
    assert(pageCrc(INPUT_START) == 0xf154670au);
    mock.input[0] = 0x12345678u;
    mock.input[WORDS - 1] = 0xa5a55a5au;
    assert(pageCrc(INPUT_START) == 0x7e2f1f02u);
    mock.idle_control = FCI_IDLE_CHARGE;
    mock.control = mock.idle_control;
    flash_page_mailbox.challenge = 0x24681357u;
    assert(armLoader(flash_page_mailbox.challenge));
    assert(flash_page_mailbox.response == ~flash_page_mailbox.challenge && flash_page_mailbox.status == READY);
}

static struct Request stage(uint32_t sequence, uint32_t page)
{
    struct Request request = {sequence, page, pageCrc(APP_START), pageCrc(INPUT_START)};
    flash_page_mailbox.sequence = sequence;
    flash_page_mailbox.page = page;
    flash_page_mailbox.expected_before_crc = request.before_crc;
    flash_page_mailbox.expected_input_crc = request.input_crc;
    return request;
}

static uint32_t prepare(const struct Request *request, uint32_t sequence, uint32_t seen)
{
    uint32_t challenge = flash_page_mailbox.challenge;
    unsigned previous_writes = mock.writes;
    uint32_t result = preparePage(challenge, sequence, seen, challenge ^ sequence ^ PREPARE_TAG, request);
    assert(mock.writes == previous_writes);
    if (!result)
        flash_page_mailbox.status = PREPARED;
    return result;
}

static uint32_t commit(const struct Request *request)
{
    uint32_t challenge = flash_page_mailbox.challenge;
    uint32_t result = commitPage(challenge, challenge ^ request->sequence ^ COMMIT_TAG, request);
    if (!result)
        flash_page_mailbox.status = PAGE_COMPLETE;
    return result;
}

int main(void)
{
    begin();
    assert(!armLoader(flash_page_mailbox.challenge));
    struct Request request = stage(1, APP_END);
    assert(prepare(&request, 1, 0) == BAD_PAGE);
    request = stage(1, APP_START + 4);
    assert(prepare(&request, 1, 0) == BAD_PAGE);
    request = stage(1, APP_START);
    assert(prepare(&request, 1, 1) == BAD_PAGE);
    assert(prepare(&request, 2, 0) == BAD_SEQUENCE);
    mock.converter_active = true;
    assert(prepare(&request, 1, 0) == CONVERTER_ACTIVE);

    begin();
    request = stage(1, APP_START);
    assert(prepare(&request, 1, 0) == 0);
    mock.input[WORDS - 1] ^= 1u;
    assert(commit(&request) == INPUT_CRC_MISMATCH && mock.writes == 0);

    begin();
    request = stage(1, APP_START);
    assert(prepare(&request, 1, 0) == 0);
    mock.app[WORDS - 1] = 0;
    assert(commit(&request) == TARGET_CRC_MISMATCH && mock.writes == 0);

    begin();
    request = stage(1, APP_START);
    assert(prepare(&request, 1, 0) == 0);
    flash_page_mailbox.page += PAGE_SIZE;
    assert(commit(&request) == BAD_MAILBOX && mock.writes == 0);

    begin();
    request = stage(1, APP_START);
    assert(prepare(&request, 1, 0) == 0);
    assert(commit(&request) == 0 && mock.programs == 2 && mock.erases == 1 && mock.locks == 1);
    assert(pageCrc(APP_START) == request.input_crc && mock.control == (FCI_IDLE_CHARGE | FCI_LOCK));
    request = stage(2, APP_END - PAGE_SIZE);
    request.before_crc = 0xf154670au;
    flash_page_mailbox.expected_before_crc = request.before_crc;
    assert(prepare(&request, 2, 1) == 0);
    assert(commit(&request) == 0 && mock.programs == 4 && mock.erases == 2 && mock.locks == 2);
    assert(pageCrc(APP_END - PAGE_SIZE) == request.input_crc);
    assert(pageCrc(APP_START + PAGE_SIZE) == 0xf154670au && mock.status == 0);
    assert(flash_page_mailbox.final_control == (FCI_IDLE_CHARGE | FCI_LOCK));

    begin();
    mock.idle_control = FCI_IDLE_RESET;
    mock.control = FCI_IDLE_RESET | FCI_LOCK;
    for (unsigned word = 0; word < WORDS; ++word)
        mock.input[word] = 0x12340000u + word;
    request = stage(1, APP_START);
    assert(prepare(&request, 1, 0) == 0);
    assert(flash_page_mailbox.final_control == (FCI_IDLE_RESET | FCI_LOCK));
    assert(commit(&request) == 0 && mock.programs == WORDS && mock.erases == 1 && mock.locks == 1);
    assert(memcmp(mock.app, mock.input, PAGE_SIZE) == 0 && mock.status == 0);
    assert(flash_page_mailbox.final_control == (FCI_IDLE_RESET | FCI_LOCK));

    begin();
    mock.control = FCI_IDLE_RESET | FCI_LOCK | 4u;
    request = stage(1, APP_START);
    assert(prepare(&request, 1, 0) == CONTROLLER_STATE && mock.writes == 0);

    const struct
    {
        enum Fault fault;
        uint32_t status;
        unsigned programs;
        unsigned locks;
    } failures[] = {
        {ERASE_STUCK, ERASE_TIMEOUT, 0, 0},
        {ERASE_INCOMPLETE, ERASE_VERIFY, 0, 1},
        {PROGRAM_STUCK, PROGRAM_TIMEOUT, 1, 0},
        {PROGRAM_PROTECTED, PROGRAM_ERROR, 1, 1},
        {INPUT_MUTATED_DURING_PROGRAM, PAGE_CRC_MISMATCH, 2, 1},
    };
    for (unsigned i = 0; i < sizeof(failures) / sizeof(failures[0]); ++i)
    {
        begin();
        mock.fault = failures[i].fault;
        request = stage(1, APP_START);
        assert(prepare(&request, 1, 0) == 0);
        assert(commit(&request) == failures[i].status && mock.erases == 1);
        assert(mock.programs == failures[i].programs && mock.locks == failures[i].locks);
    }

    begin();
    mock.keys = 2;
    mock.status = FCI_CLEAR_FLAGS;
    assert(finishPage(0, mock.idle_control) == CONTROLLER_STATE);
    assert(flash_page_mailbox.detail == FCI_CLEAR_FLAGS && mock.status == FCI_CLEAR_FLAGS);
    assert(mock.writes == 1 && mock.locks == 1);
    assert(flash_page_mailbox.final_control == (FCI_IDLE_CHARGE | FCI_LOCK));
    puts("H240 flash page MMIO tests passed");
}
