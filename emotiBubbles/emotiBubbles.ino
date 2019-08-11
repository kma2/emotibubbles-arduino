
/*
   This program implements the current functionality for EmotiBubbles.
   EmotiBubbles is an interactive soft tangible interface that helps kids with autism
   recognize, learn, and express their emotions.

   Fall 2017 CS320 Tangible User Interfaces
   Wellesley College
   Authors: Pooja Diwakar and Katy Ma
   Modified: 12/17/17
*/

// Import FastLED library to use for the LED strip lights
#include <FastLED.h>
#define NUM_LEDS 60 // there are 60 LEDs in the core LED strip light
#define DATA_PIN 2

// Initialize arduino pin number information for each of the 6 core emotions

// Initialize sadness emotiBubble pins
const int sadnessLed = 4;
const int sadPressurePin = A15;
const int sadnessVibratingPin = 49;
int sadnessForce;


// Initialize fear bubble pins
const int fearLed = 12;
const int fearPressurePin = A0;
const int fearVibratingPin = 45;
int fearForce;

// Initialize anger emotiBubble pins
const int angerLed = 8;
const int angerPressurePin = A1;
const int angerVibratingPin = 43;
int angerForce;

// Initialize disgust emotiBubble pins
const int disgustRedLed = 9;
const int disgustBlueLed = 10;
const int disgustPressurePin = A14;
const int disgustVibratingPin = 41;
int disgustForce;

// Initialize happiness emotiBubble pins
const int happinessPressurePin = A10;
const int happinessGreenLed = 11;
const int happinessRedLed = 3;
const int happinessVibratingPin = 31;
int happinessForce;

// Initialize surprise emotiBubble pins
const int surprisePressurePin = A8;
const int surpriseGreenLed = 6;
const int surpriseRedLed = 5;
const int surpriseVibratingPin = 35;
int surpriseForce;

// The RGB color code value for each of the colors that represent the six core emotions represented
const int BLUE = 171;
const int RED = 254;
const int GREEN = 85;
const int PURPLE = 192;
const int YELLOW = 64;
const int ORANGE = 25;

// The indices for each of the emotions within the LEDStrip arrays below
const int sadnessIndex = 0;
const int angerIndex = 1;
const int fearIndex = 2;
const int disgustIndex = 3;
const int happyIndex = 4;
const int surpriseIndex = 5;

// Initialize the LED strip lights
CRGB leds[NUM_LEDS];
int colorIntensity;

// index order: sadness, anger, fear, disgust, happiness, surprise
/* Contains the color intensities (between 0 and 255) of emotiBubbles that correlate to the force
   with which they are being pressed.
   these values will be averaged to set the intensity of the LED strip */
int LedStripColorIntensity[] = { -1, -1, -1, -1, -1, -1};

/* Contains the hardcoded RGB color values of each of the emotiBubbles.
   These values will be averaged to set the intensity of the LED strip */
int LedStripColorHue[] = {BLUE, RED, GREEN, PURPLE, YELLOW, ORANGE};

/*
   Sets up and initializes the sensors/leds/motors used
*/
void setup() {
  // Make sure that LED strip is off when application starts
  FastLED.clear ();

  // Initialize the output pins for each of the emotions and initialize the force to be 0 for each

  // Setup blue sadness emotiBubble pins
  pinMode(sadnessLed, OUTPUT);
  pinMode(sadnessVibratingPin, OUTPUT);
  sadnessForce = 0;

  // Setup red anger emotiBubble pins
  pinMode(angerLed, OUTPUT);
  pinMode(angerVibratingPin, OUTPUT);
  angerForce = 0;

  // Setup green fear emotiBubble pins
  pinMode(fearLed, OUTPUT);
  pinMode(fearVibratingPin, OUTPUT);
  fearForce = 0;

  // Setup purple (blue + red) disgust emotiBubble pins
  pinMode(disgustRedLed, OUTPUT);
  pinMode(disgustBlueLed, OUTPUT);
  pinMode(disgustVibratingPin, OUTPUT);
  disgustForce = 0;

  // Setup yellow (green + red) happiness emotiBubble pins
  pinMode(happinessRedLed, OUTPUT);
  pinMode(happinessGreenLed, OUTPUT);
  pinMode(happinessVibratingPin, OUTPUT);
  happinessForce = 0;

  // Setup orange (green + red) surprise emotiBubble pins
  pinMode(surpriseRedLed, OUTPUT);
  pinMode(surpriseGreenLed, OUTPUT);
  pinMode(surpriseVibratingPin, OUTPUT);
  surpriseForce = 0;

  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);
  Serial.begin(9600);     //Begin serial communication

}

