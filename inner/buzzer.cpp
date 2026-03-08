// ============================================================
//  INNER UNIT — BUZZER MODULE (Implementation)
//  Active-high piezo buzzer via GPIO.
// ============================================================
#include "buzzer.h"
#include "config.h"
#include <Arduino.h>

void buzzer_init() {
  pinMode(BUZZER_PIN, OUTPUT);
  digitalWrite(BUZZER_PIN, LOW);
  Serial.println(F("[BUZZER] Initialized"));
}

void buzzer_beep(int durationMs) {
  digitalWrite(BUZZER_PIN, HIGH);
  delay(durationMs);
  digitalWrite(BUZZER_PIN, LOW);
}

void buzzer_double_beep() {
  buzzer_beep(BUZZER_MS);
  delay(80);
  buzzer_beep(BUZZER_MS);
}
