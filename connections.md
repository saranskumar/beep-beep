# Token Queue Management System — Hardware Connections & Flashing Guide

## 1. Outer Unit (Public ESP32) Connections

### 1.1 MAX7219 LED Matrix (Two 4-panel chains)
Both matrices share power, DIN, and CLK, but have separate CS pins.
- **VCC** → 5V
- **GND** → GND
- **DIN (Both)** → GPIO 23 (VSPI MOSI)
- **CLK (Both)** → GPIO 18 (VSPI SCK)
- **CS (NEXT Display)**    → GPIO 5  (`MATRIX_CS_NEXT`)
- **CS (CURRENT Display)** → GPIO 4  (`MATRIX_CS_CURR`)

### 1.2 DFPlayer Mini (Audio)
*(Note: It is highly recommended to put a 1kΩ resistor between ESP32 TX and DFPlayer RX to reduce noise and protect the chip)*
- **VCC** → 5V
- **GND** → GND
- **RX**  → GPIO 17 (`DFPLAYER_TX_PIN` - ESP32 transmits here)
- **TX**  → GPIO 16 (`DFPLAYER_RX_PIN` - ESP32 receives here)
- **SPK1 & SPK2** → Direct to Speaker
- *SD Card should be formatted as FAT32, with `/01/` and `/02/` folders.*

### 1.3 Thermal Printer (TTL Serial)
- **VCC** → 5V / 9V (Depending on Printer specs. Needs a strong power supply!)
- **GND** → GND
- **RX**  → GPIO 12 (`PRINTER_TX_PIN`)
- **TX**  → GPIO 13 (`PRINTER_RX_PIN`)

### 1.4 Token Request Button
- **Pin 1** → GPIO 0 (`BTN_PIN`)
- **Pin 2** → GND
*(Internal pull-up is enabled in code. The button triggers when pulled to GND).*

---

## 2. Inner Unit (Operator ESP32) Connections

### 2.1 ST7735 TFT Display
- **VCC** → 3.3V or 5V (Check your display's regulator)
- **GND** → GND
- **CS**  → GPIO 15 (`TFT_CS_PIN`)
- **RST / RES** → GPIO 4  (`TFT_RST_PIN`)
- **DC / RS** → GPIO 2  (`TFT_DC_PIN`)
- **SDA / DIN** → GPIO 23 (VSPI MOSI)
- **SCK / CLK** → GPIO 18 (VSPI SCK)
- **LED / BL** → 3.3V (Backlight)

### 2.2 4x4 Matrix Keypad
- **Row 1** → GPIO 32  (`KP_ROW0`)
- **Row 2** → GPIO 33  (`KP_ROW1`)
- **Row 3** → GPIO 25  (`KP_ROW2`)
- **Row 4** → GPIO 26  (`KP_ROW3`)
- **Col 1** → GPIO 27  (`KP_COL0`)
- **Col 2** → GPIO 14  (`KP_COL1`)
- **Col 3** → GPIO 12  (`KP_COL2`)
- **Col 4** → GPIO 13  (`KP_COL3`)

### 2.3 Piezo Buzzer
- **Positive / Signal** → GPIO 16 (`BUZZER_PIN`)
- **Negative / GND**    → GND

---

## 3. Flashing & Pairing Guide (ESP-NOW)

Because the system uses ESP-NOW, the devices communicate peer-to-peer using their MAC addresses without needing a WiFi router. You must pair them in the correct sequence.

### Step 1: Flash the Outer Unit
1. Open `d:\WORK\beep\main\outer\outer.ino` in Arduino IDE.
2. Connect the **Outer ESP32** to your PC via USB.
3. Select the correct ESP32 board and COM port.
4. Click **Upload**.
5. Once uploaded, open the Arduino **Serial Monitor** and set baud rate to **115200**.
6. Note the MAC address printed:
   ```
   === OUTER UNIT STARTING ===
   [COMM] Outer MAC: AA:BB:CC:DD:EE:FF
   [COMM] ESP-NOW receiver ready
   ...
   ```
7. **Copy that MAC address!** (e.g., `AA:BB:CC:DD:EE:FF`)

### Step 2: Configure the Inner Unit
1. Open `d:\WORK\beep\main\inner\config.h`.
2. Locate the line that says `#define OUTER_MAC ...` at the bottom (line 39).
3. Replace the placeholder with the MAC address you copied from the Outer unit.
   *Note: Format it with `0x` prefixes, replacing the colons with commas.*
   *Example: If MAC was `AA:BB:CC:DD:EE:FF`, it should become:*
   `#define OUTER_MAC {0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF}`
4. Save the file.

### Step 3: Flash the Inner Unit
1. Open `d:\WORK\beep\main\inner\inner.ino` in Arduino IDE.
2. Connect the **Inner ESP32** via USB.
3. Select the correct ESP32 board and COM port.
4. Click **Upload**.

### Step 4: Verify Communication
1. Provide power to both units.
2. Press `#` on the Inner unit keypad.
3. The Inner unit TFT should display `CALLED OK`.
4. The Outer unit's LED Matrix should update, the audio should play the new token, and the printer should be ready to print when the outer button is pressed.
