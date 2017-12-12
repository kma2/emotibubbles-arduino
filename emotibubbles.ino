#include <FastLED.h>
#define NUM_LEDS 60
#define DATA_PIN 2

const int sadnessLed = 4;
const int sadPressurePin = A15; //pin A0 to read analog input
const int sadnessVibratingPin = 47;
int sadnessForce;

const int fearLed = 12;
const int fearPressurePin = A0; //pin A0 to read analog input
const int fearVibratingPin = 45;
int fearForce;

const int angerLed = 8;
const int angerPressurePin = A1; //pin A0 to read analog input
const int angerVibratingPin = 43;
int angerForce;

const int disgustRedLed = 9;
const int disgustBlueLed = 10;
const int disgustPressurePin = A14; //pin A0 to read analog input
const int disgustVibratingPin = 41;
int disgustForce;

const int happinessPressurePin = A10;
const int happinessGreenLed = 11;
const int happinessRedLed = 3;
const int happinessVibratingPin = 31;
int happinessForce;

const int surprisePressurePin = A8;
const int surpriseGreenLed = 6;
const int surpriseRedLed = 5;
const int surpriseVibratingPin = 35;
int surpriseForce;

const int BLUE = 171;
const int RED = 254;
const int GREEN = 85;
const int PURPLE = 192;
const int YELLOW = 64;
const int ORANGE = 25;

const int sadnessIndex = 0;
const int angerIndex = 1;
const int fearIndex = 2;
const int disgustIndex = 3;
const int happyIndex = 4;
const int surpriseIndex = 5;

CRGB leds[NUM_LEDS];
int colorIntensity;

// index order: sadness, anger, fear, disgust, happiness, surprise
int LedStripColorIntensity[] = {-1,-1,-1,-1,-1,-1}; // these values will be averaged to set the intensity of the LED strip
int LedStripColorHue[] = {BLUE,RED,GREEN,PURPLE,YELLOW,ORANGE}; // these values will be averaged to set the intensity of the LED strip

void setup() {
  FastLED.clear ();
    pinMode(sadnessLed, OUTPUT);
    pinMode(sadnessVibratingPin, OUTPUT);
    sadnessForce = 0;

    pinMode(angerLed, OUTPUT);
    pinMode(angerVibratingPin, OUTPUT);
    angerForce = 0;

    digitalWrite(angerLed,HIGH);

    pinMode(fearLed, OUTPUT);
    pinMode(fearVibratingPin, OUTPUT);
    fearForce = 0;

    pinMode(disgustRedLed, OUTPUT);
    pinMode(disgustBlueLed, OUTPUT);
    pinMode(disgustVibratingPin, OUTPUT);
    disgustForce = 0;

    pinMode(happinessRedLed, OUTPUT);
    pinMode(happinessGreenLed, OUTPUT);
    pinMode(happinessVibratingPin, OUTPUT);
    happinessForce = 0;

    pinMode(surpriseRedLed, OUTPUT);
    pinMode(surpriseGreenLed, OUTPUT);
    pinMode(surpriseVibratingPin, OUTPUT);
    surpriseForce = 0;

    FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);

    Serial.begin(9600);     //Begin serial communication
}

void loop() {
    sadnessForce = analogRead(sadPressurePin);
    if(sadnessForce>70){
      colorIntensity = abs((sadnessForce * 255L)/1000L);
      analogWrite(sadnessLed, colorIntensity);
      digitalWrite(sadnessVibratingPin,HIGH);
      LedStripColorIntensity[sadnessIndex] = colorIntensity;

      Serial.println("JUST CHANGED LED STRIPCOLOR: ");
      Serial.print(LedStripColorIntensity[0]);
    } else{
      digitalWrite(sadnessLed, LOW);
      digitalWrite(sadnessVibratingPin,LOW);
      LedStripColorIntensity[sadnessIndex] = -1;
    }

    fearForce = analogRead(fearPressurePin);
    if(fearForce>70){
      colorIntensity = abs((fearForce * 255L)/1000L);
      LedStripColorIntensity[fearIndex] = colorIntensity;
      analogWrite(fearLed, colorIntensity);
      digitalWrite(fearVibratingPin,HIGH);
    } else{
      digitalWrite(fearLed, LOW);
      digitalWrite(fearVibratingPin,LOW);
      LedStripColorIntensity[fearIndex] = -1;
    }

    angerForce = analogRead(angerPressurePin);
    Serial.println("this is anger force:");
    Serial.println(angerForce);
    if(angerForce>70){
      colorIntensity = abs((angerForce * 255L)/1000L);
      analogWrite(angerLed, colorIntensity);
      digitalWrite(angerVibratingPin,HIGH);
      LedStripColorIntensity[angerIndex] = colorIntensity;
    } else{
      digitalWrite(angerLed, LOW);
      LedStripColorIntensity[angerIndex] = -1;
    }

    disgustForce = analogRead(disgustPressurePin);
    if(disgustForce>70){
      colorIntensity = abs((disgustForce * 255L)/1000L);

      analogWrite(disgustRedLed, colorIntensity);
      analogWrite(disgustBlueLed, colorIntensity);
      digitalWrite(disgustVibratingPin,HIGH);

      LedStripColorIntensity[disgustIndex] = colorIntensity;
    } else{
      digitalWrite(disgustRedLed, LOW);
      digitalWrite(disgustBlueLed, LOW);
      digitalWrite(disgustVibratingPin,LOW);
      LedStripColorIntensity[disgustIndex] = -1;
    }

    happinessForce = analogRead(happinessPressurePin);
    if(happinessForce>70){
      digitalWrite(happinessVibratingPin,HIGH);
      colorIntensity = abs((happinessForce * 255L)/1000L);

      analogWrite(happinessRedLed, colorIntensity);
      analogWrite(happinessGreenLed, colorIntensity);
      LedStripColorIntensity[happyIndex] = colorIntensity;
    } else{
      digitalWrite(happinessRedLed, LOW);
      digitalWrite(happinessGreenLed, LOW);
      digitalWrite(happinessVibratingPin,LOW);
      LedStripColorIntensity[happyIndex] = -1;
    }

    surpriseForce = analogRead(surprisePressurePin);
    if(surpriseForce>70){
      digitalWrite(surpriseVibratingPin,HIGH);
      colorIntensity = abs((surpriseForce * 255L)/1000L);

      analogWrite(surpriseRedLed, colorIntensity);
      analogWrite(surpriseGreenLed, colorIntensity);
      LedStripColorIntensity[surpriseIndex] = colorIntensity;
    } else{
      digitalWrite(surpriseRedLed, LOW);
      digitalWrite(surpriseGreenLed, LOW);
      digitalWrite(surpriseVibratingPin,LOW);
      LedStripColorIntensity[surpriseIndex] = -1;
    }
    updateLedStrip();
}

void updateLedStrip(){
  int intensityAverage = 0;
  int intensityTotal = 0;
  int colorAverage = 0;
  int colorTotal = 0;
  int numPressed = 0;

  // 6 is length of the intensity array
  for(int i = 0; i<6;i++){
    if(LedStripColorIntensity[i] != -1){

      intensityTotal+=LedStripColorIntensity[i];
      colorTotal += LedStripColorHue[i];
      numPressed++;
    }
  }
  if(numPressed==0){
    FastLED.clear ();
  }
  else{
    colorAverage = float(colorTotal)/numPressed;
    intensityAverage = float(intensityTotal)/numPressed;

    FastLED.showColor(CHSV(colorAverage, 255, intensityAverage));
    delay(10);
  }
  return;
}
