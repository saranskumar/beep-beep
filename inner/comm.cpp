// ============================================================
//  INNER UNIT — COMM MODULE (Implementation) — ESP-NOW
//  Sends token update messages to the Outer unit.
//
//  SETUP STEP: Flash Outer unit first, read its MAC from Serial,
//  paste it into OUTER_MAC in config.h, then flash Inner unit.
// ============================================================
#include "comm.h"
#include "comm.h"
#include "config.h"
#include <Arduino.h>
#include <WiFi.h>
#include <esp_now.h>

// Shared payload struct — must match outer/comm.h
typedef struct {
  uint8_t type;
  int     current;
  int     next;
} CommPayload;

#define MSG_CALL  1
#define MSG_RESET 2

static esp_now_peer_info_t _peer;

// Send status callback (optional logging)
static void _onSent(const uint8_t* mac, esp_now_send_status_t status) {
  if (status == ESP_NOW_SEND_SUCCESS) {
    Serial.println(F("[COMM] Send OK"));
  } else {
    Serial.println(F("[COMM] Send FAILED — check Outer MAC and proximity"));
  }
}

void comm_init() {
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();

  if (esp_now_init() != ESP_OK) {
    Serial.println(F("[COMM] ERROR: ESP-NOW init failed"));
    return;
  }

  esp_now_register_send_cb(_onSent);

  // Register the Outer unit as a peer
  uint8_t outerMac[] = OUTER_MAC;
  memset(&_peer, 0, sizeof(_peer));
  memcpy(_peer.peer_addr, outerMac, 6);
  _peer.channel = 0;    // Use current WiFi channel
  _peer.encrypt = false;

  if (esp_now_add_peer(&_peer) != ESP_OK) {
    Serial.println(F("[COMM] ERROR: Failed to add Outer peer — check OUTER_MAC in config.h"));
    return;
  }

  Serial.print(F("[COMM] Inner MAC: "));
  Serial.println(WiFi.macAddress());
  Serial.println(F("[COMM] ESP-NOW sender ready"));
}

static void _send(CommPayload& payload) {
  uint8_t outerMac[] = OUTER_MAC;
  esp_err_t result = esp_now_send(outerMac,
                                   reinterpret_cast<uint8_t*>(&payload),
                                   sizeof(payload));
  if (result != ESP_OK) {
    Serial.println(F("[COMM] esp_now_send error"));
  }
}

void comm_send_call(int current, int next) {
  CommPayload msg = { MSG_CALL, current, next };
  Serial.printf("[COMM] Sending CALL: current=%d next=%d\n", current, next);
  _send(msg);
}

void comm_send_reset() {
  CommPayload msg = { MSG_RESET, 0, 0 };
  Serial.println(F("[COMM] Sending RESET"));
  _send(msg);
}
