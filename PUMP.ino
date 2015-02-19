//PUMP
#define FWD 1
#define STOP 0
#define REV 2
#define P_PIN_A A8
#define P_PIN_B A9//not needed as wont run backwards
#define PUMP_COEFICIENT 120//pure guess - time it takes to pump 1ml of liquid ###########

// - - - - - - - - - - - - - - - P U M P - - - - - - - - - - - - - - -
void initPump()
{
  pinMode(P_PIN_A, OUTPUT);
  pinMode(P_PIN_B, OUTPUT);
}

//need to check this#################################################################

void runPump(int pumpState)
{//assumes only one pump
  switch(pumpState)
  {
    case FWD: digitalWrite(P_PIN_A, HIGH); digitalWrite(P_PIN_B, LOW); leds[NUM_LEDS-1] = C_FWD; break;
    case REV: digitalWrite(P_PIN_A, LOW); digitalWrite(P_PIN_B, HIGH); leds[NUM_LEDS-1] = C_REV; break;
    default:  digitalWrite(P_PIN_A, LOW); digitalWrite(P_PIN_B, LOW); leds[NUM_LEDS-1] = C_STOP; break;
  }
}

// - - - W R A P P E R S - - -

void unpump(int ml)
{//try to avoid using
  runPump(REV);
  delay(PUMP_COEFICIENT * ml);
  runPump(STOP);
}

void pump(int ml)
{
  if(DEBUG){Serial.print(" Pump(ml) ");}
  if(DEBUG){Serial.println(ml);}
  runPump(FWD);
  delay(PUMP_COEFICIENT * ml);
  runPump(STOP);
}


