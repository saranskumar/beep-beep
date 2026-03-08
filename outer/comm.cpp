// ============================================================
//  OUTER UNIT — COMM MODULE (Implementation) — ESP-NOW
//  Receives CALL / RESET messages from the Inner unit.
// ============================================================
#include "comm.h"
#include "config.h"
#include <Arduino.h>
#include <WiFi.h>
#include <esp_now.h>

static CommCallCallback  _callCb  = nullptr;
static CommResetCallback _resetCb = nullptr;

// Pending message state — set in ISR callback, dispatched in loop()
static volatile bool _pending     = false;
static volatile int  _pendCurrent = 0;
static volatile int  _pendNext    = 0;
static volatile uint8_t _pendType = 0;

// ESP-NOW receive callback (runs in WiFi task context — keep short)
static void _onReceive(const uint8_t* mac, const uint8_t* data, int len) {
  if (len != sizeof(CommPayload)) return;

  const CommPayload* msg = reinterpret_cast<const CommPayload*>(data);
  _pendType    = msg->type;
  _pendCurrent = msg->current;
  _pendNext    = msg->next;
  _pending     = true;
}

void comm_init() {
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();

  if (esp_now_init() != ESP_OK) {
    Serial.println(F("[COMM] ERROR: ESP-NOW init failed"));
    return;
  }

  esp_now_register_recv_cb(_onReceive);
  Serial.print(F("[COMM] Outer MAC: "));
  Serial.println(WiFi.macAddress());
  Serial.println(F("[COMM] ESP-NOW receiver ready"));
}

void comm_set_call_callback(CommCallCallback cb)  { _callCb  = cb; }
void comm_set_reset_callback(CommResetCallback cb) { _resetCb = cb; }

// Dispatch pending message safely from main loop (not ISR context)
void comm_update() {
  if (!_pending) return;
  _pending = false;

  if (_pendType == MSG_CALL) {
    Serial.printf("[COMM] CALL: current=%d next=%d\n", _pendCurrent, _pendNext);
    if (_callCb) _callCb(_pendCurrent, _pendNext);
  } else if (_pendType == MSG_RESET) {
    Serial.println(F("[COMM] RESET received"));
    if (_resetCb) _resetCb();
  }
}
