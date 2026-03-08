// ============================================================
//  INNER UNIT — MAIN SKETCH
//  Operator-facing ESP32: TFT dashboard, keypad, queue logic,
//  NVS persistence, ESP-NOW transmission to Outer unit.
//
//  This file is the orchestrator only.
//  All hardware logic lives in the module files below.
//
//  Module responsibilities:
//    config.h     — pin definitions & constants
//    storage      — NVS persistent state (survives power loss)
//    queue        — token queue business logic (pure logic)
//    display      — ST7735 TFT operator dashboard
//    keypad       — 4×4 matrix keypad scanner
//    comm         — ESP-NOW transmitter to Outer unit
//    buzzer       — optional piezo feedback
//
//  Keypad actions:
//    '#'     → Call next token (CURRENT = NEXT, NEXT++)
//    '*'     → Reset full queue
//    'A'     → Enter LIMIT edit mode (type digits → '#' to confirm)
//    'B'     → Enter CURRENT override mode
//    '0'-'9' → Digit entry (used in edit modes)
//    'D'     → Cancel edit mode
// ============================================================

#include "config.h"
#include "storage.h"
#include "queue.h"
#include "display.h"
#include "keypad.h"
#include "comm.h"
#include "buzzer.h"

// ---- Edit mode state --------------------------------------
enum EditMode { EDIT_NONE, EDIT_LIMIT, EDIT_CURRENT };
static EditMode _editMode   = EDIT_NONE;
static char     _editBuf[8] = "";   // Digit buffer during edit
static uint8_t  _editLen    = 0;

// ---- Helper: save state and push to Outer unit ------------
static void _syncAll() {
  int c = queue_get_current();
  int n = queue_get_next();
  int l = queue_get_last();
  int m = queue_get_limit();

  storage_save(c, n, l, m);
  display_set_values(c, n, l, m);
}

// ---- Keypad callback --------------------------------------
void onKey(char key) {
  // --- NORMAL MODE ---
  if (_editMode == EDIT_NONE) {
    if (key == '#') {
      // Call next token
      if (queue_call_next()) {
        buzzer_beep(80);
        comm_send_call(queue_get_current(), queue_get_next());
        _syncAll();
        display_show_status("CALLED OK");
        Serial.printf("[MAIN] Called token %d\n", queue_get_current());
      } else {
        buzzer_double_beep();
        display_show_status("AT LIMIT!");
      }

    } else if (key == '*') {
      // Reset queue
      queue_reset();
      comm_send_reset();
      _syncAll();
      display_show_status("QUEUE RESET");
      buzzer_double_beep();
      Serial.println(F("[MAIN] Queue reset"));

    } else if (key == 'A') {
      // Enter LIMIT edit mode
      _editMode = EDIT_LIMIT;
      _editLen  = 0;
      memset(_editBuf, 0, sizeof(_editBuf));
      display_show_status("EDIT LIMIT: _");
      Serial.println(F("[MAIN] Edit LIMIT mode"));

    } else if (key == 'B') {
      // Enter CURRENT override mode
      _editMode = EDIT_CURRENT;
      _editLen  = 0;
      memset(_editBuf, 0, sizeof(_editBuf));
      display_show_status("EDIT CURR: _");
      Serial.println(F("[MAIN] Edit CURRENT mode"));
    }

  // --- EDIT MODE ---
  } else {
    if (key >= '0' && key <= '9' && _editLen < 3) {
      _editBuf[_editLen++] = key;
      _editBuf[_editLen]   = '\0';
      char statusMsg[24];
      snprintf(statusMsg, sizeof(statusMsg),
               _editMode == EDIT_LIMIT ? "LIM: %s_" : "CUR: %s_", _editBuf);
      display_show_status(statusMsg);

    } else if (key == '#') {
      // Confirm edit
      int val = atoi(_editBuf);
      if (val >= TOKEN_MIN && val <= TOKEN_MAX) {
        if (_editMode == EDIT_LIMIT) {
          queue_set_limit(val);
          display_show_status("LIMIT SET");
        } else {
          // Override CURRENT — also update NEXT to CURRENT+1
          queue_init(val, val + 1, queue_get_last(), queue_get_limit());
          comm_send_call(queue_get_current(), queue_get_next());
          display_show_status("CURR SET");
        }
        _syncAll();
        buzzer_beep(80);
      } else {
        display_show_status("INVALID!");
        buzzer_double_beep();
      }
      _editMode = EDIT_NONE;

    } else if (key == 'D' || key == '*') {
      // Cancel edit
      _editMode = EDIT_NONE;
      display_show_status("CANCELLED");
    }
  }
}

// ---- Setup ------------------------------------------------
void setup() {
  Serial.begin(115200);
  Serial.println(F("\n=== INNER UNIT STARTING ==="));

  // Load persisted state
  int c, n, l, m;
  storage_init();
  storage_load(&c, &n, &l, &m);

  // Initialize queue with loaded values
  queue_init(c, n, l, m);

  // Initialize hardware modules
  display_init();
  keypad_init();
  comm_init();
  buzzer_init();

  // Register keypad callback
  keypad_set_callback(onKey);

  // Show initial state on TFT
  display_set_values(c, n, l, m);
  display_show_status("READY");

  // Startup beep
  buzzer_beep(100);

  Serial.println(F("=== INNER UNIT READY ===\n"));
  Serial.println(F("Keys: # = Call  * = Reset  A = Edit Limit  B = Edit Current"));
}

// ---- Loop -------------------------------------------------
void loop() {
  keypad_update();   // Scan keys → fires onKey()
  display_update();  // Redraw TFT if dirty
}
