#define NUM_BUTTONS 8
#define NUM_MENU_BUTTONS 4
#define PRESS_SHORT 50//in milliseconds, for debouncing
#define B_NULL 0
#define B_BACK 1
#define B_SELECT 2
#define B_UP 3
#define B_DOWN 4

//2 is used for LEDS

int buttons[NUM_BUTTONS] = {1,3,4,5,6,7,8,9};//pin numbers for buttons
int menuButtons[NUM_MENU_BUTTONS] = {31,33,35,37};//pin numbers for menu buttons (bcak, select, up, down)

unsigned long buttonPressedTimer = 0;
unsigned long menuButtonPressedTimer = 0;
int buttonPressed = 200;
int menuButtonPressed = 200;
boolean buttonPressedFlag = false;
boolean menuButtonPressedFlag = false;

// - - - - - - - - - - - - - - - B U T T O N S - - - - - - - - - - - - - - -

void initButtons()
{
  for(int i = 0; i < NUM_BUTTONS; i++)
  {
    pinMode(buttons[i], INPUT);           // set pin to input
    digitalWrite(buttons[i], HIGH);       // turn on pullup r
  }
  for(int i = 0; i < NUM_MENU_BUTTONS; i++)
  {
    pinMode(menuButtons[i], INPUT);           // set pin to input
    digitalWrite(menuButtons[i], HIGH);       // turn on pullup r
  }
  
  
}


void checkButtons()
{
  for(int i = 0; i < NUM_BUTTONS; i++)//for each input
  {
    if(!digitalRead(buttons[0+i]))//if the one being checked is pressed
    {
      if(buttonPressedFlag == false)//if its first time press
      {
        //Serial.print("Button ");
        //Serial.print(i);
        //Serial.println(" Pressed");
        
        //log details and start time
        buttonPressedFlag = true;
        buttonPressedTimer = millis();
        buttonPressed = i;
      }
      else//(buttonPressedFlag == true)
      {//this means this button has been pressed before and is still being pressed
        //Serial.print(i);
        unsigned long timeTemp = (millis()-buttonPressedTimer);//how long it was pressed for
        if(timeTemp < PRESS_SHORT)
        {
          //Serial.println("INVALID");//do sod all          
        }
        else
        {
          buttonPressedFlag = false;//only do something once
          switch(i)
          {
            case 0: clean(); break;//Clean
            case 1: end(); break;//End
            case 2: dispense(1, 25); break;//R1
            case 3: dispense(2, 25); break;//R2
            case 4: dispense(3, 25); break;//R3
            case 5: dispense(4, 25); break;//R4
            case 6: dispense(5, 25); break;//R5
            case 7: dispense(6, 25); break;//R6
          }
        }
      }
    }
    else//if the one being checked is NOT pressed
    {			
      if((buttonPressed == i) && (buttonPressedFlag == true))//if it WAS pressed
      {
        //Serial.println("unpressed");
        buttonPressedFlag = false;
      }
    }
  }	
}

void checkMenuButtons()
{
  for(int i = 0; i < NUM_MENU_BUTTONS; i++)//for each input
  {
    if(!digitalRead(menuButtons[0+i]))//if the one being checked is pressed
    {
      if(menuButtonPressedFlag == false)//if its first time press
      {
        //Serial.print("Button ");
        //Serial.print(i);
        //Serial.println(" Pressed");
        
        //log details and start time
        menuButtonPressedFlag = true;
        menuButtonPressedTimer = millis();
        menuButtonPressed = i;
      }
      else//(buttonPressedFlag == true)
      {//this means this button has been pressed before and is still being pressed
        //Serial.print(i);
        unsigned long timeTemp = (millis()-menuButtonPressedTimer);//how long it was pressed for
        if(timeTemp < PRESS_SHORT)
        {
          //Serial.println("INVALID");//do sod all          
        }
        else
        {

                  switch(i)
          {
            case 0: doMenuButton(B_BACK); Serial.println("BUTTON_BACK"); break;
            case 1: doMenuButton(B_SELECT); Serial.println("BUTTON_SELECT"); break;
            case 2: doMenuButton(B_UP); Serial.println("BUTTON_UP"); break;
            case 3: doMenuButton(B_DOWN); Serial.println("BUTTON_DOWN"); break;
          }
          menuButtonPressedFlag = false;//only do something once

        }
      }
    }
    else//if the one being checked is NOT pressed
    {			
      if((menuButtonPressed == i) && (menuButtonPressedFlag == true))//if it WAS pressed
      {
        //Serial.println("unpressed");
        menuButtonPressedFlag = false;

      }
    }
  }	
}


