// ============================================================
//  OUTER UNIT — AUDIO MODULE (Implementation)
//  Uses a ring buffer queue and millis() timers since the
//  ACK-disabled DFPlayer cannot reliably report readState().
// ============================================================
#include "audio.h"
#include "config.h"
#include <HardwareSerial.h>
#include <DFRobotDFPlayerMini.h>

#define FILE_ALERT          4001
#define FILE_TOKEN_EN       4002
#define FILE_TOKEN_ML       5002

// Number logic offsets:
// English "1" is file 1001.mp3 -> offset is 1000
// Malayalam "1" is file 2001.mp3 -> offset is 2000
#define FILE_NUM_OFFSET_EN  1000
#define FILE_NUM_OFFSET_ML  2000

#define AUDIO_QUEUE_SIZE 20

static HardwareSerial     _ss(1);
static DFRobotDFPlayerMini _player;

// Queue state
static int _queue[AUDIO_QUEUE_SIZE];
static int _head = 0;
static int _tail = 0;

// Playback timing state
static bool _isPlaying = false;
static unsigned long _trackStartTime = 0;
static unsigned long _trackDurationMs = 0;
static unsigned long _lastPlayTime = 0;
static const unsigned long MIN_GAP_MS = 200; // Small silence between files

// Returns expected duration of a file based on its type
// Tweak these values if files get cut off or have too much trailing silence
static unsigned long get_file_duration(int fileNum) {
  if (fileNum == FILE_ALERT) return 3000;    // Alert duration was 2.5s, bumped to 3s
  if (fileNum == FILE_TOKEN_EN) return 2500; // English phrase was 2s, bumped to 2.5s
  if (fileNum == FILE_TOKEN_ML) return 3000; // Malayalam phrase was 2.5s, bumped to 3s
  return 2000; // Estimated duration for numbers was 1.5s, bumped to 2s
}

void audio_init() {
  pinMode(DFPLAYER_BUSY_PIN, INPUT_PULLUP);

  _ss.begin(9600, SERIAL_8N1, DFPLAYER_RX_PIN, DFPLAYER_TX_PIN);
  Serial.println(F("[AUDIO] Initializing DFPlayer Mini..."));

  // False = disable ACK. Crucial for clone DFPlayer models.
  if (!_player.begin(_ss, false)) {
    Serial.println(F("[AUDIO] ERROR: DFPlayer not found."));
    while (true) { delay(1000); }
  }

  _player.volume(DFPLAYER_VOLUME);
  Serial.println(F("[AUDIO] Initialization complete (Hardware Queue Ready)"));
}

void audio_set_volume(int vol) {
  _player.volume(vol);
}

// Push a file number to the ring buffer queue
static void audio_enqueue(int fileNum) {
  int nextHead = (_head + 1) % AUDIO_QUEUE_SIZE;
  if (nextHead != _tail) {
    _queue[_head] = fileNum;
    _head = nextHead;
  } else {
    Serial.println(F("[AUDIO] ERROR: Queue full! DROPPING AUDIO"));
  }
}

void audio_play_token(int tokenNumber) {
  Serial.printf("[AUDIO] Queueing Token: %d\n", tokenNumber);
  
  audio_enqueue(FILE_ALERT);
  
  // English
  audio_enqueue(FILE_TOKEN_EN);
  audio_enqueue(tokenNumber + FILE_NUM_OFFSET_EN);

  // Malayalam
  audio_enqueue(FILE_TOKEN_ML);
  audio_enqueue(tokenNumber + FILE_NUM_OFFSET_ML);
}

void audio_play_alert() {
  audio_enqueue(FILE_ALERT);
}

bool audio_is_playing() {
  return _isPlaying || (_head != _tail);
}

void audio_update() {
  unsigned long now = millis();

  if (_isPlaying) {
    if (now - _trackStartTime >= _trackDurationMs) {
      _isPlaying = false;
      _lastPlayTime = now; 
    }
  } else {
    if (_head != _tail && (now - _lastPlayTime >= MIN_GAP_MS)) {
      
      int fileNum = _queue[_tail];
      _tail = (_tail + 1) % AUDIO_QUEUE_SIZE;
      
      _player.playMp3Folder(fileNum);
      
      _trackStartTime = now;
      _trackDurationMs = get_file_duration(fileNum);
      _isPlaying = true;

      Serial.printf("[AUDIO] Playing /mp3/%04d.mp3 (Duration: %lu ms)\n", fileNum, _trackDurationMs);
    }
  }
}
