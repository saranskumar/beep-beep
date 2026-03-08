#pragma once

// ============================================================
//  OUTER UNIT — AUDIO MODULE (Header)
//  Drives DFPlayer Mini for bilingual token announcements.
//
//  SD Card folder structure:
//    /01/  — English audio
//      001.mp3 … 999.mp3  — number pronunciations
//      1001.mp3            — alert / attention chime
//      1002.mp3            — "Your token number is"
//    /02/  — Malayalam audio
//      001.mp3 … 999.mp3  — number pronunciations
//      1001.mp3            — alert ("ദയവായി കാത്തിരിക്കുക")
//      1002.mp3            — "നിങ്ങളുടെ ടോക്കൺ നമ്പർ"
// ============================================================

// Initialize DFPlayer Mini (halts with error if SD card missing)
void audio_init();

// Play full bilingual announcement:
//   alert → English phrase + number → Malayalam phrase + number
void audio_play_token(int tokenNumber);

// Play attention/alert chime only
void audio_play_alert();

// Set playback volume (0–30)
void audio_set_volume(int volume);

// Returns true if audio is currently playing
bool audio_is_playing();
