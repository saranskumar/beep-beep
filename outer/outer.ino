// ============================================================
//  OUTER UNIT — MAIN SKETCH
//  Public-facing ESP32: token display, audio, printer, button.
//
//  This file is the orchestrator only.
//  All hardware logic lives in the module files below.
//
//  Module responsibilities:
//    config.h     — pin definitions & constants
//    comm         — receive CALL/RESET from Inner unit via UART
//    display      — MAX7219 8-panel LED matrix (dual zone)
//    audio        — DFPlayer Mini bilingual announcements
//    printer      — ESC/POS thermal token slip printer
//    button       — debounced customer token request button
// ============================================================

#include "config.h"
#include "comm.h"
#include "display.h"
#include "audio.h"
#include "printer.h"
#include "button.h"

// ---- State ------------------------------------------------
static int _currentToken = 0;   // Token currently being served
static int _nextToken    = 0;   // Next token in queue

// ---- Callbacks --------------------------------------------

// Called when Inner unit sends CALL|current,next
void onTokenCalled(int current, int next) {
  _currentToken = current;
  _nextToken    = next;

  // Update public display
  display_show(current, next);

  // Announce by audio (bilingual)
  audio_play_token(current);
}

// Called when Inner unit sends RESET
void onReset() {
  _currentToken = 0;
  _nextToken    = 0;
  display_clear();
}

// Called when customer presses the token request button
void onTokenRequest() {
  // The Outer unit does NOT manage LAST — that is owned by the Inner unit.
  // Pressing the button here triggers a print of the NEXT available token.
  // The Inner unit must have already been notified via its own button/comm path
  // if multi-counter token issuance is needed.
  //
  // For single-counter systems: the button increments local last and prints.
  // Adjust this callback if the Inner unit is the authority for LAST.

  // Simple approach: print next in the local last+1 sequence
  // (In full deployment, replace with Inner-acknowledged token number.)
  Serial.println(F("[MAIN] Token request button pressed"));
  // Currently print a placeholder — final token number comes from comm
  // printer_print_token(_nextToken);   // Uncomment when comm is wired
}

// ---- Setup ------------------------------------------------
void setup() {
  Serial.begin(115200);
  Serial.println(F("\n=== OUTER UNIT STARTING ==="));

  // Initialize all modules
  comm_init();
  display_init();
  audio_init();
  printer_init();
  button_init();

  // Register callbacks
  comm_set_call_callback(onTokenCalled);
  comm_set_reset_callback(onReset);
  button_set_callback(onTokenRequest);

  // Startup splash
  display_show_text("RDY", "---");
  delay(2000);
  display_clear();

  Serial.println(F("=== OUTER UNIT READY ===\n"));
}

// ---- Loop -------------------------------------------------
void loop() {
  button_update();    // Poll button → fires onTokenRequest if pressed
  comm_update();      // Parse UART input → fires onTokenCalled / onReset
  display_update();   // Drive MD_Parola animation engine
}
