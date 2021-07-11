#include <Arduino.h>

bool debounce(bool buttonState){
    unsigned long debounceDelay = 50;  // miliseconds
    static unsigned long lastDebounceTime = millis();
    static bool prevFiltState = buttonState;

    if (buttonState != prevFiltState){
        lastDebounceTime = millis();
    }

    if ((millis()-lastDebounceTime) > debounceDelay){
        prevFiltState = buttonState;
    }
    
    return prevFiltState;
}