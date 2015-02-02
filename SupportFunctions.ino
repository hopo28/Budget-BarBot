
// - - - - - - - - - - - - - - - S E Q U E N C E S - - - - - - - - - - - - - - -
void dispense(int reagent, int ml)
{
  if(DEBUG){Serial.println(ml + "ml dispensing of " + reagent);}
  out(OPEN);  //open out valve
  valve(reagent + 3, OPEN);  //open reagent valve
  pump(ml);  //pump(ml)
  valve(reagent + 3, CLOSE);  //close reagent valve
}

void end()
{
  if(DEBUG){Serial.println("Finishing Sequence...");}
  air(OPEN);  //open air valve
  pump(PIPE_VOLUME);  //pump(pipe volume)
  air(CLOSE);  //close air valve
  out(CLOSE);  //close output valve
}

void clean()
{
  if(DEBUG){Serial.println("Cleaning Pipes...");}
  out(CLOSE);  //close output valve//just to make sure
  dump(OPEN);  //open dump valve
  water(OPEN);  //open water valve
  pump(DUMP_VOLUME);  //pump(dump volume)
  water(CLOSE);  //close water valve
  air(OPEN);  //open air valve
  pump(PIPE_VOLUME);  //pump(pipe volume)
  air(CLOSE);  //close air valve
  dump(CLOSE);  //close dump valve
}

// - - - - - - - - - - - - - - - V A L V E S - - - - - - - - - - - - - - -

void initValves()
{
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

void valve(int valveNum, boolean state)
{//open is true
  int valveValue = 0;
  if(state)//opening
  {
    valveValue = V_OPEN;
    //leds[valveNum] = C_OPEN;//colour done elsewhere
  }
  else
  {
    valveValue = V_CLOSE;
    //leds[valveNum] = C_CLOSE;
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
//takes 350 to 400 to change state - ish moving about 100 degrees 
  if(state)//opening
  {
    for(int i = 0; i <= divs; i++)
    {
      leds[valveNum].r = ((C_CLOSE.r / divs)) * i + ((C_OPEN.r / divs) * (divs / i));
      leds[valveNum].g = ((C_CLOSE.g / divs)) * i + ((C_OPEN.g / divs) * (divs / i));
      leds[valveNum].b = ((C_CLOSE.b / divs)) * i + ((C_OPEN.b / divs) * (divs / i));
      FastLED.show();
      delay(wait);
    }
  }
  else//closing
  {
  for(int i = 0; i < divs; i++)
    {
      leds[valveNum].r = ((C_OPEN.r / divs)) * i + ((C_CLOSE.r / divs) * (divs / i));
      leds[valveNum].g = ((C_OPEN.g / divs)) * i + ((C_CLOSE.g / divs) * (divs / i));
      leds[valveNum].b = ((C_OPEN.b / divs)) * i + ((C_CLOSE.b / divs) * (divs / i));
      FastLED.show();
      delay(wait);
    }
  }
}

// - - - W R A P P E R S - - -

void air(boolean state){valve(PNUM_AIR,state);}
void water(boolean state){valve(PNUM_WATER,state);}
void dump(boolean state){valve(PNUM_DUMP,state);}
void out(boolean state){valve(PNUM_OUT,state);}

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
    case FWD: digitalWrite(P_PIN_A, HIGH); digitalWrite(P_PIN_B, LOW); break;
    case REV: digitalWrite(P_PIN_A, LOW); digitalWrite(P_PIN_B, HIGH); break;
    default: digitalWrite(P_PIN_A, LOW); digitalWrite(P_PIN_B, LOW); break;
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
  runPump(FWD);
  delay(PUMP_COEFICIENT * ml);
  runPump(STOP);
}
