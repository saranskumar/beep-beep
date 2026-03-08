// ============================================================
//  INNER UNIT — KEYPAD MODULE (Implementation)
//  4×4 matrix keypad with software debounce.
//  Uses direct GPIO scan — no external library needed.
//
//  Key map:
//   '1','2','3','A'
//   '4','5','6','B'
//   '7','8','9','C'
//   '*','0','#','D'
// ============================================================
#include "keypad.h"
#include "config.h"
#include <Arduino.h>

static const uint8_t ROWS = 4;
static const uint8_t COLS = 4;

static const char KEYS[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

static const uint8_t ROW_PINS[ROWS] = { KP_ROW0, KP_ROW1, KP_ROW2, KP_ROW3 };
static const uint8_t COL_PINS[COLS] = { KP_COL0, KP_COL1, KP_COL2, KP_COL3 };

#define DEBOUNCE_MS  50

static KeypadCallback _cb        = nullptr;
static char     _lastKey         = '\0';
static unsigned long _lastPressMs = 0;

void keypad_init() {
  // Rows → OUTPUT, start HIGH
  for (uint8_t r = 0; r < ROWS; r++) {
    pinMode(ROW_PINS[r], OUTPUT);
    digitalWrite(ROW_PINS[r], HIGH);
  }
  // Cols → INPUT with pull-up
  for (uint8_t c = 0; c < COLS; c++) {
    pinMode(COL_PINS[c], INPUT_PULLUP);
  }
  Serial.println(F("[KEYPAD] Initialized"));
}

void keypad_set_callback(KeypadCallback cb) {
  _cb = cb;
}

void keypad_update() {
  char pressed = '\0';

  for (uint8_t r = 0; r < ROWS; r++) {
    digitalWrite(ROW_PINS[r], LOW);     // Drive row LOW

    for (uint8_t c = 0; c < COLS; c++) {
      if (digitalRead(COL_PINS[c]) == LOW) {   // Column pulled LOW → key pressed
        pressed = KEYS[r][c];
      }
    }

    digitalWrite(ROW_PINS[r], HIGH);    // Restore row HIGH
  }

  unsigned long now = millis();

  if (pressed != '\0') {
    // New key or debounce window expired
    if (pressed != _lastKey || (now - _lastPressMs) > DEBOUNCE_MS) {
      _lastKey      = pressed;
      _lastPressMs  = now;
      Serial.printf("[KEYPAD] Key: %c\n", pressed);
      if (_cb) _cb(pressed);
    }
  } else {
    // No key held — reset so same key can repeat next press
    if ((now - _lastPressMs) > DEBOUNCE_MS) {
      _lastKey = '\0';
    }
  }
}
