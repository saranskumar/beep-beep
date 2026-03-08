// ============================================================
//  INNER UNIT — QUEUE MODULE (Implementation)
//  Pure logic — no hardware, no serial, no I/O.
// ============================================================
#include "queue.h"
#include "config.h"
#include <Arduino.h>

static int _current = 0;
static int _next    = 0;
static int _last    = 0;
static int _limit   = TOKEN_DEFAULT_LIMIT;

void queue_init(int current, int next, int last, int limit) {
  _current = current;
  _next    = next;
  _last    = last;
  _limit   = limit;
}

bool queue_call_next() {
  if (_next > _limit) {
    Serial.println(F("[QUEUE] At limit — cannot call next"));
    return false;
  }
  _current = _next;
  _next    = _next + 1;
  return true;
}

int queue_issue_token() {
  if (_last >= _limit) {
    Serial.println(F("[QUEUE] LAST at limit — no more tokens today"));
    return -1;
  }
  _last++;
  return _last;
}

void queue_reset() {
  _current = 0;
  _next    = 1;
  _last    = 0;
  // Limit is preserved — intentionally not reset unless explicitly changed
}

void queue_set_limit(int limit) {
  _limit = constrain(limit, TOKEN_MIN, TOKEN_MAX);
}

int queue_get_current() { return _current; }
int queue_get_next()    { return _next;    }
int queue_get_last()    { return _last;    }
int queue_get_limit()   { return _limit;   }
