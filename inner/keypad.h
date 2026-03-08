#pragma once

// ============================================================
//  INNER UNIT — KEYPAD MODULE (Header)
//  4×4 matrix keypad scanner with software debounce.
//
//  Default key layout:
//   [ 1 ][ 2 ][ 3 ][ A ]
//   [ 4 ][ 5 ][ 6 ][ B ]
//   [ 7 ][ 8 ][ 9 ][ C ]
//   [ * ][ 0 ][ # ][ D ]
//
//  Actions in inner.ino:
//   '#'  → Call next token
//   '*'  → Reset queue
//   'A'  → Enter LIMIT edit mode
//   'B'  → Enter CURRENT override mode
//   '0'–'9' → Digit entry (used in edit modes)
// ============================================================

typedef void (*KeypadCallback)(char key);

// Initialize row/col GPIO pins
void keypad_init();

// Scan keys — must be called every loop()
// Fires the registered callback with the pressed key character
void keypad_update();

// Register a callback to receive key press events
void keypad_set_callback(KeypadCallback cb);
