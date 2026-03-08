#pragma once

// ============================================================
//  INNER UNIT — CONFIG
//  All pin numbers and compile-time constants live here.
//  Edit this file when adapting to different wiring.
// ============================================================

// ----------------------------------------------------------
// ST7735 TFT Display (via SPI)
// ----------------------------------------------------------
#define TFT_CS_PIN   15
#define TFT_DC_PIN    2
#define TFT_RST_PIN   4

// ----------------------------------------------------------
// 4×4 Matrix Keypad
// Row pins: top-to-bottom; Col pins: left-to-right
// ----------------------------------------------------------
#define KP_ROW0  32
#define KP_ROW1  33
#define KP_ROW2  25
#define KP_ROW3  26
#define KP_COL0  27
#define KP_COL1  14
#define KP_COL2  12
#define KP_COL3  13

// ----------------------------------------------------------
// Buzzer (optional — wire active-high piezo buzzer)
// ----------------------------------------------------------
#define BUZZER_PIN   16   // Reassigned from 27 to avoid keypad column conflict
#define BUZZER_MS    80   // Default beep duration ms

// ----------------------------------------------------------
// ESP-NOW Communication
// ----------------------------------------------------------
// MAC address of the OUTER (public) ESP32.
// Run `Serial.println(WiFi.macAddress());` on the Outer ESP32
// once to get its MAC, then fill it in here.
// Example: {0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF}
#define OUTER_MAC   {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}  // ← REPLACE THIS

// ----------------------------------------------------------
// Queue / Token Limits
// ----------------------------------------------------------
#define TOKEN_DEFAULT_LIMIT  100
#define TOKEN_MIN              1
#define TOKEN_MAX            999
