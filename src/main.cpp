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
    lcd.begin(CS_PIN, WR_PIN, DATA_PIN, BLK_PIN);
    lcd.clear();
    lcd.backlight(200); //Set the Backlight to 200/255
    delay(120);
}

void loop() {
    // -------------------------------------------------------------
    // Test 1: Precision Sweep on Left LCD (25.3 at different precisions)
    // -------------------------------------------------------------
    lcd.printNum(LCD_LEFT, 25.3, 1);   // Should show: "  25.3"
    lcd.printNum(LCD_MID, 123.4, 1);    // Should show: " 123.4"
    lcd.printNum(LCD_RIGHT, 99.9, 1);   // Should show: "   99.9"
    delay(2000);

    lcd.printNum(LCD_LEFT, 25.32, 2);  // Should show: " 25.32"
    lcd.printNum(LCD_MID, 12.34, 2);   // Should show: " 12.34"
    delay(2000);

    lcd.printNum(LCD_LEFT, 25.32, 3);  // Should show: "25.320"
    delay(2000);

    // -------------------------------------------------------------
    // Test 2: Negative Numbers & Integers
    // -------------------------------------------------------------
    lcd.printNum(LCD_LEFT, -5.2, 1);   // Should show: "  -5.2"
    lcd.printNum(LCD_MID, -128, 0);    // Should show: " -128"
    lcd.printNum(LCD_RIGHT, -1234, 0); // Should show: " -1234"
    delay(2000);

    // -------------------------------------------------------------
    // Test 3: Temperature Readings
    // -------------------------------------------------------------
    lcd.printCelsiusLeft(31.7);         // Should show: " 31.7°C"
    lcd.printCelsiusMid(-4.5);          // Should show: " -4.5°C"
    lcd.printCelsiusRight(105.0);       // Should show: "  105°C"
    delay(2000);

    // -------------------------------------------------------------
    // Test 4: Cycle Battery Level (0 to 3) & Triangles
    // -------------------------------------------------------------
    for (int b = 0; b <= 3; b++) {
        lcd.setBatteryLevel(b);
        
        // Alternate triangles
        lcd.setLeftTriangles(b % 2 == 1, b % 2 == 0);
        lcd.setMidTriangles(b % 2 == 0, b % 2 == 1);
        
        delay(800);
    }
}