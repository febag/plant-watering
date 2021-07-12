#include <Arduino.h>
//#include "utils.h"
#include <Wire.h>
#include "RTClib.h"

const int pinPushButtonC1 = 6;
//const int pinPushButtonC2 = 6;
const int pinPushButtonStop = 7;
const int pinPump = 8;
const int pinLed = 9;
//const int pinValve1 = 9;
//const int pinValve2 = 10;
const unsigned long wateringTime = 3;       // Seconds
//const unsigned long wateringTimeC1 = 3;     // Seconds
//const unsigned long wateringTimeC2 = 5;     // Seconds
const unsigned long debounceDelay = 50;     // miliseconds

bool buttonC1, buttonC2, buttonStop, prevButtonStateC1, prevButtonStateStop;

RTC_DS3231 rtc;
String daysOfTheWeek[7] = { "Domingo", "Lunes", "Martes", "Miercoles", "Jueves", "Viernes", "Sabado" };
String monthsNames[12] = { "Enero", "Febrero", "Marzo", "Abril", "Mayo",  "Junio", "Julio","Agosto","Septiembre","Octubre","Noviembre","Diciembre" };
DateTime now;
bool wateringDone;

int step;             // 1 = Waiting, 2 = Watering
int prev_step;
int autoMode;         // 0 = Manual Mode, 1 = Auto Mode
int selectedCircuit;  // 0 = Valve1 circuit, 1 = Valve2 circuit
int pinValve;
//unsigned long wateringTime;

unsigned long secondsElapsed = 0;
unsigned long startTime;
unsigned long currentTime;
unsigned long lastDebounceTimeC1 = 0;
unsigned long lastDebounceTimeStop = 0;

int counter = 0;

void setup() {
  Serial.begin(9600);
  delay(1000);

  if (!rtc.begin()) {
    Serial.println(F("Couldn't find RTC"));
    while (1);
  }
  if (rtc.lostPower()) {
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }

  pinMode(pinPushButtonC1, INPUT_PULLUP);
  //pinMode(pinPushButtonC2, INPUT_PULLUP);
  pinMode(pinPushButtonStop, INPUT_PULLUP);
  pinMode(pinPump, OUTPUT);
  pinMode(pinLed, OUTPUT);
  //pinMode(pinValve1, OUTPUT);
  //pinMode(pinValve2, OUTPUT);
}

void printDate(DateTime date)
{
   Serial.print(date.year(), DEC);
   Serial.print('/');
   Serial.print(date.month(), DEC);
   Serial.print('/');
   Serial.print(date.day(), DEC);
   Serial.print(" (");
   Serial.print(daysOfTheWeek[date.dayOfTheWeek()]);
   Serial.print(") ");
   Serial.print(date.hour(), DEC);
   Serial.print(':');
   Serial.print(date.minute(), DEC);
   Serial.print(':');
   Serial.print(date.second(), DEC);
   Serial.println();
}

bool isScheduledON(DateTime date)
{
   int weekDay = date.dayOfTheWeek();
   float hours = date.hour() + date.minute() / 60.0;
   // At 06:00 turn on the sequence
   bool hourCondition = (hours > 6.00);
   //bool dayCondition = (weekDay == 3 || weekDay == 6 || weekDay == 0);

   if (hourCondition)
   {
      return true;
   }
   return false;
}


void loop() {

  /*Serial.print("Pulsador: ");
  Serial.println(buttonState);*/

  /*if (counter == 4200){  //Every 5s
    now = rtc.now();
    printDate(now);
    Serial.print("Condiciones de riego: ");
    Serial.println(isScheduledON(now));
    Serial.print("wateringDone: ");
    Serial.println(wateringDone);

    Serial.print("Estado: ");
    Serial.println(step);

    Serial.print("SecondsElapsed: ");
    Serial.println(secondsElapsed);

    counter = 0;
  }
  else {
    counter += 1;
  }*/



// USER INPUT MANAGEMENT
  // Button pressed -> Go to step 2
  /*buttonC1 = debounce(digitalRead(pinPushButtonC1));
  buttonC2 = debounce(digitalRead(pinPushButtonC2));
  buttonStop = debounce(digitalRead(pinPushButtonStop));

  if (buttonC1 == LOW){
    autoMode = 0;
    selectedCircuit = 0;
    step = 1;
  }
  else if (buttonC2 == LOW){
    autoMode = 0;
    selectedCircuit = 1;
    step = 1; 
  }
  else if (buttonStop == LOW) {
    step = 3;
  }*/

  //Function?
  buttonC1 = digitalRead(pinPushButtonC1);
  buttonStop = digitalRead(pinPushButtonStop);

  if (buttonC1 != prevButtonStateC1){
    lastDebounceTimeC1 = millis();
  }
  if (buttonStop != prevButtonStateStop){
    lastDebounceTimeStop = millis();
  }

  if ((millis()-lastDebounceTimeC1) > debounceDelay){
    // Only change step if the button has been pressed
    if (buttonC1 == LOW){
      //autoMode = 0;
      //selectedCircuit = 0;
      step = 1;
    }
  }
  if ((millis()-lastDebounceTimeStop) > debounceDelay){
    // Only change step if the button has been pressed
    if (buttonStop == LOW){
      step = 2;
    }
  }

  prevButtonStateC1 = buttonC1;
  prevButtonStateStop = buttonStop;



// SEQUENCE TRIGGER
// RTC code
  now = rtc.now();

  if (wateringDone == false && isScheduledON(now)){
    step = 1;
    wateringDone = true;
  }
  // At 00:00 clear the condition
  else if (wateringDone == true && isScheduledON(now) == false){
    wateringDone = false;
  }

  //debug
  if (wateringDone == true){
    digitalWrite(pinLed, HIGH);
  }
  else {
    digitalWrite(pinLed, LOW);
  }


// WATERING SEQUENCE

  /*switch (step){
    case 0:
      break;

    case 1:   // Pump activation
      digitalWrite(pinPump, HIGH);

      if (secondsElapsed == 2){
        step = 2;
      }
      break;

    case 2:   // Open Valve
      if (selectedCircuit == 0){
        pinValve = pinValve1;
        wateringTime = wateringTimeC1;
      }
      else {
        pinValve = pinValve2;
        wateringTime = wateringTimeC2;
      }

      digitalWrite(pinValve, HIGH);

      if (secondsElapsed == wateringTime){
        step = 3;
      }
      break;

    case 3:   // Pump deactivation
      digitalWrite(pinPump, LOW);

      if (secondsElapsed == 2){
        step = 4;
      }
      break;

    case 4:   // Close Valve
      digitalWrite(pinValve1, LOW);

      if (autoMode == 1 & selectedCircuit == 0){
        selectedCircuit = 1;
        step = 1;
      }
      else {
        step = 0;
      }
      break;
  }*/

  switch (step){
    case 0:
      break;

    case 1:   // Pump activation
      digitalWrite(pinPump, HIGH);

      if (secondsElapsed == wateringTime){
        step = 2;
      }
      break;

    case 2:   // Pump deactivation
      digitalWrite(pinPump, LOW);
      step = 0;
      break;
  }

//  ELAPSED TIME MANAGEMENT
  // Seconds counter -> Function? Class?
  if (step != prev_step) {
    secondsElapsed = 0;
  }
  prev_step = step;

  currentTime = millis();

  if (currentTime - startTime >= 1000){
    secondsElapsed += 1;
    startTime   = millis();
  }

}