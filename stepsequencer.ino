#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>

// GUItool: begin automatically generated code
AudioSynthWaveform       waveform1;      //xy=326,295
AudioOutputAnalog        dac1;           //xy=529,337
AudioConnection          patchCord1(waveform1, dac1);
// waveform generator import

int led1 = 11;
int led2 = 10;
int led3 = 9;
int led4 = 8;
int led5 = 7;
int buttonPin1 = 37;
int buttonPin2 = 36;
int buttonPin3 = 35;
int buttonPin4 = 34;
int buttonPin5 = 33;
int leds[5] = { led1, led2, led3, led4, led5 }; //LED locations
int potPin = A14; //location of tempo pot
int midiArray[5] = {60, 60, 60, 60, 60}; //empty midi notes
int tempo = 0; //tempo variable
int currentStep = 0;
unsigned long lastStepTime = 0;
int switchPin1 = 32; //toggleswitch location
boolean lastButtonState[5] = { LOW, LOW, LOW, LOW, LOW };
boolean buttonState[5] = { LOW, LOW, LOW, LOW, LOW };
boolean on[5] = { false, false, false, false, false };
int buttonPins[5] = { buttonPin1, buttonPin2, buttonPin3, buttonPin4, buttonPin5 };



void setup() {
  
  for (int i = 0; i < 5; i++) {
    pinMode(leds[i], OUTPUT); //claiming everything on breadboard as inputs/ outputs
    pinMode(buttonPins[i], INPUT);
  }
  pinMode(switchPin1, INPUT); //claiming toggles as inputs
}



void sequence( ) { //change without delay
  tempo = analogRead(potPin);

  if (millis( ) > lastStepTime + tempo) {
    digitalWrite(leds[currentStep], LOW);

    if (digitalRead(switchPin1) == HIGH) {
      incrementDown( );
    }

    else {
      incrementUp( );
    }

    lastStepTime = millis( );
    digitalWrite(leds[currentStep], HIGH);
    if (on[currentStep] == true) {
      usbMIDI.sendNoteOff(midiArray[currentStep], 0, 1);
      usbMIDI.sendNoteOn(midiArray[currentStep], 127, 1); //send midi note to DAW
    }

  }
}


void incrementUp( ) {
  //increment up
  currentStep += 1;
  if (currentStep > 4) {
    currentStep = 0;
  }
}


void incrementDown( ) {
  //increment down
  currentStep --;
  if (currentStep < 0) {
    currentStep = 4;
  }
}


void checkButtons( ) {

  for (int i = 0; i < 5; i++) {
    lastButtonState[i] = buttonState[i];
    buttonState[i] = digitalRead(buttonPins[i]);

    if (buttonState[i] == HIGH && lastButtonState[i] == LOW) {
      if (on[i] == false)
        on[i] = true;
      else if (on[i] == true)
        on[i] = false;
    }
  }
}



void setLeds() {

  for (int i = 0; i < 5; i++) {
    if (on[i] == true) {
      digitalWrite(leds[i], HIGH);
      digitalWrite(leds[currentStep], HIGH);

    } else if (on[i] == false) {
      digitalWrite(leds[i], LOW);
      digitalWrite(leds[currentStep], HIGH);
    }
  }
}


void loop() { //loop that allows toggle to choose which function mode you are in
  sequence();
  checkButtons();
  setLeds();
}
