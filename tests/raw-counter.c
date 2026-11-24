/*
 * Rad Pro raw-counter host regression test
 *
 * License: MIT
 */

#include <assert.h>
#include <mcu-renderer.h>
#include <stdio.h>
#include <stdlib.h>

#include "ca32/h240_display.h"
#include "measurements/average.h"
#include "measurements/cumulative.h"
#include "measurements/datalog.h"
#include "measurements/instantaneous.h"
#include "measurements/pulses.h"
#include "peripherals/adc.h"
#include "peripherals/display.h"
#include "peripherals/flash.h"
#include "peripherals/keyboard.h"
#include "peripherals/tube.h"
#include "system/events.h"
#include "system/power.h"
#include "system/settings.h"
#include FONT_LARGE

static OnViewEvent *activeView;
static const Menu *activeMenu;
static bool flashValid = true;
static bool panelHealthy;
static bool hvEnabled;
static unsigned int integrityChecks;
static bool running;
static const char *expectedUnit;
static const char *expectedSecondaryUnit;

volatile uint32_t tubePulseCount = UINT32_MAX - 2;
char menuOption[MENU_OPTION_STRING_SIZE];

static void sampleSecond(uint32_t count)
{
    tubePulseCount += count;
    for (uint32_t tick = 0; tick < SYSTICK_FREQUENCY; tick++)
    {
        currentTick++;
        onTick();
    }
}

static const Menu *selectOption(const Menu *menu, const char *name)
{
    assert(menu);
    for (menu_size_t index = 0; index < menu->itemCount; index++)
    {
        MenuStyle style = 0;
        if (strcmp(menu->onGetOption(index, &style), name) == 0)
        {
            menu->onSelect(index);
            return activeMenu;
        }
    }

    abort();
}

static void ignoreRectangle(mr_t *renderer, const mr_rectangle_t *rectangle)
{
    (void)renderer;
    (void)rectangle;
}

static void captureGlyphColumns(mr_t *renderer, uint8_t *pixels, uint32_t pitch, mr_rectangle_t *rectangle)
{
    bool *columns = renderer->display;
    for (int y = 0; y < rectangle->height; y++)
        for (int x = 0; x < rectangle->width; x++)
            columns[rectangle->x + x] |= pixels[y * pitch + x] != 0;
}

static void assertRenderedValue(const char *value)
{
    bool columns[DISPLAY_WIDTH] = {0};
    uint8_t buffer[H240_DISPLAY_TEXTBUFFER_SIZE] = {0};
    mr_t renderer = {
        .display = columns,
        .buffer = buffer,
        .buffer_size = sizeof(buffer),
        .font = font_large,
        .draw_string_callback = mr_draw_string_textbuffer,
        .draw_textbuffer_callback = captureGlyphColumns,
        .draw_rectangle_callback = ignoreRectangle,
    };
    const uint8_t *utf8 = (const uint8_t *)value;
    assert(mr_draw_utf8_text(&renderer, utf8,
                             &(mr_rectangle_t){0, 0, DISPLAY_WIDTH, DISPLAY_HEIGHT}, &(mr_point_t){0, 0}));

    // The numeric font leaves blank columns between adjacent glyphs.
    size_t glyphs = columns[0];
    for (size_t x = 1; x < DISPLAY_WIDTH; x++)
        glyphs += columns[x] && !columns[x - 1];

    size_t expectedGlyphs = 0;
    for (const uint8_t *c = utf8; *c; c++)
        expectedGlyphs += (*c & 0xc0) != 0x80;
    if (glyphs != expectedGlyphs)
    {
        fprintf(stderr, "Expected %zu glyphs for %s, drew %zu\n", expectedGlyphs, value, glyphs);
        abort();
    }
}

