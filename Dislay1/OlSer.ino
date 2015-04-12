#include <SPI.h>
#include "SH1106_SPI.h"

#define USE_FRAME_BUFFER

#ifdef USE_FRAME_BUFFER
SH1106_SPI_FB lcd;
#else
SH1106_SPI lcd;
#endif

String inputString = "";// a string to hold incoming data
String inStr = "";
boolean stringComplete = false;  // whether the string is complete
double Input;
int led = 13;
int num = 0;

void setup(void)
{
  pinMode(led, OUTPUT);
  // initialize serial:
  Serial.begin(9600);
  Serial1.begin(9600);
  // reserve 200 bytes for the inputString:
  inputString.reserve(100);
	lcd.begin();
	lcd.gotoXY(40,2);
        lcd.print(F("SpaceApps"));
        lcd.gotoXY(55,3);
        lcd.print(F("2015"));
#ifdef USE_FRAME_BUFFER
	lcd.renderAll();
#endif
	delay(3000);
  lcd.clear();
  lcd.gotoXY(49,3);
  lcd.print(F("Tormes"));

#ifdef USE_FRAME_BUFFER
	lcd.renderAll();
#endif
  delay(4500);
  
  lcd.clear();
  
  lcd.print(F("Temp"));
  lcd.gotoXY(100,0);
  lcd.print(F("Hum"));
  
  lcd.gotoXY(1,3);
  lcd.print(F("MQ2"));
  lcd.gotoXY(95,3);
  lcd.print(F("MQ135"));
  lcd.gotoXY(20,6);
  lcd.print(F("Presion"));
 #ifdef USE_FRAME_BUFFER
	lcd.renderAll();
#endif 
}

void loop(void) 
{
  
  if (stringComplete) {
    Serial.println(Input);
    Serial.println(num); 
    if(num == 8){
      // Dato temperatura
        lcd.gotoXY(0,1);
        lcd.print(inStr);
    }
    if(num == 10){
      // Dato Humedad
        lcd.gotoXY(95,1);
        lcd.print(inStr);
    }
    if(num == 11){
      // Dato MQ2
        lcd.gotoXY(0,4);
        lcd.print(inStr);
        if (Input > 120){
          lcd.gotoXY(35,3);
          lcd.print(F("Dang"));
        }
    }
    if(num == 12){
      // Dato MQ135
        lcd.gotoXY(95,4);
        lcd.print(inStr);
        if (Input > 120){
          lcd.gotoXY(65,3);
          lcd.print(F("Dang"));
        }
    }
    if(num == 13){
      // Dato Presion
        lcd.gotoXY(70,6);
        lcd.print(inStr);
    }
      #ifdef USE_FRAME_BUFFER
	lcd.renderAll();
      #endif
      inputString = "";
      stringComplete = false;
  }
}

void serialEvent1() {
  while (Serial1.available()) {
    // get the new byte:
    char inChar = (char)Serial1.read();
    //2 if the incoming character is an "F", set a flag
    // Fin de la cadena
    if (inChar == 'F') {
      //stringComplete = true;
      num = 0;
      Serial.println("Final de trama");
    }
    
    // if the incoming character is an ":", set a flag
    // so the main loop can do something about it: 
    if (inChar == ':') {
      stringComplete = true;
      inStr = inputString;
      char buf[inputString.length()];
      inputString.toCharArray(buf,inputString.length()+1);
      Input = atof(buf); // Edit the 6 to how many digits precision you need
      num = num + 1; 
    }
    else{
      // add it to the inputString:
      inputString += inChar;
    } 
  }
}
