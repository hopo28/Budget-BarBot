#include <Servo.h> 

//VALVES
#define NUM_VALVES 10//nano has a limit of 12, mega has a limit of 48
#define V_NUM_AIR 3
#define V_NUM_WATER 2
#define V_NUM_DUMP 1
#define V_NUM_OUT 0
#define V_OPEN 20 //degree settings for the servos
#define V_CLOSE 120

Servo V0,V1,V2,V3,V4,V5,V6,V7,V8,V9; //create servo objects
unsigned char V_PIN[NUM_VALVES] = {30,32,34,36,38,40,42,44,46,48};//servo pins

// - - - - - - - - - - - - - - - V A L V E S - - - - - - - - - - - - - - -

void initValves()
{
//set to outputs,             attach to pins,       close all valves
  pinMode(V_PIN[0], OUTPUT);  V0.attach(V_PIN[0]);  valve(0, CLOSE);
  pinMode(V_PIN[1], OUTPUT);  V1.attach(V_PIN[1]);  valve(1, CLOSE);
  pinMode(V_PIN[2], OUTPUT);  V2.attach(V_PIN[2]);  valve(2, CLOSE);
  pinMode(V_PIN[3], OUTPUT);  V3.attach(V_PIN[3]);  valve(3, CLOSE);
  pinMode(V_PIN[4], OUTPUT);  V4.attach(V_PIN[4]);  valve(4, CLOSE);
  pinMode(V_PIN[5], OUTPUT);  V5.attach(V_PIN[5]);  valve(5, CLOSE);
  pinMode(V_PIN[6], OUTPUT);  V6.attach(V_PIN[6]);  valve(6, CLOSE);
  pinMode(V_PIN[7], OUTPUT);  V7.attach(V_PIN[7]);  valve(7, CLOSE);
  pinMode(V_PIN[8], OUTPUT);  V8.attach(V_PIN[8]);  valve(8, CLOSE);
  pinMode(V_PIN[9], OUTPUT);  V9.attach(V_PIN[9]);  valve(9, CLOSE);
}

void valve(int valveNum, boolean state)
{//open is true
  int valveValue = 0;
  if(state)//opening
  {
    valveValue = V_OPEN;
    //leds[valveNum] = C_OPEN;//colour done elsewhere
    if(DEBUG){Serial.print(" Open ");}
    if(DEBUG){Serial.println(valveNum);}
  }
  else
  {
    valveValue = V_CLOSE;
    //leds[valveNum] = C_CLOSE;
    if(DEBUG){Serial.print(" Close ");}
    if(DEBUG){Serial.println(valveNum);}
  }
  int currentVal = 0;
  switch(valveNum)
  {
    case 0: currentVal = V0.read(); V0.write(valveValue); break;
    case 1: currentVal = V1.read(); V1.write(valveValue); break;
    case 2: currentVal = V2.read(); V2.write(valveValue); break;
    case 3: currentVal = V3.read(); V3.write(valveValue); break;
    case 4: currentVal = V4.read(); V4.write(valveValue); break;
    case 5: currentVal = V5.read(); V5.write(valveValue); break;
    case 6: currentVal = V6.read(); V6.write(valveValue); break;
    case 7: currentVal = V7.read(); V7.write(valveValue); break;
    case 8: currentVal = V8.read(); V8.write(valveValue); break;
    case 9: currentVal = V9.read(); V9.write(valveValue); break;
  }
  const int divs = 16;
  signed int result = currentVal - valveValue;
  if(result < 0)
    result = -result;
  const int wait = result/5;
//takes 350 to 400 to change, moving about 100 degrees 
  if(state)//opening
  {
    for(int i = 0; i <= divs; i++)
    {
      leds[valveNum].r = ((C_OPEN.r / divs)) * i + ((C_CLOSE.r / divs) * (divs / i));
      leds[valveNum].g = ((C_OPEN.g / divs)) * i + ((C_CLOSE.g / divs) * (divs / i));
      leds[valveNum].b = ((C_OPEN.b / divs)) * i + ((C_CLOSE.b / divs) * (divs / i));
      FastLED.show();
      delay(wait);
    }
  }
  else//closing
  {
  for(int i = 0; i < divs; i++)
    {
      leds[valveNum].r = ((C_CLOSE.r / divs)) * i + ((C_OPEN.r / divs) * (divs / i));
      leds[valveNum].g = ((C_CLOSE.g / divs)) * i + ((C_OPEN.g / divs) * (divs / i));
      leds[valveNum].b = ((C_CLOSE.b / divs)) * i + ((C_OPEN.b / divs) * (divs / i));
      FastLED.show();
      delay(wait);
    }
  }
}

// - - - W R A P P E R S - - -
void air(boolean state){valve(V_NUM_AIR,state);}
void water(boolean state){valve(V_NUM_WATER,state);}
void dump(boolean state){valve(V_NUM_DUMP,state);}
void out(boolean state){valve(V_NUM_OUT,state);}
