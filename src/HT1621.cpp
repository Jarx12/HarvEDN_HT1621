/*******************************************************************************
Copyright 2016-2018 anxzhu (github.com/anxzhu)
Copyright 2018-2020 Valerio Nappi (github.com/5N44P) (changes)
Based on segment-lcd-with-ht1621 from anxzhu (2016-2018)
(https://github.com/anxzhu/segment-lcd-with-ht1621)

Partially rewritten and extended by Valerio Nappi (github.com/5N44P) in 2018

This file is part of the HT1621 arduino library, and thus under the MIT license.
More info on the project and the license conditions on :
https://github.com/5N44P/ht1621-7-seg

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
of the Software, and to permit persons to whom the Software is furnished to do
so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

*******************************************************************************/

#include <Arduino.h>
#include "HT1621.h"

//Constructor
HT1621::HT1621() 
{
    memset(_buffer, 0x00, sizeof(_buffer));
}

// Initialization Functions
void HT1621::begin(int cs_p, int wr_p, int data_p, int backlight_p)
{
	pinMode(cs_p, OUTPUT);
	pinMode(wr_p, OUTPUT);
	pinMode(data_p, OUTPUT);
	pinMode(backlight_p, OUTPUT);
	_cs_p=cs_p;
	_wr_p=wr_p;
	_data_p=data_p;
	_backlight_p=backlight_p;
	_backlight_en=true;
	config();
}

void HT1621::begin(int cs_p, int wr_p, int data_p)
{
	pinMode(cs_p, OUTPUT);
	pinMode(wr_p, OUTPUT);
	pinMode(data_p, OUTPUT);
	_cs_p=cs_p;
	_wr_p=wr_p;
	_data_p=data_p;
	_backlight_en = false;
	config();
}

void HT1621::config()
{
	wrCMD(BIAS);
	wrCMD(RC256);
	wrCMD(SYSDIS);
	wrCMD(WDTDIS1);
	wrCMD(SYSEN);
	wrCMD(LCDON);
}


void HT1621::wrDATA(unsigned char data, unsigned char cnt) {
	unsigned char i;
	for (i = 0; i < cnt; i++) {
		digitalWrite(_wr_p, LOW);
		delayMicroseconds(4);
		if (data & 0x80) {
			digitalWrite(_data_p, HIGH);
		}
		else {
			digitalWrite(_data_p, LOW);
		}
		digitalWrite(_wr_p, HIGH);
		delayMicroseconds(4);
		data <<= 1;
	}
}
// Write a command to the HT1621
void HT1621::wrCMD(unsigned char CMD) {  //100
	digitalWrite(_cs_p, LOW);
	wrDATA(0x80, 4);
	wrDATA(CMD, 8);
	digitalWrite(_cs_p, HIGH);
}
//
void HT1621::wrclrdata(unsigned char addr, unsigned char sdata)
{
	addr <<= 2;
	digitalWrite(_cs_p, LOW);
	wrDATA(0xa0, 3);
	wrDATA(addr, 6);
	wrDATA(sdata, 8);
	digitalWrite(_cs_p, HIGH);
}
void HT1621::wrCLR(unsigned char len) {
	unsigned char addr = 0;
	unsigned char i;
	for (i = 0; i < len; i++) {
		wrclrdata(addr, 0x00);
		addr = addr + 2;
	}
}
//Write One Byte to the HT1621
void HT1621::wrone(unsigned char addr, unsigned char sdata)
{
	addr <<= 2;
	digitalWrite(_cs_p, LOW);
	wrDATA(0xa0, 3);
	wrDATA(addr, 6);
	wrDATA(sdata, 8);
	digitalWrite(_cs_p, HIGH);
}

// Control functions
//Set the display on or off
void HT1621::display(bool state)
{
	if (state) {
		wrCMD(LCDON);
	} else {
		wrCMD(LCDOFF);
	}
}
// Set the backlight on or off
void HT1621::set_backlight(bool state) {
    backlight(state ? (uint8_t)255 : (uint8_t)0);
}
// Set the backlight brightness level (0-255)
void HT1621::backlight(uint8_t level) {
    if (_backlight_en) {
        // Simple quadratic gamma curve for perceptually linear dimming
        uint16_t pwm_scaled = ((uint16_t)level * level) / 255; 
        analogWrite(_backlight_p, (uint8_t)pwm_scaled);
    }
}
//Clear the display and the buffer
void HT1621::clear() {
    // 1. Clear the hardware display memory (16 addresses: 0x00 through 0x1E)
    wrCLR(16);
    // 2. Clear the local display buffer
    for (uint8_t i = 0; i < BUFFERSIZE; i++) {
        _buffer[i] = 0x00;
    }
}

