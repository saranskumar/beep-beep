#pragma once

// ============================================================
//  INNER UNIT — STORAGE MODULE (Header)
//  Persistent state via ESP32 NVS (Non-Volatile Storage).
//  Survives power cycles and resets.
// ============================================================

// Open the NVS namespace and load defaults if first boot
void storage_init();

// Load saved queue state — call after storage_init()
void storage_load(int* current, int* next, int* last, int* limit);

// Persist current queue state — call after every state change
void storage_save(int current, int next, int last, int limit);

// Erase all saved values (useful for full factory reset)
void storage_reset();
