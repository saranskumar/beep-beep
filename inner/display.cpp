// ============================================================
//  INNER UNIT — DISPLAY MODULE (Implementation)
//  ST7735 TFT operator dashboard.
//  Layout:
//    Row 0 (header)  — "TOKEN QUEUE"
//    Row 1           — CURRENT  xxx
//    Row 2           — NEXT     xxx
//    Row 3           — LAST     xxx
//    Row 4           — LIMIT    xxx
//    Row 5 (status)  — message text
// ============================================================
#include "display.h"
#include "config.h"
#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>
#include <SPI.h>

// Colour palette
#define COL_BG       ST77XX_BLACK
#define COL_HEADER   0x07FF   // Cyan
#define COL_LABEL    0xAD55   // Light grey
#define COL_VALUE    ST77XX_WHITE
#define COL_STATUS   0xFD20   // Orange

// Layout constants (pixels)
#define ROW_HEADER_Y   4
#define ROW_START_Y   30
#define ROW_HEIGHT    22
#define LABEL_X        4
#define VALUE_X       90
#define STATUS_Y     118

static Adafruit_ST7735 _tft(TFT_CS_PIN, TFT_DC_PIN, TFT_RST_PIN);

static int  _cur = 0, _nxt = 0, _lst = 0, _lim = 0;
static bool _dirty = true;
static char _statusMsg[24] = "";

// Draw a single labelled row
static void _drawRow(int rowIdx, const char* label, int value) {
  int y = ROW_START_Y + rowIdx * ROW_HEIGHT;

  // Clear row
  _tft.fillRect(0, y, 160, ROW_HEIGHT - 2, COL_BG);

  // Label
  _tft.setTextColor(COL_LABEL);
  _tft.setTextSize(1);
  _tft.setCursor(LABEL_X, y + 5);
  _tft.print(label);

  // Value
  _tft.setTextColor(COL_VALUE);
  _tft.setTextSize(2);
  _tft.setCursor(VALUE_X, y + 2);
  _tft.printf("%03d", value);
}

void display_init() {
  _tft.initR(INITR_BLACKTAB);
  _tft.setRotation(1);          // Landscape
  _tft.fillScreen(COL_BG);

  // Static header
  _tft.setTextColor(COL_HEADER);
  _tft.setTextSize(1);
  _tft.setCursor(30, ROW_HEADER_Y);
  _tft.print("TOKEN QUEUE SYSTEM");

  // Divider line
  _tft.drawFastHLine(0, 18, 160, COL_HEADER);
  _tft.drawFastHLine(0, 110, 160, COL_LABEL);

  _dirty = true;
  Serial.println(F("[DISPLAY] TFT initialized"));
}

void display_set_values(int current, int next, int last, int limit) {
  _cur = current;
  _nxt = next;
  _lst = last;
  _lim = limit;
  _dirty = true;
}

void display_mark_dirty() {
  _dirty = true;
}

void display_update() {
  if (!_dirty) return;
  _dirty = false;

  _drawRow(0, "CURRENT", _cur);
  _drawRow(1, "NEXT   ", _nxt);
  _drawRow(2, "LAST   ", _lst);
  _drawRow(3, "LIMIT  ", _lim);

  // Status bar
  _tft.fillRect(0, STATUS_Y, 160, 10, COL_BG);
  if (_statusMsg[0] != '\0') {
    _tft.setTextColor(COL_STATUS);
    _tft.setTextSize(1);
    _tft.setCursor(LABEL_X, STATUS_Y);
    _tft.print(_statusMsg);
  }
}

void display_show_status(const char* msg) {
  strncpy(_statusMsg, msg, sizeof(_statusMsg) - 1);
  _statusMsg[sizeof(_statusMsg) - 1] = '\0';
  _dirty = true;
}