// ==========================================
//General Prints
// ==========================================

/*void HT1621::print(long num, const char* flags, int precision){
	if(num > 999999) // basic checks
		num = 999999; // clip into 999999
	if(num < -99999) // basic checks
		num = -99999; // clip into -99999

	char localbuffer[7]; //buffer to work within the function
	snprintf(localbuffer, 7, flags, num); // convert the decimal into string
	#ifdef _HTDEBUG
		Serial.begin(9600);
		Serial.print(localbuffer);
		Serial.print("\t");
	#endif

	// horrible handling but should get us working. needs refactor in next major
	if (precision > 0 && (num) < pow(10, precision)) {
		// we remove extra leading zeros
		for (int i = 0; i < (5 - precision); i++) {
			#ifdef _HTDEBUG
				Serial.print(localbuffer[1]);
			#endif // _HTDEBUG
			if(localbuffer[i+1] == '0' && localbuffer[i] != '-'){ // we remove only if there is another zero ahead AND if it's not a minus sign
				localbuffer[i] = ' ';
			}
			else{
				break;
			} 
			#ifdef _HTDEBUG
				Serial.println();buffer[1]);
			#endif // _HTDEBUG
	}
	}


	for(int i=0; i<6; i++){
		_buffer[i] &= 0x80; // mask the first bit, used by batter and decimal point
		_buffer[i] |= charToSegBits(localbuffer[i]);
	}
	update();
}

void HT1621::print(double num, int precision){
	if(num > 999999) // basic checks
		num = 999999; // clip into 999999
	if(num < -99999) // basic checks
		num = -99999; // clip into -99999

	if(precision > 3 && num > 0)
		precision = 3; // if positive max precision allowed = 3
	else if(precision > 2 && num < 0)
		precision = 2;// if negative max precision allowed = 2
	if(precision < 0)
		precision = 0; // negative precision?!

	const char* flags = (precision > 0 && abs(num) < 1) ? "%06li" : "%6li";

	long integerpart;
	integerpart = ((long)(num*pow(10,precision)));

	print(integerpart, flags, precision); // draw the integerized number
	setDecimalRight(precision); // draw the decimal point

	update();
}


void HT1621::print(const char* str, bool leftPadded){
	int chars = strlen(str);
	int padding = 6 - chars;

	for(int i = 0; i < 6; i++){
		_buffer[i] &= 0x80; // mask the first bit, used by batter and decimal point
		char character = leftPadded
				 		 ? i < padding ? ' ' : str[i - padding]
				 		 : i >= chars ? ' ' : str[i];
		_buffer[i] |= charToSegBits(character);
	}

	setDecimalRight(0); // Hide decimal point
	update();
}*/

//Sets the battery level on the display. 0=off, 1=1 segment, 2=2 segments, 3=3 segments
void HT1621::setBatteryLevel(int level) 
{
    // Clear Bit 7 (0x80) on the 3 battery buffer positions
    _buffer[3] &= 0x7F; // Battery bar 1
    _buffer[4] &= 0x7F; // Battery bar 2
    _buffer[5] &= 0x7F; // Battery bar 3

    switch(level) {
        case 3: // Battery on + all 3 segments
            _buffer[5] |= 0x80;
        case 2: // Battery on + 2 segments
            _buffer[4] |= 0x80;
        case 1: // Battery on + 1 segment
            _buffer[3] |= 0x80;
        case 0: // Battery off
        default:
            break;
    }
    update();
}

void HT1621::setMidTriangles(bool up, bool down) {
    // Clear Bit 7 (0x80) on Middle Triangle buffer positions
    _buffer[9] &= 0x7F; // Middle Triangle UP (RAM 0x11)
    _buffer[10] &= 0x7F; // Middle Triangle DOWN (RAM 0x13)

    if (up)   _buffer[9] |= 0x80;
    if (down) _buffer[10] |= 0x80;

    update();
}

void HT1621::setLeftTriangles(bool up, bool down) {
    // Clear Bit 7 (0x80) on Left Triangle buffer positions
    _buffer[14] &= 0x7F; // Left Triangle UP (RAM 0x1D)
    _buffer[15] &= 0x7F; // Left Triangle DOWN (RAM 0x1F)

    if (up)   _buffer[14] |= 0x80;
    if (down) _buffer[15] |= 0x80;

    update();
}

