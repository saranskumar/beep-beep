#pragma once

// ============================================================
//  OUTER UNIT — PRINTER MODULE (Header)
//  Controls serial thermal printer (ESC/POS compatible).
//
//  Printed slip example:
//  ----------------------
//     SERVICE TOKEN
//  ----------------------
//  Token  : 034
//  Please wait for your
//  turn.
//  ----------------------
// ============================================================

// Initialize printer serial port
void printer_init();

// Print a token slip for the given token number
void printer_print_token(int tokenNumber);
