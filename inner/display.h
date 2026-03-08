#pragma once

// ============================================================
//  INNER UNIT — DISPLAY MODULE (Header)
//  ST7735 TFT operator dashboard.
//
//  Screen shows four rows:
//    CURRENT   042
//    NEXT      043
//    LAST      078
//    LIMIT     100
// ============================================================

// Initialize TFT and draw static UI shell
void display_init();

// Refresh screen rows that have changed (dirty-flag-driven)
// Call once per loop — internally tracks what needs redrawing
void display_update();

// Mark the display as needing a full redraw on next display_update()
void display_mark_dirty();

// Update the displayed values and trigger a redraw
void display_set_values(int current, int next, int last, int limit);

// Show a temporary status message on the bottom row (e.g. "RESET OK")
void display_show_status(const char* msg);