void HT1621::setDecimalSeparator(LCDSection section, int decimaldigits) {
    // 3 Decimal Point buffer slots for each LCD section
    static const uint8_t dp_map[3][3] = {
        { 0,  1,  2 },   // LCD_RIGHT
        { 6,  7, 8 },   // LCD_MID
        { 11, 12, 13 }   // LCD_LEFT
    };

    if (section < LCD_RIGHT || section > LCD_LEFT) return;

    // Clear Bit 7 (0x80) on all 3 DP positions for the target LCD
    _buffer[dp_map[section][0]] &= 0x7F;
    _buffer[dp_map[section][1]] &= 0x7F;
    _buffer[dp_map[section][2]] &= 0x7F;

    // Set Bit 7 for the chosen decimal position (1, 2, or 3)
    if (decimaldigits >= 1 && decimaldigits <= 3) {
        _buffer[dp_map[section][3 - decimaldigits]] |= 0x80;
    }
    update();
}

//Prints a number in celsius with max 1 decimal point, will truncate the number if it is too big to fit in the display. so the effective range is -99 to 999, the right display is used as if it had 5 digits when it has 6, so could be improved
void HT1621::printCelsius(LCDSection section, double num) {
    // Clamp range to prevent string overflow
    if (num > 999.0)  num = 999.0;
    if (num < -99.0)  num = -99.0;

    // Use 1 decimal place if between -9.9 and 99.9, otherwise 0
    int precision = (num >= -9.9 && num < 100.0) ? 1 : 0;

    int32_t val;
    if (precision == 1) {
        double scaled = num * 10.0;
        val = (scaled >= 0) ? (int32_t)(scaled + 0.5) : (int32_t)(scaled - 0.5);
    } else {
        val = (num >= 0) ? (int32_t)(num + 0.5) : (int32_t)(num - 0.5);
    }

    char str[8];
    if (section == LCD_RIGHT) {
        // 6-Digit LCD: " 317*C" (1 leading space shifts digits onto DP-capable buffer slots)
        snprintf(str, sizeof(str), "%4ld*C", (long)val);
    } else {
        // 5-Digit LCDs (Left & Mid): "317*C"
        snprintf(str, sizeof(str), "%3ld*C", (long)val);
    }

    // Write formatted string to screen
    switch (section) {
        case LCD_LEFT:  printLeft(str);  break;
        case LCD_MID:   printMid(str);   break;
        case LCD_RIGHT: printRight(str); break;
    }

    // Turn on the 1st decimal point for 1-decimal numbers, or clear it
    if (precision == 1) {
        setDecimalSeparator(section, 1);
    } else {
        setDecimalSeparator(section, 0);
    }
}
// Dedicated display printing methods
// ==========================================
// LEFT DISPLAY (5 Digits)
// ==========================================
void HT1621::printLeft(long num) {
    char buf[6];
    snprintf(buf, sizeof(buf), "%5ld", num); // Right-aligned 5 digits
    printLeft(buf);
}

// Update Leftmost Display (5 Digits)
void HT1621::printLeft(const char* str) {
    static const uint8_t left_map[5] = { 15, 14, 13, 12, 11 };
    int len = strlen(str);

    for (int i = 0; i < 5; i++) {
        char c = (i < len) ? str[i] : ' ';
        uint8_t segs = charToSegBits(c);
        _buffer[left_map[i]] = (_buffer[left_map[i]] & 0x80) | (segs & 0x7F);
    }
    update();
}

// ==========================================
// MIDDLE DISPLAY (5 Digits)
// ==========================================
void HT1621::printMid(long num) {
    char buf[6];
    snprintf(buf, sizeof(buf), "%5ld", num); // Right-aligned 5 digits
    printMid(buf);
}

void HT1621::printMid(const char* str) {
    static const uint8_t mid_map[5] = { 10, 9, 8, 7, 6 };
    int len = strlen(str);

    for (int i = 0; i < 5; i++) {
        char c = (i < len) ? str[i] : ' ';
        uint8_t segs = charToSegBits(c);
        _buffer[mid_map[i]] = (_buffer[mid_map[i]] & 0x80) | (segs & 0x7F);
    }
    update();
}

// ==========================================
// RIGHT DISPLAY (6 Digits)
// ==========================================
void HT1621::printRight(long num) {
    char buf[7];
    snprintf(buf, sizeof(buf), "%6ld", num); // Right-aligned 6 digits
    printRight(buf);
}

