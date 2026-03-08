// ============================================================
//  OUTER UNIT — AUDIO MODULE (Implementation)
//  Bilingual token announcements via DFPlayer Mini.
//  Ported and modularized from test/token.ino
// ============================================================
#include "audio.h"
#include "config.h"
#include <SoftwareSerial.h>
#include <DFRobotDFPlayerMini.h>

// SD card folder constants
#define FOLDER_EN      1     // English folder /01/
#define FOLDER_ML      2     // Malayalam folder /02/
#define FILE_ALERT     1001  // alert / chime
#define FILE_TOKEN_PFX 1002  // "Your token number is" / "ടോക്കൺ നമ്പർ"

// Playback timing (ms) — adjust to match your audio file lengths
#define DELAY_ALERT    2500
#define DELAY_PHRASE   2000
#define DELAY_NUMBER   2000
#define DELAY_PAUSE     500

static SoftwareSerial     _ss(DFPLAYER_RX_PIN, DFPLAYER_TX_PIN);
static DFRobotDFPlayerMini _player;

void audio_init() {
  _ss.begin(9600);
  Serial.println(F("[AUDIO] Initializing DFPlayer Mini..."));

  if (!_player.begin(_ss)) {
    Serial.println(F("[AUDIO] ERROR: DFPlayer not found. Check wiring and SD card."));
    while (true) { delay(1000); }  // Halt — cannot proceed without audio
  }

  _player.outputDevice(DFPLAYER_DEVICE_SD);
  _player.volume(DFPLAYER_VOLUME);
  Serial.println(F("[AUDIO] DFPlayer online"));
}

void audio_play_token(int tokenNumber) {
  if (tokenNumber < 1 || tokenNumber > 999) {
    Serial.printf("[AUDIO] Invalid token number: %d\n", tokenNumber);
    return;
  }
  Serial.printf("[AUDIO] Announcing token %d\n", tokenNumber);

  // --- Alert chime ---
  _player.playFolder(FOLDER_EN, FILE_ALERT);
  delay(DELAY_ALERT);

  // --- English announcement ---
  _player.playFolder(FOLDER_EN, FILE_TOKEN_PFX);  // "Your token number is"
  delay(DELAY_PHRASE);
  _player.playFolder(FOLDER_EN, tokenNumber);      // e.g. "forty two"
  delay(DELAY_NUMBER);

  delay(DELAY_PAUSE);

  // --- Malayalam announcement ---
  _player.playFolder(FOLDER_ML, FILE_TOKEN_PFX);  // "നിങ്ങളുടെ ടോക്കൺ നമ്പർ"
  delay(DELAY_PHRASE);
  _player.playFolder(FOLDER_ML, tokenNumber);      // e.g. number in Malayalam
  delay(DELAY_NUMBER);

  Serial.println(F("[AUDIO] Announcement complete"));
}

void audio_play_alert() {
  _player.playFolder(FOLDER_EN, FILE_ALERT);
  delay(DELAY_ALERT);
}

void audio_set_volume(int volume) {
  volume = constrain(volume, 0, 30);
  _player.volume(volume);
  Serial.printf("[AUDIO] Volume set to %d\n", volume);
}

bool audio_is_playing() {
  return (_player.readState() == 1);
}
