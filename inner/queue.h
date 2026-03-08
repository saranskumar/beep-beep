#pragma once

// ============================================================
//  INNER UNIT — QUEUE MODULE (Header)
//  All queue business logic — no hardware access here.
//
//  State variables:
//    current  — token currently being served
//    next     — next token to be called
//    last     — last token issued to a customer
//    limit    — maximum token allowed for the day
// ============================================================

// Initialize queue with loaded state (call after storage_load)
void queue_init(int current, int next, int last, int limit);

// Advance the queue: CURRENT = NEXT, NEXT++
// Returns false if NEXT would exceed LIMIT (queue full), true on success
bool queue_call_next();

// Issue a new token to a customer: LAST++
// Returns the new LAST value, or -1 if limit reached
int queue_issue_token();

// Reset all counters to zero / initial state
void queue_reset();

// Set a new daily limit
void queue_set_limit(int limit);

// Getters
int queue_get_current();
int queue_get_next();
int queue_get_last();
int queue_get_limit();
