#pragma once

// ============================================================
//  OUTER UNIT — CONFIG
//  All pin numbers and compile-time constants live here.
//  Edit this file when adapting to different wiring.
// ============================================================

// ----------------------------------------------------------
// MAX7219 LED Matrix (via SPI)
// Two separate 4-panel boards, each with its own CS pin
// ----------------------------------------------------------
#define MATRIX_CS_NEXT   5    // CS pin for NEXT token matrix (4 panels)
#define MATRIX_CS_CURR   4    // CS pin for CURRENT token matrix (4 panels) — CHANGE IF NEEDED
#define MATRIX_DEVICES   4    // Panels per chain

// ----------------------------------------------------------
// DFPlayer Mini (via HardwareSerial 1)
// ----------------------------------------------------------
#define DFPLAYER_RX_PIN   16   // Connect to DFPlayer TX
#define DFPLAYER_TX_PIN   17   // Connect to DFPlayer RX (use 1k resistor)
#define DFPLAYER_VOLUME   25   // 0–30

// ----------------------------------------------------------
// Thermal Printer (via SoftwareSerial / HardwareSerial2)
// ----------------------------------------------------------
#define PRINTER_RX_PIN   13   // ESP32 RX ← Printer TX
#define PRINTER_TX_PIN   12   // ESP32 TX → Printer RX
#define PRINTER_BAUD     9600

// ----------------------------------------------------------
// Token Request Button
// ----------------------------------------------------------
#define BTN_PIN          0    // Active-LOW push button
#define BTN_DEBOUNCE_MS  50   // Debounce window

// ----------------------------------------------------------
// ESP-NOW Communication
// ----------------------------------------------------------
// MAC address of the INNER unit.
// Run `Serial.println(WiFi.macAddress());` on the Inner ESP32
// once to get its MAC, then fill it in here.
// Example: {0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF}
#define INNER_MAC   {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}  // ← REPLACE THIS

// ----------------------------------------------------------
// Token Numbering
// ----------------------------------------------------------
#define TOKEN_MIN        1
#define TOKEN_MAX        999
