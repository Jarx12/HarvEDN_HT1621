#include <Arduino.h>
#include "HT1621.h"

HT1621 lcd;

const uint8_t CS_PIN   = 7;
const uint8_t WR_PIN   = 5;
const uint8_t DATA_PIN = 4;
const uint8_t BLK_PIN  = 8; 


uint8_t current_buf = 0;
uint8_t current_bit = 0;

void setup() {
    Serial.begin(115200);
    
    // 1. Initialize LCD pins
    lcd.begin(CS_PIN, WR_PIN, DATA_PIN, BLK_PIN);
    lcd.backlight(200);
    // 2. Wipe hardware and local buffer
    lcd.clear();
    delay(500);

    Serial.println("--- Starting HT1621 Display Test ---");

    // -------------------------------------------------------------
    // TEST 1: Section Specific Printing
    // -------------------------------------------------------------
    Serial.println("Testing Left (5-digit), Mid (5-digit), Right (6-digit)...");
    lcd.printCelsiusLeft(25.4); // Left Display with 1 decimal place
    lcd.printCelsiusMid(37.1);   // Mid Display with 1 decimal place
    lcd.printCelsiusRight(32); // Right Display with 1 decimal place
    delay(500);
    // Clear screen before starting loop counter
    //lcd.clear();
}

void loop() {

}