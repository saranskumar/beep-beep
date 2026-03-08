#pragma once

// ============================================================
//  OUTER UNIT — AUDIO MODULE (Header)
//  Drives DFPlayer Mini for bilingual token announcements.
//  Uses a robust, non-blocking queue via millis() timers.
//
//  SD Card folder structure (Strictly 1 folder named 'mp3'):
//    /mp3/
//      0001.mp3  — Alert / chime
//      0002.mp3  — "Your token number is" (English)
//      0003.mp3  — Malayalam token prefix
//      0004.mp3  — Number 1
//      0005.mp3  — Number 2
//      ...
//      X.mp3     — Number (X - 3)
// ============================================================

// Initialize DFPlayer Mini
void audio_init();

// Called every loop() to process the audio queue without blocking
void audio_update();

// Queue a full bilingual announcement
void audio_play_token(int tokenNumber);

// Queue an attention/alert chime
void audio_play_alert();

// Set playback volume (0–30)
void audio_set_volume(int volume);

// Returns true if audio is actively playing or queued
bool audio_is_playing();
