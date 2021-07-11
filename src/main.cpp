#include <Arduino.h>
#include "utils.h"

const int pinPushButtonC1 = 8;
const int pinPushButtonC2 = 9;
const int pinPushButtonStop = 10;
const int pinPump = 0;
const int pinValve1 = 1;
const int pinValve2 = 2;
//const unsigned long waitingTime = 259200;   // 3 days in seconds
const unsigned long wateringTimeC1 = 120;     // Seconds
const unsigned long wateringTimeC2 = 120;     // Seconds
const unsigned long debounceDelay = 50;       // miliseconds

bool buttonC1, buttonC2, buttonStop;

int step;             // 1 = Waiting, 2 = Watering
int prev_step;
int autoMode;         // 0 = Manual Mode, 1 = Auto Mode
int selectedCircuit;  // 0 = Valve1 circuit, 1 = Valve2 circuit
int pinValve;
unsigned long wateringTime;

unsigned long secondsElapsed = 0;
unsigned long startTime;
unsigned long currentTime;
unsigned long lastDebounceTime = 0;

void setup() {
  pinMode(pinPushButtonC1, INPUT_PULLUP);
  pinMode(pinPushButtonC2, INPUT_PULLUP);
  pinMode(pinPushButtonStop, INPUT_PULLUP);
  pinMode(pinPump, OUTPUT);
  pinMode(pinValve1, OUTPUT);
  pinMode(pinValve2, OUTPUT);
  //Serial.begin(9600);
}

void loop() {

  /*Serial.print("Pulsador: ");
  Serial.println(buttonState);

  Serial.print("Estado: ");
  Serial.println(step);

  delay(1000);*/


// USER INPUT MANAGEMENT
  // Button pressed -> Go to step 2
  buttonC1 = debounce(digitalRead(pinPushButtonC1));
  buttonC2 = debounce(digitalRead(pinPushButtonC2));
  buttonStop = debounce(digitalRead(pinPushButtonStop));

  //Function?
  /*if (buttonState != prevButtonState){
    lastDebounceTime = millis();
  }

  if ((millis()-lastDebounceTime) > debounceDelay){
    // Only change step if the button has been pressed
    if (buttonState == LOW){
      secondsElapsed = 0;
      step = 2;
    }
  }

  prevButtonState = buttonState;*/

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
  }


// SEQUENCE TRIGGER
// RTC code


// WATERING SEQUENCE

  switch (step){
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
      digitalWrite(pinValve1, LOW);

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