void HT1621::printRight(const char* str) {
    static const uint8_t right_map[6] = { 5, 4, 3, 2, 1, 0 };
    int len = strlen(str);

    for (int i = 0; i < 6; i++) {
        char c = (i < len) ? str[i] : ' ';
        uint8_t segs = charToSegBits(c);
        _buffer[right_map[i]] = (_buffer[right_map[i]] & 0x80) | (segs & 0x7F);
    }
    update();
}

void HT1621::printNum(LCDSection section, double num, int precision) {
    // 1. Clamp precision to valid range (0 to 3 decimal places)
    if (precision < 0) precision = 0;
    if (precision > 3) precision = 3;

    // Standard multipliers (fast integer math, no pow() bloat)
    static const int32_t mult[] = { 1, 10, 100, 1000 };

    // 2. Scale number to integer with rounding
    double scaled = num * mult[precision];
    int32_t val = (scaled >= 0) ? (int32_t)(scaled + 0.5) : (int32_t)(scaled - 0.5);

    // 3. Determine digit count and clamp limits based on display capacity
    int maxDigits = (section == LCD_RIGHT) ? 6 : 5;
    int32_t maxVal = (maxDigits == 6) ? 999999 : 99999;
    int32_t minVal = (maxDigits == 6) ? -99999 : -9999;

    if (val > maxVal) val = maxVal;
    if (val < minVal) val = minVal;

    // 4. Format integer with right alignment into local string buffer
    char str[7];
    if (maxDigits == 6) {
        snprintf(str, sizeof(str), "%6ld", (long)val);
    } else {
        snprintf(str, sizeof(str), "%5ld", (long)val);
    }

    // 5. Send formatted digits to the selected LCD section
    switch (section) {
        case LCD_LEFT:  printLeft(str);  break;
        case LCD_MID:   printMid(str);   break;
        case LCD_RIGHT: printRight(str); break;
    }

    // 6. Draw decimal separator
    setDecimalSeparator(section, precision);
}


//Helper Functions
//Takes the buffer and puts it straight into the driver
void HT1621::update() {
    for (uint8_t i = 0; i < BUFFERSIZE; i++) {
        wrone(i * 2, _buffer[i]);
    }
}

char HT1621::charToSegBits(char character) {
	switch (character) {
	case '*': // For degree for now
	case '°': 
		return 0b0110011;
	case '|':
		return 0b0000101;
	case '-':
		return 0b0000010;
	case '_':
		return 0b0001000;
	case '0':
		return 0b1111101;
	case '1':
		return 0b1100000;
	case '2':
		return 0b111110;
	case '3':
		return 0b1111010;
	case '4':
		return 0b1100011;
	case '5':
		return 0b1011011;
	case '6':
		return 0b1011111;
	case '7':
		return 0b1110000;
	case '8':
		return 0b1111111;
	case '9':
		return 0b1111011;
	case 'A':
	case 'a':
		return 0b1110111;
	case 'b':
	case 'B':
		return 0b1001111;
	case 'c':
		return 0b01101001;
	case 'C':
		return 0b0011101;
	case 'd':
	case 'D':
		return 0b1101110;
	case 'e':
	//	return 0b0001110;
	case 'E':
		return 0b0011111;
	case 'f':
	//	return 0b0000111;
	case 'F':
		return 0b0010111;
	case 'G':
	case 'g':
		return 0b1011101;
	case 'h':
	//	return 0b1000111;
	case 'H':
		return 0b1100111;
	case 'i':
	//	return 0b1000000;
	case 'I':
		return 0b1100000;
	case 'J':
	case 'j':
		return 0b1101000;
	case 'l':
	//	return 0b1100000;
	case 'L':
		return 0b0001101;
	case 'm':
	case 'M':
		return 0b1010100;
	case 'n':
	case 'N':
		return 0b1000110;
	case 'O': // we can keep this for zero
	//	return 0b1111101;
	case 'o':
		return 0b1001110;
	case 'P':
	case 'p':
		return 0b0110111;
	case 'q':
	case 'Q':
		return 0b1110011;
	case 'r':
	case 'R':
		return 0b0000110;
	case 'S':
	case 's':
		return 0b1011011;
	case 't':
	case 'T':
		return 0b0001111;
	case 'u':
	//	return 0b1001100;
	case 'U':
		return 0b1101101;
	case 'Y':
	case 'y':
		return 0b1101011;
	case 'z':
	case 'Z':
		return 0b0111110;
	case ' ':
	default:
		return 0b0000000;
	}
}