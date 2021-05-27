#include <Arduino.h>

const int waitingTime = 10;   // Seconds
const int wateringTime = 3;   // Seconds
const int pinBuzzer = 9;

int secondsElapsed = 0;

unsigned long startTime;
unsigned long currentTime;

bool initCounter;

int status;   // 1 = Waiting, 2 = Watering


void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
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

  /*Serial.print("Contador: ");
  Serial.println(secondsElapsed);

  Serial.print("Estado: ");
  Serial.println(status);

  delay(1000);*/

  // Pump activation

  switch (status){
    case 1:
      digitalWrite(LED_BUILTIN, LOW);
      noTone(pinBuzzer);

      if (secondsElapsed == waitingTime){
        secondsElapsed = 0;
        status = 2;
      }
      break;

    case 2:
      digitalWrite(LED_BUILTIN, HIGH);
      tone(pinBuzzer, 440);

      if (secondsElapsed == wateringTime){
        secondsElapsed = 0;
        status = 1;
      }
      break;
  }

}