void updateView(void)
{
    if (running)
        return;
    running = true;

    // The real boot path must initialize a view even if panel setup failed.
    assert(activeView && !isPoweredOn() && !hvEnabled);
    assert(integrityChecks == 0);
    powerOn();
    assert(!isPoweredOn() && !hvEnabled);

    panelHealthy = true;
    powerOn();
    assert(isPoweredOn());
    assert(integrityChecks == 1);
    assert(!hvEnabled);
    assert(settings.doseUnits == DOSE_UNITS_CPS);
    assert(settings.secondaryDoseUnits == DOSE_UNITS_CPM);
    assert(settings.loggingMode == DATALOG_LOGGINGMODE_OFF);
    assert(!isAlertEnabled());

    assertRenderedValue("4");
    assertRenderedValue("14");
    assertRenderedValue("80");
    assertRenderedValue("81");
    assertRenderedValue("1140");
    assertRenderedValue("9999");
    assertRenderedValue("1.4");
    assertRenderedValue("1" STRING_ELLIPSIS);

    // Pulse input is ignored until the measurement view is active.
    sampleSecond(1);
    updateEvents();
    assert(hvEnabled);
    assert(getCumulativeDosePulseCount() == 0);

    // Raw averaging ignores timing options from calibrated targets.
    settings.averaging = AVERAGING_1_SECOND;

    // The hardware counter wraps in the first measured second. Simulate a busy
    // main loop by publishing the second sample before processing either one.
    sampleSecond(4);
    sampleSecond(2);
    updateEvents();
    assert(getCumulativeDosePulseCount() == 6);
    assert(getCumulativeDoseTime() == 2);
    assert(getInstantaneousRate() == 3.0F);
    assert(getAverageRate() == 3.0F);

    expectedUnit = STRING_CPS;
    expectedSecondaryUnit = STRING_CPM;
    activeView(EVENT_DRAW);
    assert(!expectedUnit && !expectedSecondaryUnit);

    sampleSecond(0);
    updateEvents();
    assert(getInstantaneousRate() == 0.0F);
    assert(getAverageRate() == 2.0F);
    assert(getCumulativeDosePulseCount() == 6);
    expectedUnit = STRING_CPS;
    expectedSecondaryUnit = STRING_CPM;
    activeView(EVENT_DRAW);
    assert(!expectedUnit && !expectedSecondaryUnit);

    activeView(EVENT_KEY_DOWN);
    expectedUnit = STRING_CPS;
    activeView(EVENT_DRAW);
    assert(!expectedUnit);

    activeView(EVENT_KEY_DOWN);
    expectedUnit = STRING_COUNTS;
    activeView(EVENT_DRAW);
    assert(!expectedUnit);

    // Verify that isolated events survive the empty seconds between them.
    for (uint32_t second = 0; second < 60; second++)
    {
        uint32_t count = second % 10 == 0;
        sampleSecond(count);
        updateEvents();
        assert(getInstantaneousRate() == (float)count);
    }
    assert(getCumulativeDosePulseCount() == 12);
    assert(getCumulativeDoseTime() == 63);

    char rateValue[32] = "";
    char rateUnit[32] = "";
    buildPulseValueString(rateValue, rateUnit, getAverageRate(), DOSE_UNITS_CPS, false);
    assert(strcmp(rateValue, "0.190") == 0);
    assert(strcmp(rateUnit, STRING_CPS) == 0);
    assertRenderedValue(rateValue);
    rateValue[0] = '\0';
    rateUnit[0] = '\0';
    buildPulseValueString(rateValue, rateUnit, getAverageRate(), DOSE_UNITS_CPM, false);
    assert(strcmp(rateValue, "11.43") == 0);
    assert(strcmp(rateUnit, STRING_CPM) == 0);
    assertRenderedValue(rateValue);

    activeView(EVENT_KEY_SELECT);
    for (menu_size_t index = 0; index < activeMenu->itemCount; index++)
    {
        MenuStyle style = 0;
        const char *item = activeMenu->onGetOption(index, &style);
        assert(strcmp(item, STRING_ALERTS) != 0);
        assert(strcmp(item, STRING_MEASUREMENTS) != 0);
        assert(strcmp(item, STRING_GEIGER_TUBE) != 0);
        assert(strcmp(item, STRING_DATALOG) != 0);
        assert(strcmp(item, STRING_POWER) != 0);
    }
    const Menu *pulsesMenu = selectOption(activeMenu, STRING_PULSES);
    const Menu *indicationMenu = selectOption(pulsesMenu, STRING_INDICATION);
    selectOption(indicationMenu, STRING_DISPLAY_FLASH);
    assert(settings.pulseDisplayFlash);

    // A transfer fault while measuring powers down and blocks a later wake.
    panelHealthy = false;
    sampleSecond(0);
    updateEvents();
    assert(!isPoweredOn() && !isMeasurementsEnabled() && !hvEnabled);
    powerOn();
    assert(!isPoweredOn() && !hvEnabled);
    stopDatalog();
    saveSettings();

    initSettings();
    assert(!settings.pulseDisplayFlash);

    // A healthy panel can enter splash, then fail before the enabling heartbeat.
    panelHealthy = true;
    powerOn();
    assert(isPoweredOn() && !hvEnabled && integrityChecks == 2);
    panelHealthy = false;
    sampleSecond(0);
    updateEvents();
    assert(!isPoweredOn() && !isMeasurementsEnabled() && !hvEnabled);

    panelHealthy = true;
    flashValid = false;
    powerOn();
    assert(integrityChecks == 3);
    sampleSecond(0);
    updateEvents();
    sampleSecond(0);
    updateEvents();
    assert(!isMeasurementsEnabled() && !hvEnabled);

    exit(0);
}

