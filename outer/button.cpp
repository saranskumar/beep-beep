// ============================================================
//  OUTER UNIT — BUTTON MODULE (Implementation)
//  Debounced push button — active-LOW, internal pull-up.
// ============================================================
#include "button.h"
#include "config.h"
#include <Arduino.h>

static ButtonCallback _cb       = nullptr;
static bool           _lastState = HIGH;
static unsigned long  _lastTime  = 0;

void button_init() {
  pinMode(BTN_PIN, INPUT_PULLUP);
  _lastState = digitalRead(BTN_PIN);
  Serial.println(F("[BUTTON] Initialized"));
}

void button_set_callback(ButtonCallback cb) {
  _cb = cb;
}

void button_update() {
  bool state = digitalRead(BTN_PIN);
  unsigned long now = millis();

  if (state != _lastState && (now - _lastTime) >= BTN_DEBOUNCE_MS) {
    _lastState = state;
    _lastTime  = now;

    if (state == LOW) {            // Active-LOW: button pressed
      Serial.println(F("[BUTTON] Pressed"));
      if (_cb) _cb();
    }
  }
}
