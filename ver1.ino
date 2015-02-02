#include <Servo.h> 
#include "FastLED.h"

#define LED_PIN 2
#define NUM_VALVES 10//nano has a limit of 12, mega has a limit of 48
#define NUM_LEDS NUM_VALVES+1
#define V_OPEN 20
#define V_CLOSE 120
#define OPEN true
#define CLOSE false
#define FWD 1
#define STOP 0
#define REV 2
#define P_PIN_A A1
#define P_PIN_B A2
#define PNUM_AIR 3
#define PNUM_WATER 2
#define PNUM_DUMP 1
#define PNUM_OUT 0
//reagents are 4 onwards
#define PIPE_VOLUME 20//in ml
#define DUMP_VOLUME 20//in ml
#define PUMP_COEFICIENT 120//pure guess - time it takes to pump 1ml of liquid
#define DEBUG true

//colour definitions
CRGB C_OPEN = CRGB(000,255,000);//Green
CRGB C_CLOSE = CRGB(255,000,000);//Red
CRGB C_FWD = CRGB(000,255,000);//Green
CRGB C_STOP= CRGB(255,000,000);//Red
CRGB C_REV = CRGB(000,000,255);//Blue

CRGB leds[NUM_LEDS];

Servo V0,V1,V2,V3,V4,V5,V6,V7,V8,V9; //create servo objects
unsigned char V_PIN[NUM_VALVES] = {30,32,34,36,38,40,42,44,46,48};//servo pins

int brightness = 16;//was 16

//static recipies for now (reagent, ml)
unsigned char squash[] = {1,42};

void setup()
{
  FastLED.addLeds<NEOPIXEL, LED_PIN, GRB>(leds, NUM_LEDS);
  LEDS.setBrightness(brightness);
  initValves();
  initPump();
  if(DEBUG){Serial.begin(9600);}
}

void loop()
{
  //what happens if you try to close and already closed valve?
  
  //reagents 1,2,3,4,5,6
  dispense(squash[0],squash[1]);
  end();
  clean();
  
// - - - - - TESTING - - - - 

  //just open and close valve 0 for all eternity
  valve(0,CLOSE);
    //delay(500);
  valve(1,CLOSE);
    //delay(100);
  valve(2,CLOSE);
    //delay(100);
  valve(3,CLOSE);
    //delay(100);
  
  //valve(4,CLOSE);
  //valve(5,CLOSE);
  //valve(6,CLOSE);
  //valve(7,CLOSE);
  //valve(8,CLOSE);
  //valve(9,CLOSE);
  //delay(3000);
  valve(0,OPEN);
   // delay(500);
  valve(1,OPEN);
    //delay(100);
  valve(2,OPEN);
    //delay(100);
  valve(3,OPEN);
   // delay(100);
    
  //valve(4,OPEN);
  //valve(5,OPEN);
  //valve(6,OPEN);
  //valve(7,OPEN);
  //valve(8,OPEN);
  //valve(9,OPEN);
  //delay(3000);
  
/*
//
V0.write(180);  
V1.write(180);  
V2.write(180);  
V3.write(180);  
delay(1000);  
V0.write(0);  
V1.write(0);  
V2.write(0);  
V3.write(0);  
delay(1000);  
*/
}

