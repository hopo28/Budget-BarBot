
void pump(int pumpState)
{//assumes only one pump
  switch(pumpState)
  {
    case FWD: /*FORWARD*/ break;
    case REV: /*REVERSE*/ break;
    default: /*STOP*/ break;
  }
}

void valve(int valveNum, boolean state)
{//open is true
  int valveValue = 0;
  if(state)
  {
    valveValue = V_OPEN;
    leds[valveNum] = C_OPEN;
  }
  else
  {
    valveValue = V_CLOSE;
    leds[valveNum] = C_CLOSE;
  }
  
  switch(valveNum)
  {
    case 0: V0.write(valveValue); break;  
    case 1: V1.write(valveValue); break;
    case 2: V2.write(valveValue); break;
    case 3: V3.write(valveValue); break;
    case 4: V4.write(valveValue); break;
    case 5: V5.write(valveValue); break;
    case 6: V6.write(valveValue); break;
    case 7: V7.write(valveValue); break;
    case 8: V8.write(valveValue); break;
    case 9: V9.write(valveValue); break;
  }
}
