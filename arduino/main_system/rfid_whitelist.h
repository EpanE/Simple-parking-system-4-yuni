// RFID UID whitelist — hardcoded list of cards allowed through the gate.
//
// To find a card's UID: flash arduino/tests/test_rc522/test_rc522.ino,
// open Serial Monitor at 115200 baud, scan the card, copy the printed
// "Card UID: ..." value (uppercase hex, no spaces) into ALLOWED_UIDS below.

#pragma once

const char* ALLOWED_UIDS[] = {
  "04A1B2C3",   // placeholder — replace with a real scanned UID
  "1A2B3C4D",   // placeholder — add more rows as needed
};
const int NUM_ALLOWED_UIDS = sizeof(ALLOWED_UIDS) / sizeof(ALLOWED_UIDS[0]);

bool isWhitelisted(const String& uid) {
  for (int i = 0; i < NUM_ALLOWED_UIDS; i++) {
    if (uid.equalsIgnoreCase(ALLOWED_UIDS[i])) return true;
  }
  return false;
}
