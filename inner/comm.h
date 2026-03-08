#pragma once

// ============================================================
//  INNER UNIT — COMM MODULE (Header) — ESP-NOW
//  Sends token update messages to the Outer unit.
//
//  IMPORTANT: Set OUTER_MAC in config.h to the MAC address
//  of your Outer ESP32 before flashing.
//  Get MAC by running this on Outer unit once:
//    Serial.println(WiFi.macAddress());
// ============================================================

// Initialize WiFi in STA mode and register Outer unit as ESP-NOW peer
void comm_init();

// Send CALL message: Outer unit updates display + announces token
void comm_send_call(int current, int next);

// Send RESET message: Outer unit clears display
void comm_send_reset();