/*
   Iteratively reads force sensitive resistors contained within each emotiBubble.
   If the force is over 70 (to account for peripheral/minimal force detections)
   on a range of 0-1000, calculate and reflect the correlating color/color intensities
   on the individual LEDs and the LED strip lights. Trigger the vibrating motor(s) on as well.

   If the force is not over 70 (no bubble actively pressed), turn LEDs and vibrating motors off.
   Reinitialize the corresponding LedStripColorIntensity value to -1.
*/
void loop() {


  sadnessForce = analogRead(sadPressurePin);
  if (sadnessForce > 70) {
    // calculates the color intensity by expressing the force proportionally as an RGB value on a scale of 0-255
    colorIntensity = abs((sadnessForce * 255L) / 1000L);
    analogWrite(sadnessLed, colorIntensity);
    digitalWrite(sadnessVibratingPin, HIGH);
    LedStripColorIntensity[sadnessIndex] = colorIntensity;
  } else {
    digitalWrite(sadnessLed, LOW);
    digitalWrite(sadnessVibratingPin, LOW);
    LedStripColorIntensity[sadnessIndex] = -1;
  }

  fearForce = analogRead(fearPressurePin);
  if (fearForce > 70) {
    // calculates the color intensity by expressing the force proportionally as an RGB value on a scale of 0-255
    colorIntensity = abs((fearForce * 255L) / 1000L);
    LedStripColorIntensity[fearIndex] = colorIntensity;
    analogWrite(fearLed, colorIntensity);
    digitalWrite(fearVibratingPin, HIGH);
  } else {
    digitalWrite(fearLed, LOW);
    digitalWrite(fearVibratingPin, LOW);
    LedStripColorIntensity[fearIndex] = -1;
  }

  angerForce = analogRead(angerPressurePin);
  if (angerForce > 70) {
    // calculates the color intensity by expressing the force proportionally as an RGB value on a scale of 0-255
    colorIntensity = abs((angerForce * 255L) / 1000L);
    analogWrite(angerLed, colorIntensity);
    digitalWrite(angerVibratingPin, HIGH);
    LedStripColorIntensity[angerIndex] = colorIntensity;
  } else {
    digitalWrite(angerLed, LOW);
    digitalWrite(angerVibratingPin, LOW);
    LedStripColorIntensity[angerIndex] = -1;
  }

  disgustForce = analogRead(disgustPressurePin);
  if (disgustForce > 70) {
    // calculates the color intensity by expressing the force proportionally as an RGB value on a scale of 0-255
    colorIntensity = abs((disgustForce * 255L) / 1000L);
    analogWrite(disgustRedLed, colorIntensity);
    analogWrite(disgustBlueLed, colorIntensity);
    digitalWrite(disgustVibratingPin, HIGH);
    LedStripColorIntensity[disgustIndex] = colorIntensity;
  } else {
    digitalWrite(disgustRedLed, LOW);
    digitalWrite(disgustBlueLed, LOW);
    digitalWrite(disgustVibratingPin, LOW);
    LedStripColorIntensity[disgustIndex] = -1;
  }

  happinessForce = analogRead(happinessPressurePin);
  if (happinessForce > 70) {
    // calculates the color intensity by expressing the force proportionally as an RGB value on a scale of 0-255
    colorIntensity = abs((happinessForce * 255L) / 1000L);
    analogWrite(happinessRedLed, colorIntensity);
    analogWrite(happinessGreenLed, colorIntensity);
    digitalWrite(happinessVibratingPin, HIGH);
    LedStripColorIntensity[happyIndex] = colorIntensity;
  } else {
    digitalWrite(happinessRedLed, LOW);
    digitalWrite(happinessGreenLed, LOW);
    digitalWrite(happinessVibratingPin, LOW);
    LedStripColorIntensity[happyIndex] = -1;
  }

  surpriseForce = analogRead(surprisePressurePin);
  if (surpriseForce > 70) {
    // calculates the color intensity by expressing the force proportionally as an RGB value on a scale of 0-255
    colorIntensity = abs((surpriseForce * 255L) / 1000L);
    analogWrite(surpriseRedLed, colorIntensity);
    analogWrite(surpriseGreenLed, colorIntensity);
    digitalWrite(surpriseVibratingPin, HIGH);
    LedStripColorIntensity[surpriseIndex] = colorIntensity;
  } else {
    digitalWrite(surpriseRedLed, LOW);
    digitalWrite(surpriseGreenLed, LOW);
    digitalWrite(surpriseVibratingPin, LOW);
    LedStripColorIntensity[surpriseIndex] = -1;
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
