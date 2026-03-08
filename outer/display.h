#pragma once

// ============================================================
//  OUTER UNIT — DISPLAY MODULE (Header)
//  Controls dual-zone MAX7219 8-panel LED matrix.
//  Left 4 panels (Zone 0) = NEXT token
//  Right 4 panels (Zone 1) = CURRENT token
// ============================================================

// Initialize MD_Parola with dual zones
void display_init();

// Must be called every loop() iteration — drives MD_Parola animation engine
void display_update();

// Show integer token numbers on both zones
//   left  (NEXT zone)    e.g. 43
//   right (CURRENT zone) e.g. 42
void display_show(int current, int next);

// Show arbitrary text strings (for startup messages, errors, etc.)
void display_show_text(const char* right_text, const char* left_text);

// Clear both zones
void display_clear();
