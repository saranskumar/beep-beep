// ============================================================
//  OUTER UNIT — DISPLAY MODULE (Implementation)
//  Dual MAX7219 4-panel LED matrices using separate CS pins.
// ============================================================
#include "display.h"
#include "config.h"
#include <MD_Parola.h>
#include <MD_MAX72xx.h>
#include <SPI.h>

#define HARDWARE_TYPE MD_MAX72XX::FC16_HW

// Two independent displays instead of one zoned chain
static MD_Parola _dispNext(HARDWARE_TYPE, MATRIX_CS_NEXT, MATRIX_DEVICES);
static MD_Parola _dispCurr(HARDWARE_TYPE, MATRIX_CS_CURR, MATRIX_DEVICES);

static char _currentMsg[16] = "---";
static char _nextMsg[16]    = "---";
static bool _dirty = false;

void display_init() {
  _dispNext.begin();
  _dispCurr.begin();

  _dispNext.setIntensity(2);
  _dispCurr.setIntensity(2);

  _dirty = true;
  Serial.println(F("[DISPLAY] Dual LED matrices initialized"));
}

void display_update() {
  _dispNext.displayAnimate();
  _dispCurr.displayAnimate();

  if (_dirty) {
    _dirty = false;
    
    _dispNext.displayClear();
    _dispCurr.displayClear();

    // Set static centered text
    _dispNext.displayText(_nextMsg, PA_CENTER, 0, 0, PA_PRINT, PA_NO_EFFECT);
    _dispCurr.displayText(_currentMsg, PA_CENTER, 0, 0, PA_PRINT, PA_NO_EFFECT);
  }

  _dispNext.displayAnimate();
  _dispCurr.displayAnimate();
}

void display_show(int current, int next) {
  snprintf(_currentMsg, sizeof(_currentMsg), "%d", current);
  snprintf(_nextMsg,    sizeof(_nextMsg),    "%d", next);
  _dirty = true;
  Serial.printf("[DISPLAY] Show current=%d next=%d\n", current, next);
}

void display_show_text(const char* right_text, const char* left_text) {
  strncpy(_currentMsg, right_text, sizeof(_currentMsg) - 1);
  _currentMsg[sizeof(_currentMsg) - 1] = '\0';
  strncpy(_nextMsg, left_text, sizeof(_nextMsg) - 1);
  _nextMsg[sizeof(_nextMsg) - 1] = '\0';
  _dirty = true;
}

void display_clear() {
  display_show_text("---", "---");
}
