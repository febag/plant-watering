#include <Arduino.h>

const unsigned long waitingTime = 259200;   // 3 days in seconds
const unsigned long wateringTime = 120;     // Seconds
const int pinPushButton = 8;
const int pinPump = 9;
const unsigned long debounceDelay = 50;     // miliseconds

unsigned long secondsElapsed = 0;

unsigned long startTime;
unsigned long currentTime;
unsigned long lastDebounceTime = 0;


bool initCounter;
bool buttonState;
bool prevButtonState;

int status;   // 1 = Waiting, 2 = Watering


void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(pinPushButton, INPUT_PULLUP);
  pinMode(pinPump, OUTPUT);
  //Serial.begin(9600);
  status = 1;
}

void loop() {

  // Seconds counter

  if (initCounter) {
    initCounter = false;
    startTime   = millis();
  }

  currentTime = millis();

  if (currentTime - startTime >= 1000){
    secondsElapsed += 1;
    initCounter = true;
  }

  /*Serial.print("Pulsador: ");
  Serial.println(buttonState);

  Serial.print("Estado: ");
  Serial.println(status);

  delay(1000);*/

  // Button pressed -> Go to status 2
  buttonState = digitalRead(pinPushButton);

  if (buttonState != prevButtonState){
    lastDebounceTime = millis();
  }

  if ((millis()-lastDebounceTime) > debounceDelay){
    // Only change status if the button has been pressed
    if (buttonState == LOW){
      secondsElapsed = 0;
      status = 2;
    }
  }

  prevButtonState = buttonState;

  // Pump activation

  switch (status){
    case 1:
      digitalWrite(LED_BUILTIN, LOW);
      digitalWrite(pinPump, LOW);

      if (secondsElapsed == waitingTime){
        secondsElapsed = 0;
        status = 2;
      }
      break;

    case 2:
      digitalWrite(LED_BUILTIN, HIGH);
      digitalWrite(pinPump, HIGH);

      if (secondsElapsed == wateringTime){
        secondsElapsed = 0;
        status = 1;
      }
      break;
  }

}