// Hardware and drawing seams. Storage and transport deliberately have no
// implementations, so any reachable call becomes a link error.
void initGPIO(void) {}
void initSystem(void) {}
void initPower(bool value) { (void)value; }
void initEvents(void) {}
void initFlash(void) {}
void initADC(void) {}
void initTube(void) {}
void initKeyboard(void) {}
void initDisplay(void) {}
bool h240DisplayHealthy(void) { return panelHealthy; }
void initView(void) {}
void resetDisplay(void) {}
void resetRNG(void) {}
void updateRNG(void) {}
void sleep(uint32_t value) { (void)value; }
void clearResetFlags(void) {}
bool wasResetByWatchdog(void) { return false; }
bool verifyFlash(void) { integrityChecks++; return flashValid; }
void setPowerEnabled(bool value) { (void)value; }
bool isUSBPowered(void) { return false; }
float readBatteryVoltage(void) { return 3.8F; }
void onKeyboardTick(void) {}
void onKeyboardUpdate(void) {}
void setKeyboardMode(KeyboardMode value) { (void)value; }
bool isLockModeEnabled(void) { return false; }
void setBacklight(bool value) { (void)value; }
bool isDisplayEnabled(void) { return false; }
bool readTubeDet(void) { return false; }
void setTubeHVEnabled(bool value)
{
    assert(!value || panelHealthy);
    hvEnabled = value;
}
void showRNGMenu(void) {}
void showDisplayMenu(void) {}
void requestViewUpdate(void) {}

void showView(OnViewEvent *onViewEvent) { activeView = onViewEvent; }
void updateViewHeartbeat(void) { activeView(EVENT_HEARTBEAT); }
void showMenu(const Menu *menu) { activeMenu = menu; }
void selectMenuItem(const Menu *menu, menu_size_t index)
{
    menu->state->selectedIndex = index;
}

void drawSplash(const char *message) { (void)message; }
void drawPowerOff(bool battery) { (void)battery; }
void drawTitleBar(const char *title) { (void)title; }
void drawMeasurementAlert(const char *message) { (void)message; }
void drawMeasurementBar(float value, int32_t minExponent, float warning, float alarm)
{
    (void)value;
    (void)minExponent;
    assert(warning == 0 && alarm == 0);
}
void drawMeasurementValue(const char *value, const char *unit, float confidence, MeasurementStyle style)
{
    (void)confidence;
    assert(style == MEASUREMENTSTYLE_NORMAL);
    assert(expectedUnit && strstr(unit, expectedUnit));
    expectedUnit = NULL;
    assertRenderedValue(value);
}
void drawMeasurementInfo(const char *key, const char *value, const char *unit, MeasurementStyle style)
{
    (void)key;
    (void)value;
    assert(style == MEASUREMENTSTYLE_NORMAL);
    if (expectedSecondaryUnit)
    {
        assert(strstr(unit, expectedSecondaryUnit));
        expectedSecondaryUnit = NULL;
    }
}
