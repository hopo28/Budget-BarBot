#include "FastLED.h"
//example recipe:
//R1010R2020R4030EC
/* - PINS - 
54 digital pins

01 - B
02 - LED
03 - B
04 - B
05 - B
06 - B
07 - B
08 - B
09 - B
10 - SD CS
11 - 
12 - 
13 - 
14 - 
MEGA ONLY:
15 - 
16 - 
17 - 
18 - 
19 - 
20 - LCD SDA
21 - LCD SCL
22 - 
23 - 
24 - 
25 - 
26 - 
27 - 
28 - 
29 - 
30 - V
31 - B
32 - V
33 - B
34 - V
35 - B
36 - V
37 - B
38 - V
39 - 
40 - V
41 - 
42 - V
43 - 
44 - V
45 - 
46 - V
47 - 
48 - V
49 - 
50 - SD MOSI
51 - SD MISO
52 - SD CLK
53 - 
54 - 

16 analogue pins, 8 for NANO
A8 - P_PIN_A
nano - 14+8=22
mega - 54+16=70
teensy++ 2.0 - 46
teensy 3.1 - 34


*/

#define MAX_NUM_COCKTAILS 50
#define MAX_NUM_REAGENTS 20
#define MAX_NUM_MIXERS 10
#define NUM_FEEDS 6//valves-4
#define OPEN true
#define CLOSE false
#define DEBUG true
//LEDS
#define LED_PIN 2
#define NUM_LEDS 11//remember one for the pump
//colour definitions
CRGB C_OPEN = CRGB(000,255,000);//Green
CRGB C_CLOSE = CRGB(255,000,000);//Red
CRGB C_FWD = CRGB(000,255,000);//Green
CRGB C_STOP= CRGB(255,000,000);//Red
CRGB C_REV = CRGB(000,000,255);//Blue
CRGB leds[NUM_LEDS];
int brightness = 16;//was 16

String feeds[NUM_FEEDS];
String cocktails[MAX_NUM_COCKTAILS];//holders for cocktail names
String cocktailFN[MAX_NUM_COCKTAILS];//holders for cocktail file names
int NUM_COCKTAILS = 0;//initial value
String reagents[MAX_NUM_REAGENTS];
int NUM_REAGENTS = 0;//initial value
String mixers[MAX_NUM_MIXERS];
int NUM_MIXERS = 0;//initial value

void setup()
{
  FastLED.addLeds<NEOPIXEL, LED_PIN, GRB>(leds, NUM_LEDS);
  LEDS.setBrightness(brightness);
  initValves();
  initPump();
  initButtons();
  if(DEBUG){Serial.begin(9600);}
  initLCD();
  initSD();

  for(int i = 0; i < NUM_FEEDS; i++)//temp reagent names
  {
    feeds[i] = "RName ";
    feeds[i] += (i+1);
  }
  if(DEBUG){printMenu();}
  /*
  Serial.println(cocktailFN[0]);
  Serial.println(cocktailFN[1]);
  Serial.println(cocktailFN[2]);
  Serial.println(cocktailFN[3]);
  */
   
  LCD_MainMenu(0);//0 means show with default options
}

void loop()
{
  //what happens if you try to close and already closed valve?
  //add in possibility to change brightness levels from serial/SD card/EEPROM??
  
  //SORT BUTTONS - done
  //sort lcd menus - partial, still working on
  //sort actually doing cocktails
  //REM multplex motor control channels
  
  checkMenuButtons();
//  checkButtons();//other buttons
  if(DEBUG){
    if (Serial.available() > 0)
    {
      doMenu(Serial.read());
    }
  }
    
    
  /*
  dispense(squash[0],squash[1]);
  end();
  clean();
  while(true){}//stop here
  */
  
}

