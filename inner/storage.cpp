// ============================================================
//  INNER UNIT — STORAGE MODULE (Implementation)
//  Persistent queue state via ESP32 NVS.
//  Values survive power cycles and resets.
// ============================================================
#include "storage.h"
#include "config.h"
#include <Arduino.h>
#include <Preferences.h>

// NVS namespace key — must be ≤ 15 chars
#define NVS_NAMESPACE "tokenq"

static Preferences _prefs;

void storage_init() {
  _prefs.begin(NVS_NAMESPACE, false);  // false = read-write mode
  Serial.println(F("[STORAGE] NVS initialized"));
}

void storage_load(int* current, int* next, int* last, int* limit) {
  *current = _prefs.getInt("current",  0);
  *next    = _prefs.getInt("next",     1);
  *last    = _prefs.getInt("last",     0);
  *limit   = _prefs.getInt("limit",   TOKEN_DEFAULT_LIMIT);
  Serial.printf("[STORAGE] Loaded → C=%d N=%d L=%d LIM=%d\n",
                *current, *next, *last, *limit);
}

void storage_save(int current, int next, int last, int limit) {
  _prefs.putInt("current", current);
  _prefs.putInt("next",    next);
  _prefs.putInt("last",    last);
  _prefs.putInt("limit",   limit);
  Serial.printf("[STORAGE] Saved → C=%d N=%d L=%d LIM=%d\n",
                current, next, last, limit);
}

void storage_reset() {
  _prefs.clear();
  Serial.println(F("[STORAGE] NVS cleared"));
}
