#include <Servo.h> 
#include "FastLED.h"

#define LED_PIN 13
#define NUM_VALVES 10//nano has a limit of 12, mega has a limit of 48
#define NUM_LEDS NUM_VALVES+1
#define V_OPEN 20
#define V_CLOSE 130
#define OPEN true
#define CLOSE false
#define FWD 1
#define STOP 0
#define REV 2
#define P_DIR_PIN A1
#define P_PWM_PIN A2

//colour definitions
CRGB C_OPEN = CRGB::Green;
CRGB C_CLOSE = CRGB::Red;
CRGB C_FWD = CRGB::Green;
CRGB C_STOP= CRGB::Red;
CRGB C_REV = CRGB::Blue;

CRGB leds[NUM_LEDS];
Servo V0,V1,V2,V3,V4,V5,V6,V7,V8,V9; //valves

unsigned char V_PIN[NUM_VALVES] = {2,3,4,5,6,7,8,9,10,11};//servo pins

void setup()
{
  FastLED.addLeds<NEOPIXEL, LED_PIN, GRB>(leds, NUM_LEDS);
  V0.attach(V_PIN[0]);
  V1.attach(V_PIN[1]);
  V2.attach(V_PIN[2]);
  V3.attach(V_PIN[3]);
  V4.attach(V_PIN[4]);
  V5.attach(V_PIN[5]);
  V6.attach(V_PIN[6]);
  V7.attach(V_PIN[7]);
  V8.attach(V_PIN[8]);
  V9.attach(V_PIN[9]);
}

void loop()
{
  valve(0,CLOSE);
  delay(1000);
  valve(0,OPEN);
  delay(1000);
}


