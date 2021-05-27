#include <Arduino.h>

const int waitingTime = 24;   // Hours
const int wateringTime = 3;   // Seconds

int secondsElapsed = 0;
int minutesElapsed = 0;
int hoursElapsed = 0;

unsigned long startTime;
unsigned long currentTime;

bool initCounter;


void setup() {

}

void loop() {
  if (initCounter) {
    initCounter = false;
    startTime   = millis();
  }

  currentTime = millis();

  if (currentTime - startTime >= 1000){
    secondsElapsed += 1;
  }

  if (secondsElapsed = 60){
    secondsElapsed = 0;
    minutesElapsed += 1;
  }

  if (minutesElapsed = 60){
    minutesElapsed = 0;
    hoursElapsed += 1;
  }

  if (hoursElapsed = waitingTime){
    secondsElapsed = 0;
    minutesElapsed = 0;
    hoursElapsed = 0;
    initCounter = true;

    //Action for wateringTime seconds
  }
}