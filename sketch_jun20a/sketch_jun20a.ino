// Test sketch for PN532 on ESP8266
// Used libraries from https://github.com/elechouse/PN532
// Copy PN532 and PN532_I2C folders to the Arduino libraries folder

#include <Wire.h>
#include <PN532_I2C.h>
#include <PN532.h>
PN532_I2C pn532i2c(Wire);
PN532 nfc(pn532i2c);

void setup(void) {
  Serial.begin(115200);
  Serial.println("Sketch started...");

  nfc.begin();

  Serial.println("Scan I2C bus...");
  for (byte address = 1; address <= 127; address++ )
  {
    Wire.beginTransmission(address);
    if (Wire.endTransmission() == 0)
    {
      Serial.println(address, HEX);
    }
  }

  uint32_t versiondata = nfc.getFirmwareVersion();
  if (! versiondata) {
    Serial.println("Didn't find PN53x board");
    Serial.println("Sketch halted!");
    while (1)
      delay(1000);
  }

  // Got ok data, print it out!
  Serial.print("Found chip PN5"); Serial.println((versiondata >> 24) & 0xFF, HEX);
  Serial.print("Firmware ver. "); Serial.print((versiondata >> 16) & 0xFF, DEC);
  Serial.print('.'); Serial.println((versiondata >> 8) & 0xFF, DEC);

  // configure board to read RFID tags
  nfc.SAMConfig();

  Serial.println("Waiting for an ISO14443A Card ...");
}

unsigned long counter=0;

void loop(void) {
  uint8_t success;
  uint8_t uid[] = { 0, 0, 0, 0, 0, 0, 0 };  // Buffer to store the returned UID
  uint8_t uidLength;                        // Length of the UID (4 or 7 bytes depending on ISO14443A card type)

  // Wait for an ISO14443A type cards (Mifare, etc.).  When one is found
  // 'uid' will be populated with the UID, and uidLength will indicate
  // if the uid is 4 bytes (Mifare Classic) or 7 bytes (Mifare Ultralight)
  success = nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, uid, &uidLength);

  if (success) {
    // Display some basic information about the card
    Serial.print(++counter);
    Serial.print(" Found an ISO14443A card with ");
    Serial.print(" UID Length: "); Serial.print(uidLength, DEC); Serial.print(" bytes");
    Serial.print(" and UID Value: ");
    nfc.PrintHex(uid, uidLength);

    delay(250);
  }
}
