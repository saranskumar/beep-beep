#pragma once

// ============================================================
//  OUTER UNIT — BUTTON MODULE (Header)
//  Debounced push button for customer token requests.
//  Active-LOW (internal pull-up enabled).
// ============================================================

typedef void (*ButtonCallback)();

// Initialize GPIO and internal pull-up
void button_init();

// Poll button state — must be called every loop()
// Fires the registered callback on a confirmed press
void button_update();

// Register a callback to fire when the button is pressed
void button_set_callback(ButtonCallback cb);
