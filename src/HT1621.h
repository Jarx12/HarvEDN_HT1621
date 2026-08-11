/*******************************************************************************
Copyright 2016-2018 anxzhu (github.com/anxzhu)
Copyright 2018-2020 Valerio Nappi (github.com/valerionew) (changes)
Based on segment-lcd-with-ht1621 from anxzhu (2016-2018)
(https://github.com/anxzhu/segment-lcd-with-ht1621)

Partially rewritten and extended by Valerio Nappi (github.com/valerionew) in 2018

This file is part of the HT1621 arduino library, and thus under the MIT license.
More info on the project and the license conditions on :
https://github.com/valerionew/ht1621-7-seg

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

#ifndef HT1621_H_
#define HT1621_H_   //Prevent multiple inclusions of this header file

#define  BIAS     0x52             //0b1000 0101 0010  Sets the driver Bias to 1/3 Duty Cycle and 4 COM
#define  SYSDIS   0X00             //0b1000 0000 0000  Turn off system oscillator and LCD bias generator
#define  SYSEN    0X02             //0b1000 0000 0010 Turn on system oscillator
#define  LCDOFF   0X04             //0b1000 0000 0100  Turn off LCD bias
#define  LCDON    0X06             //0b1000 0000 0110  Turn on LCD bias
#define  XTAL     0x28             //0b1000 0010 1000 External clock input (disabled)
#define  RC256    0X30             //0b1000 0011 0000  Internal clock (enabled)
#define  TONEON   0X12             //0b1000 0001 0010  Enable Buzzer output
#define  TONEOFF  0X10             //0b1000 0001 0000 Disable Buzzer output
#define  WDTDIS1  0X0A             //0b1000 0000 1010  Disable Watchdog timer
#define  BUFFERSIZE 16


// #define HT1621_DEBUG

enum LCDSection {
    LCD_RIGHT = 0,
    LCD_MID   = 1,
    LCD_LEFT  = 2
};

class  HT1621
{
public:
	HT1621();
	// Initialization Functions
	void begin(int cs_p, int wr_p, int data_p, int backlight_p);
	void begin(int cs_p, int wr_p, int data_p);
	
	
	// Control Functions
	void display(bool state); // true=on, false=off
	void set_backlight(bool state); // true=on, false=off
	void backlight(uint8_t level); // 0-255, 0=min, 255=max
	void clear();
	
	// General Prints
	void print(const char* str, bool leftPadded = false);
	void print(long num, const char* flags="%6li", int precision = 0);
	void print(double num, int precision = 3);
	void printCelsius(double num); // precision is always 1
	void setBatteryLevel(int level);
	void setMidTriangles(bool top, bool down); //Control the triangle indicators on the middle display. 
	void setLeftTriangles(bool top, bool down);
	// Dedicated display printing methods
    //Left
	void printLeft(long num);
    void printLeft(const char* str);
	//Mid
    void printMid(long num);
    void printMid(const char* str);
	//Right
    void printRight(long num);
    void printRight(const char* str);
	
	void setDecimalSeparator(LCDSection section, int dpposition);

	void setDecimalRight(int dpposition) { setDecimalSeparator(LCD_RIGHT, dpposition); }
    void setDecimalMid(int dpposition)   { setDecimalSeparator(LCD_MID, dpposition); }
    void setDecimalLeft(int dpposition)  { setDecimalSeparator(LCD_LEFT, dpposition); }



private:
	//Pins and Variables
	int _cs_p;
	int _wr_p;
	int _data_p;
	int _backlight_p;
	bool _backlight_en;
	char _buffer[BUFFERSIZE];
	unsigned char _battery[3];
	// Initialization Functions
	void wrone(unsigned char addr, unsigned char sdata);
	void wrclrdata(unsigned char addr, unsigned char sdata);
	void wrCLR(unsigned char len);
	void wrDATA(unsigned char data, unsigned char cnt);
	void wrCMD(unsigned char CMD);
	void config();
	//Helper Functions
	void update();
	
	char charToSegBits(char character);
};
#endif
