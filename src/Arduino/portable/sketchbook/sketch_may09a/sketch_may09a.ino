#include <LedControl.h>
#include <Servo.h>
#include <Wire.h>
#include <Arduino.h>
#include <Colon.h>
#include <Adafruit_NeoPixel.h>

Colon aControl;




void setup(){
    aControl.init();
    aControl.runwait();
    aControl.controlRGBLED(100,1);
    
}

void loop(){
    
}

