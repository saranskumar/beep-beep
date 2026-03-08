// ============================================================
//  OUTER UNIT — PRINTER MODULE (Implementation)
//  ESC/POS compatible thermal printer via HardwareSerial2.
// ============================================================
#include "printer.h"
#include "config.h"
#include <Arduino.h>

// Use ESP32 Hardware Serial 2 for the printer
#define PrinterSerial Serial2

// ESC/POS command helpers
#define ESC_INIT        "\x1B\x40"          // Initialize printer
#define ESC_CENTER      "\x1B\x61\x01"      // Center align
#define ESC_LEFT        "\x1B\x61\x00"      // Left align
#define ESC_BOLD_ON     "\x1B\x45\x01"      // Bold on
#define ESC_BOLD_OFF    "\x1B\x45\x00"      // Bold off
#define ESC_DOUBLE_ON   "\x1D\x21\x11"      // Double width+height
#define ESC_DOUBLE_OFF  "\x1D\x21\x00"      // Normal size
#define ESC_FEED_CUT    "\x1D\x56\x41\x10"  // Feed and partial cut

static void _print(const char* s) { PrinterSerial.print(s); }
static void _println(const char* s) { PrinterSerial.println(s); }

void printer_init() {
  PrinterSerial.begin(PRINTER_BAUD, SERIAL_8N1, PRINTER_RX_PIN, PRINTER_TX_PIN);
  delay(500);                     // Give printer time to wake up
  _print(ESC_INIT);               // Reset printer state
  Serial.println(F("[PRINTER] Thermal printer initialized"));
}

void printer_print_token(int tokenNumber) {
  Serial.printf("[PRINTER] Printing token %d\n", tokenNumber);

  char numStr[8];
  snprintf(numStr, sizeof(numStr), "%03d", tokenNumber);  // e.g. "034"

  _print(ESC_INIT);

  // Header
  _print(ESC_CENTER);
  _print(ESC_BOLD_ON);
  _println("----------------------");
  _println("   SERVICE TOKEN");
  _println("----------------------");
  _print(ESC_BOLD_OFF);

  // Token number (large)
  _print(ESC_CENTER);
  _print(ESC_DOUBLE_ON);
  PrinterSerial.printf("TOKEN : %s\n", numStr);
  _print(ESC_DOUBLE_OFF);

  // Footer
  _print(ESC_CENTER);
  _println("");
  _println("Please wait for");
  _println("your turn.");
  _println("----------------------");
  _println("");
  _println("");

  // Feed and cut
  _print(ESC_FEED_CUT);

  Serial.println(F("[PRINTER] Print complete"));
}
