#include <Wire.h>
#include <DS3231.h>


// time for testing : 2102264074445x , and for testing daylight savings: 2103141015945x and 2111071015945x YYMMDDwHHMMSSx

RTClib myRTC;
DS3231 clock;

bool h12Flag;
bool pmFlag;
bool century = false;

const int buttonOne = 5;
const int buttonTwo = 4;
int buttonOneState = 0;
int buttonTwoState = 0;
int lastButtonState1 = LOW;
int lastButtonState2 = LOW;
int ledOneState = LOW;
int ledTwoState = LOW;

int saving;
 
int relaytrigger = 0;

int DoW, Hour, Minute, Second, Month, Date, Year;

const int ledOne =  7;
const int ledTwo =  8;
const int relay =  9;

void setup() {


  
  clock.turnOffAlarm(1);
  clock.turnOffAlarm(2);
  clock.setClockMode(true);
  Serial.begin(57600);
  Wire.begin();


  pinMode(buttonOne, INPUT);
  pinMode(buttonTwo, INPUT);

  pinMode(ledOne, OUTPUT);
  pinMode(ledTwo, OUTPUT);
  pinMode(relay, OUTPUT);
}

void loop() {

  DateTime now = myRTC.now();
  int reading1 = digitalRead(buttonOne);

  if (reading1 != buttonOneState) {
    delay(50);
    buttonOneState = reading1;

    if (buttonOneState == HIGH) {
      ledOneState = !ledOneState;
    }
   }
 buttonTwoState = digitalRead(buttonTwo);
 if (buttonTwoState == HIGH){
   ledTwoState = HIGH;
 }else{
   ledTwoState = LOW;
 }
 
  Serial.print(clock.getMonth(century), DEC);
  Serial.print(' ');
  Serial.print(clock.getDate(), DEC);
  Serial.print(' ');
  Serial.print(clock.getDoW(), DEC);
  Serial.print(' ');
  Serial.print(clock.getHour(h12Flag, pmFlag), DEC);
  Serial.print(':');
  Serial.print(clock.getMinute(), DEC);
  Serial.print(':');
  Serial.print(clock.getSecond(), DEC);
  Serial.println();
  
  digitalWrite(ledOne, ledOneState);
  digitalWrite(ledTwo, ledTwoState);
  Year = (clock.getYear());
  DoW = (clock.getDoW());
  Hour = (clock.getHour(h12Flag, pmFlag));
  Minute = (clock.getMinute());
  Second = (clock.getSecond());
  Month = (clock.getMonth(century));
  Date = (clock.getDate());
/*
if(Month == 3){
  if (DoW == 1){
    if (7 < Date && Date < 15){
      if(Hour == 2){
  
          
      }
    }
  }
}
if(Month == 11){
  if (DoW == 1){
    if (Date < 8){
      if (Hour == 2){
        
        
      }
    }
  }
}
*/
int DST;
// ********************* Calculate offset for Sunday *********************
   int y = Year;                          // DS3231 uses two digit year (required here)
   int x = (y + y/4 + 2) % 7;      // remainder will identify which day of month
                                                // is Sunday by subtracting x from the one
                                                // or two week window.  First two weeks for March
      Serial.print("x="); Serial.print(x);  Serial.println();                                       // and first week for November
 // *********** Test DST: BEGINS on 2nd Sunday of March @ 2:00 AM *********
   if(Month == 3 && Date == (14 - x) && Hour >= 2)
       {                                   
        DST = 1;   
        Serial.print("mark1");                        // Daylight Savings Time is TRUE (add one hour)
       }
    
   if((Month == 3 && Date > (14 - x)) || Month > 3)
       {
        DST = 1;
          Serial.print("mark2");
       }
// ************* Test DST: ENDS on 1st Sunday of Nov @ 2:00 AM ************       
   if(Month == 11 && Date == (7 - x) && Hour >= 2)
       {
        DST = 0;                            // daylight savings time is FALSE (Standard time)
       }
   if((Month == 11 && Date > (7 - x)) || Month > 11 || Month < 3)
       {
        DST = 0;
       }
       Serial.print("DST="); Serial.print(DST);  Serial.println();  
   if(DST == 1)                        // Test DST and add one hour if = 1 (TRUE)
       {
        Hour = Hour + 1;
       }

Serial.print("effHour:");
Serial.print(Hour);
Serial.println();
if (ledOneState == LOW){
  if (DoW >= 2 && DoW <= 6){
   if (Hour == 7 ){
      if (Minute == 45){
        if (Second == 1){
        relaytrigger = 1;
        }
      }
    }
  }
}

if (ledTwoState == HIGH){
  digitalWrite(relay, HIGH);
}else{
  digitalWrite(relay, LOW);
}
  if (relaytrigger == 1) {
    digitalWrite(relay, HIGH);
    digitalWrite(ledTwo, HIGH);
    delay(200);
    digitalWrite(relay, LOW);
    digitalWrite(ledTwo, LOW);
    delay(810);
    relaytrigger = 0;
    
  }

}
