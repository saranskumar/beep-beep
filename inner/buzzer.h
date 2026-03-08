#pragma once

// ============================================================
//  INNER UNIT — BUZZER MODULE (Header)
//  Optional piezo buzzer for operator feedback.
// ============================================================

// Initialize buzzer pin
void buzzer_init();

// Play a single short beep
void buzzer_beep(int durationMs);

// Play two short beeps (e.g. for mode change or error)
void buzzer_double_beep();
