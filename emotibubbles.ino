
/*
   This program implements the current functionality for EmotiBubbles.
   EmotiBubbles is an interactive soft tangible interface that helps children
   recognize, learn, and express their emotions.
   Spring 2018 Independent Study
   Wellesley College
   Authors: Pooja Diwakar and Katy Ma
   Modified: 5/21/18
*/

// Import FastLED library to use for the LED strip lights
#include <FastLED.h>
#define NUM_LEDS 60 // there are 60 LEDs in the core LED strip light
#define DATA_PIN 7

#define EMOTIONS_LENGTH 6

// Struct for each individual emotion bubble on the pillow with the attributes that each bubble will have
struct Bubble {
  int pressurePin;
  int vibPin; 
  String emotion; //the name of the emotion
  /* specifies 3 diff color pins [red, green, blue]. 
    If any one of them is zero, then that color is not valid for that emotion*/
  int rgbPins[3]; 
  int LEDStripIndex; // to index into LedStripColorHue and LedStripColorIntensity arrays
};

/* Instantiate objects for each of the specific emotibubbles on the pillow
   Each object is instantiated with the pin numbers relevant to the wiring for each emotion.
   */
Bubble emotions[EMOTIONS_LENGTH] = {

  {0, 30, "happy", {13, 12, 0},4}, // uses red and green LEDs
  {A1, 52, "sad", {0,0,9},0},  // uses only blue LED 
  {A2, 23, "anger", {6,0,0},1}, // only uses red LED 
  {A3, 40, "surprise", {10,11,0},5}, // uses only red and green LEDs
  {A4, 31, "fear", {0, 8, 0},2}, // uses only green LED
  {A5, 26, "disgust", {2, 0, 3},3}, // uses only red and blue LEDs
};

// Initialize the LED strip lights
CRGB leds[NUM_LEDS];
int colorIntensity;
int force;
// The RGB color code value for each of the colors that represent the six core emotions represented-- check to see if these are hardcoded (!)
const int BLUE = 171;
const int RED = 254;
const int GREEN = 85;
const int PURPLE = 192;
const int YELLOW = 64;
const int ORANGE = 25;

// index order: sadness, anger, fear, disgust, happiness, surprise
/* Contains the color intensities (between 0 and 255) of emotiBubbles that correlate to the force
   with which they are being pressed.
   these values will be averaged to set the intensity of the LED strip */
int LedStripColorIntensity[] = { -1, -1, -1, -1, -1, -1};

/* Contains the hardcoded RGB color values of each of the emotiBubbles.
   These values will be averaged to set the intensity of the LED strip */
int LedStripColorHue[] = {BLUE, RED, GREEN, PURPLE, YELLOW, ORANGE};

void setup() {

  Serial.begin(9600);     //Begin serial communication

  // Make sure that LED strip is off when application starts
  FastLED.clear ();

    
  // iterate through emotions and set up vibrating pins and non-0 val RGB pins to be output
  for(int i = 0; i<EMOTIONS_LENGTH; i++){ // change 6 to emotions.length
    Bubble bubble = emotions[i];
    pinMode(bubble.vibPin, OUTPUT);

    for(int j = 0; j<3; j++){
      int ledPin = bubble.rgbPins[j];
      if(ledPin!=0){
        pinMode(ledPin, OUTPUT);
      }
    } 
  }
  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);
}

void loop() {
  // put your main code here, to run repeatedly:

  // iterate through emotions array and update the force value and color intensity
  for(int i = 0; i<EMOTIONS_LENGTH; i++){
      Bubble bubble = emotions[i];
      int pressureP = bubble.pressurePin;
      force = analogRead(pressureP);


        if(force>30){ //baseline value
        // coordinate it with the force once that works
        digitalWrite(bubble.vibPin, LOW);

        for(int j = 0; j<3; j++){
          int ledPin = bubble.rgbPins[j];
        
          if(ledPin!=0){
            colorIntensity = abs((force * 255L) / 300L);
            analogWrite(ledPin, colorIntensity);
            LedStripColorIntensity[bubble.LEDStripIndex] = colorIntensity;
            colorIntensity = 0;
          }
        }
      } else{
        digitalWrite(bubble.vibPin, HIGH);
          for(int j = 0; j<3; j++){
            int ledPin = bubble.rgbPins[j];
        
            if(ledPin!=0){
              digitalWrite(ledPin, LOW);
              LedStripColorIntensity[bubble.LEDStripIndex] = -1;
            }
        }
      }    
  }

  // After individual LEDs have been processed (turned off or on to whatever intensity) the core LED strip light must be processed.
  updateLedStrip();
}

// Updates the LED strip light based on the force and correlated color intensity of each individual pressed bubble
void updateLedStrip() {
  int intensityAverage = 0;
  int intensityTotal = 0;
  int colorAverage = 0;
  int colorTotal = 0;
  int numPressed = 0;

  // 6 is the length of the intensity array -- the number of emotiBubbles
  for (int i = 0; i < 6; i++) {
    
    // If at least one of the bubbles is being pressed with some minimal pressure, the number at that index is not -1
    if (LedStripColorIntensity[i] != -1) {
      // Add the color intensity of the pressed bubble to the total of intensity and color values
      intensityTotal += LedStripColorIntensity[i];
      colorTotal += LedStripColorHue[i];
      
      numPressed++;
    }
  }
  // If nothing has been pressed, turn the LED strip lights off
  if (numPressed == 0) {
    FastLED.clear ();
  }

  /*
     If at least one emotiBubble is pressed, calculate the average RGB color value for the LED strip
     based on which bubble(s) were pressed.
  */
  else {
    colorAverage = float(colorTotal) / numPressed;
    intensityAverage = float(intensityTotal) / numPressed;
    
    FastLED.showColor(CHSV(colorAverage, 255, intensityAverage));
    delay(10);
  }
  return;
}
