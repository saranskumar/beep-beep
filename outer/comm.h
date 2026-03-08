#pragma once

// ============================================================
//  OUTER UNIT — COMM MODULE (Header) — ESP-NOW
//  Receives messages from the Inner unit.
//
//  Message payload struct (shared between both units):
//    type    : MSG_CALL or MSG_RESET
//    current : token being served
//    next    : next token
//
//  No WiFi router required — peer-to-peer local radio.
//  Inner unit's MAC address must match INNER_MAC in config.h
// ============================================================

// Message types
#define MSG_CALL  1
#define MSG_RESET 2

// Payload sent over ESP-NOW
typedef struct {
  uint8_t type;       // MSG_CALL or MSG_RESET
  int     current;    // current token
  int     next;       // next token
} CommPayload;

// Callback types
typedef void (*CommCallCallback)(int current, int next);
typedef void (*CommResetCallback)();

// Initialize WiFi in STA mode and register ESP-NOW receive callback
void comm_init();

// Register application-level callbacks
void comm_set_call_callback(CommCallCallback cb);
void comm_set_reset_callback(CommResetCallback cb);

// No comm_update() needed — ESP-NOW fires the ISR-side callback automatically
// The module uses a flag + main-thread dispatch to keep callbacks off the ISR
void comm_update();  // Call in loop() to dispatch pending callbacks safely
