#include <Arduino.h>
#include "HT1621.h"

HT1621 lcd;


const uint8_t CS_PIN   = 7;
const uint8_t WR_PIN   = 5;
const uint8_t DATA_PIN = 4;
const uint8_t BLK_PIN  = 8; 
//const uint8_t TOUCHIO_PIN  = 8; 
/*const int offsetUTC = -4;
bool lastState = HIGH;
uint8_t flag=0;
unsigned long lastmilis = 0;
unsigned long lastmilislcd = 0;
unsigned long lastmilisclock = 0;




void setBLK(int level)
{
  const int brightnessTable[] = {0, 50, 200, 500, 800, 1500, 3000, 4095};
  if (level >= 0 && level <= 7)
  {
    ;  
  } 
  
  //ledcWrite(BLK_PIN, brightnessTable[level]);
}



void apagarsimbolitosmidright() //Apagamos los simbolos de los segmentos del medio y de la derecha del lcd
{
    //lcd.SetmidTriangle(1,0); //Apagar el triangulo TOP
    //lcd.SetmidTriangle(0,0); //Apagar el triangulo BOTTOM
    for (int j=1;j<3;j++)  //Apagamos los puntos decimales
      {
        for (int i=1; i<4;i++)
        {
          //lcd.setDecimal(j,i,0);
        }
      }
    lcd.setBatteryLevel(0); //Apagamos la barra de bateria
}

void utc_to_local_time(uint8_t &hora, int offset)
{
  bool sameday;
  if (offset == 0)
    ;
  else if (offset < 0) //Retroceder
  {
    sameday = (hora >= abs(offset));
    hora = sameday ? (hora - abs(offset)) : (hora + 24 - abs(offset));
  }
  else //Avanzar
  {
    sameday = (hora + abs(offset) < 24);
    hora = sameday ? (hora + abs(offset)) : (hora + abs(offset) - 24);
  }
}


void IOCambiarDatos()
{
  int touch = digitalRead(TOUCHIO_PIN);
  if (touch == HIGH && lastState == LOW) //Si se toca el pin y no se ha dejado pegado en HIGH
  {
    flag++; //Increase Flag
    if (flag > 2)
      flag = 0;
    delay(20); //Debounce
  }
  lastState = touch; //Almacenar el estado para comparar el siguiente ciclo
}


void lcd_showtime()
{
  unsigned long currentTime = millis();
  if (currentTime - lastmilislcd >= 100) //Si han pasado mas de 100 ms
      {
        lastmilislcd = currentTime;
        //;  
        if (flag==0)
        {
          apagarsimbolitosmidright(); //Apagar los simbolos usados por el print_lcd_latlong() 
          //print_lcd_time();
          //print_lcd_date();
        }
        else if (flag==1)
          ;  
          //print_lcd_latlong();
        else
        {
          apagarsimbolitosmidright(); //Apagar los simbolos usados por el print_lcd_latlong() 
          //print_lcd_time();
          //lcd.printRight("RPM 12"); //Presentar en pantalla con 3 decimales
        }          
      }
}*/


void testPanelCounters() {
    static long leftCounter  = 0;
    static long midCounter   = 1000;
    static long rightCounter = 50000;

    // Print counter values to each LCD panel
    lcd.printLeft(leftCounter);
    lcd.printMid(midCounter);
    lcd.printRight(rightCounter);

    // Increment values
    leftCounter++;
    midCounter += 2;
    rightCounter += 5;

    // Reset counters if they exceed panel display limits
    if (leftCounter > 99999)    leftCounter = 0;   // 5 digits max
    if (midCounter > 99999)     midCounter = 0;    // 5 digits max
    if (rightCounter > 999999)  rightCounter = 0;  // 6 digits max
}

void setup() {
  Serial.begin(115200);
  
  Serial.println(F("===================================="));
  Serial.println(F(" HT1621 Multi-Display Section Test  "));
  Serial.println(F("===================================="));

  // Initialize HT1621 driver
  lcd.begin(CS_PIN, WR_PIN, DATA_PIN, BLK_PIN);
  lcd.clear();
  lcd.backlight(200); // Set backlight to maximum brightness
  delay(1000);
}

void loop() {
  lcd.printCelsius(-4); // Display a sample temperature value
  delay(2000);
}