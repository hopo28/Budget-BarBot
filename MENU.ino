#include <Wire.h>
#include <LiquidCrystal_I2C.h>//version from http://arduino-info.wikispaces.com/LCD-Blue-I2C

//menus
#define M_MM 0
#define M_CC 1
#define M_SU 2
#define M_FS 3
#define M_CR 4
#define M_CM 5
int selectedMenu = M_MM;
int selectedItem = 1;//will be 1,2,3

LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);  // Set the LCD I2C address

// - - - - - - - - - - - - - - - L C D   M E N U - - - - - - - - - - - - - - -

void initLCD()
{
  lcd.begin(20,4);// initialize the lcd for 20 chars 4 lines and turn on backlight
  lcd.setCursor(0,0);
  lcd.print(" -- Starting  Up -- ");
  delay(500);
}

//REM make sure LEDS flash when reagent selected for modification

void doMenuButton(int button)
{//this is called from the 'buttons' bit
  if(button == B_SELECT)
  {
    Serial.print("Selected Item: ");
    Serial.println(selectedItem);
  }
  
  switch(selectedMenu)
  {
    case M_MM: LCD_MainMenu(button); break;
    case M_CC: LCD_ChooseCocktail(button); break;
    case M_SU: LCD_SetUp(button); break;
    case M_FS: LCD_FeedStar(button); break;
    case M_CR: LCD_ChooseReagent(button); break;
    case M_CM: LCD_ChooseMixer(button); break;
  }
}

void LCD_MainMenu(int button)
{
  /*
  Main Menu
  -Choose Cocktail
  -Clean
  -Set Up
  */
  selectedMenu = M_MM;//we are here
  //draw menu
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(" - Hopo's  BarBot - ");
  lcd.setCursor(0,1);
  lcd.print("  Choose a Cocktail");
  lcd.setCursor(0,2);
  lcd.print("  Clean");
  lcd.setCursor(0,3);
  lcd.print("  Set-Up Feeds");
  
  switch(button)
  {
    case B_NULL:
      selectedItem = 1;//always default to cocktail
      break;
    case B_UP:
      selectedItem--;
      if(selectedItem == 0)
        selectedItem = 1;
      break;
    case B_DOWN:
      selectedItem++;
      if(selectedItem == 4)
        selectedItem = 3;
      break;
    case B_SELECT:
      if(selectedItem == 1)//choose cocktail
        LCD_ChooseCocktail(B_NULL);
      if(selectedItem == 2)//clean
        clean();
      if(selectedItem == 3)//setup
        LCD_SetUp(B_NULL);
      break;
    case B_BACK: /*DO NOTHING*/ break;
  }

  lcd.setCursor(0,selectedItem);
  lcd.print(">");
}

int cocktailScrollValue = 0;//what number cocktail is at the top of the list

void LCD_ChooseCocktail(int button)
{//displays cocktail list
  /*
  Choose Cocktail
  -Cocktail 1
  -Cocktail 2
  -Cocktail 3
  -Cocktail n
  */
  selectedMenu = M_CC;//we are here
  lcd.clear();
    //draw menu
  lcd.setCursor(0,0);
  lcd.print(" -Choose  Cocktail- ");
          //"-----20---Chars-----"
  lcd.setCursor(0,1); lcd.print(" "); lcd.print(cocktails[cocktailScrollValue + 0]);
  lcd.setCursor(0,2); lcd.print(" "); lcd.print(cocktails[cocktailScrollValue + 1]);
  lcd.setCursor(0,3); lcd.print(" "); lcd.print(cocktails[cocktailScrollValue + 2]);

  switch(button)
  {
    case B_NULL:
      selectedItem = 1;//always default
      cocktailScrollValue = 0; //reset scrolling to top
      break;
    case B_UP:
      selectedItem--;
      if(selectedItem == 0)// - - - - - - NEED TO HANDLE SCROLLING HERE - - - - -
      {
        selectedItem = 1;
        cocktailScrollValue--;//unless reach beginnning of list
        if(cocktailScrollValue < 0)
          cocktailScrollValue = 0;//dont let it drop below 0
        //redraw
        lcdClearMenuItems();
        lcd.setCursor(0,1); lcd.print(" "); lcd.print(cocktails[cocktailScrollValue + 0]);
        lcd.setCursor(0,2); lcd.print(" "); lcd.print(cocktails[cocktailScrollValue + 1]);
        lcd.setCursor(0,3); lcd.print(" "); lcd.print(cocktails[cocktailScrollValue + 2]);

      }
      break;
    case B_DOWN:
      selectedItem++;
      if(selectedItem == 4)
      {
        selectedItem = 3;
        cocktailScrollValue++;//unless reach end of list
        if(cocktailScrollValue > NUM_COCKTAILS-3)
          cocktailScrollValue = NUM_COCKTAILS-3;//dont let it go above max
        //redraw
        lcdClearMenuItems();
        lcd.setCursor(0,1); lcd.print(" "); lcd.print(cocktails[cocktailScrollValue + 0]);
        lcd.setCursor(0,2); lcd.print(" "); lcd.print(cocktails[cocktailScrollValue + 1]);
        lcd.setCursor(0,3); lcd.print(" "); lcd.print(cocktails[cocktailScrollValue + 2]);
      }
      break;
    case B_SELECT:
      if(selectedItem == 1)
      {
        doCocktail(cocktailScrollValue+0);
        LCD_MainMenu(B_NULL);
      }
      if(selectedItem == 2)
      {
        doCocktail(cocktailScrollValue+1);
        LCD_MainMenu(B_NULL);
      }
      if(selectedItem == 3)
      {
        doCocktail(cocktailScrollValue+2);
        LCD_MainMenu(B_NULL);
      }
      break;
    case B_BACK: LCD_MainMenu(B_NULL); break;//back up
  }
  lcd.setCursor(0,selectedItem);
  lcd.print(">");
}

int feedScrollValue = 0;//what number feed is at the top of the list
int selectedFeed = 0;//what feed is curently being edited

void LCD_SetUp(int button)
{
  /*
  Set Up
  -Feed 1
  -Feed 2
  -Feed 3
  -Feed 4
  -Feed 5
  -Feed 6
  */
  
  selectedMenu = M_SU;//we are here
    //draw menu
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(" -  Set-Up Feeds  - ");
//lcd.print(" -     Set-Up     - ");
          //"-----20---Chars-----"
  lcd.setCursor(0,1); lcd.print(" F"); lcd.print(feedScrollValue+1); lcd.print("-"); lcd.print(feeds[0+feedScrollValue]);
  lcd.setCursor(0,2); lcd.print(" F"); lcd.print(feedScrollValue+2); lcd.print("-"); lcd.print(feeds[1+feedScrollValue]);
  lcd.setCursor(0,3); lcd.print(" F"); lcd.print(feedScrollValue+3); lcd.print("-"); lcd.print(feeds[2+feedScrollValue]);  
  switch(button)
  {
    case B_NULL:
      selectedItem = 1;//always default
      break;
    case B_UP:
      selectedItem--;
      if(selectedItem == 0)
      {
        selectedItem = 1;
        feedScrollValue--;//unless reach beginnning of list
        if(feedScrollValue < 0)
          feedScrollValue = 0;//dont let it drop below 0
        //redraw    
        lcdClearMenuItems();
        lcd.setCursor(0,1); lcd.print(" F"); lcd.print(feedScrollValue+1); lcd.print("-"); lcd.print(feeds[0+feedScrollValue]);
        lcd.setCursor(0,2); lcd.print(" F"); lcd.print(feedScrollValue+2); lcd.print("-"); lcd.print(feeds[1+feedScrollValue]);
        lcd.setCursor(0,3); lcd.print(" F"); lcd.print(feedScrollValue+3); lcd.print("-"); lcd.print(feeds[2+feedScrollValue]);
      }  
      break;
    case B_DOWN:
      selectedItem++;
      if(selectedItem == 4)
      {
        selectedItem = 3;
        feedScrollValue++;//unless reach end of list
        if(feedScrollValue > NUM_FEEDS-3)
          feedScrollValue = NUM_FEEDS-3;//dont let it go above max
        //redraw    
        lcdClearMenuItems();
        lcd.setCursor(0,1); lcd.print(" F"); lcd.print(feedScrollValue+1); lcd.print("-"); lcd.print(feeds[0+feedScrollValue]);
        lcd.setCursor(0,2); lcd.print(" F"); lcd.print(feedScrollValue+2); lcd.print("-"); lcd.print(feeds[1+feedScrollValue]);
        lcd.setCursor(0,3); lcd.print(" F"); lcd.print(feedScrollValue+3); lcd.print("-"); lcd.print(feeds[2+feedScrollValue]);  
      }
      break;
    case B_SELECT:
      if(selectedItem == 1)
      {
        selectedFeed = feedScrollValue + 0;
        LCD_FeedStar(B_NULL);
      }
      if(selectedItem == 2)
      {
        selectedFeed = feedScrollValue + 1;
        LCD_FeedStar(B_NULL);
      }
      if(selectedItem == 3)
      {
        selectedFeed = feedScrollValue + 2;
        LCD_FeedStar(B_NULL);
      }
      break;
    case B_BACK: LCD_MainMenu(B_NULL); break;
  }


  lcd.setCursor(0,selectedItem);
  lcd.print(">");
}

void LCD_FeedStar(int button)//not done yet
{
  /*
  F* [NAME]
  -Choose Reagent
  -Choose Name
  -Purge 10ml - check this quantities in testing stage
  */
  leds[selectedFeed+4] = CRGB::Blue;
  FastLED.show();
  selectedMenu = M_FS;//we are here
   lcd.clear();
     //draw menu
  lcd.setCursor(0,0);
  lcd.print(" - F");
  lcd.print(selectedFeed+1);
  lcd.print(" ");
  String trimmedName = feeds[selectedFeed];
  trimmedName.remove(11,(trimmedName.length()-11));//remove extra stuff
  lcd.print(trimmedName);
  lcd.setCursor(18,0);
  lcd.print("- ");
          //"-----20---Chars-----"
  lcd.setCursor(0,1); lcd.print("  Choose Reagent  OR");
  lcd.setCursor(0,2); lcd.print("  Choose Mixer");
//  lcd.setCursor(0,3); lcd.print("  Purge 10ml");//not needed atm
  
  switch(button)
  {
    case B_NULL:
      selectedItem = 1;//always default
      break;
    case B_UP:
      selectedItem--;
      if(selectedItem == 0)
        selectedItem = 1;
      break;
    case B_DOWN:
      selectedItem++;
      if(selectedItem == 3)//DIFFERENT AS THERE IS NO 3RD OPTION
        selectedItem = 2;
      break;
    case B_SELECT:
      if(selectedItem == 1)
        LCD_ChooseReagent(B_NULL);
      if(selectedItem == 2)
        LCD_ChooseMixer(B_NULL);
//      if(selectedItem == 3)
//        dispense(selectedFeed+1,10);
      break;
    case B_BACK: leds[selectedFeed+4] = C_CLOSE; FastLED.show(); LCD_SetUp(B_NULL); break;
  }
  lcd.setCursor(0,selectedItem);
  lcd.print(">");
}

int reagentScrollValue = 0;

void LCD_ChooseReagent(int button)//not done yet
{
  /*
  Choose Type
  -Reagent
  -Mixer
  */
  selectedMenu = M_CR;//we are here
   lcd.clear();
     //draw menu
  lcd.setCursor(0,0);
  lcd.print("-Choose Reagent(F");
  lcd.print(selectedFeed+1);
  lcd.print(")-");
          //"-----20---Chars-----"
  lcd.setCursor(0,1); lcd.print(" "); lcd.print(reagents[reagentScrollValue + 0]);
  lcd.setCursor(0,2); lcd.print(" "); lcd.print(reagents[reagentScrollValue + 1]);
  lcd.setCursor(0,3); lcd.print(" "); lcd.print(reagents[reagentScrollValue + 2]);
  switch(button)
  {
    case B_NULL:
      selectedItem = 1;//always default
      break;
    case B_UP:
      selectedItem--;
      if(selectedItem == 0)
      {
        selectedItem = 1;
        reagentScrollValue--;//unless reach beginnning of list
        if(reagentScrollValue < 0)
          reagentScrollValue = 0;//dont let it drop below 0
        //redraw    
        lcdClearMenuItems();
        lcd.setCursor(0,1); lcd.print(" "); lcd.print(reagents[reagentScrollValue + 0]);
        lcd.setCursor(0,2); lcd.print(" "); lcd.print(reagents[reagentScrollValue + 1]);
        lcd.setCursor(0,3); lcd.print(" "); lcd.print(reagents[reagentScrollValue + 2]);
      }  
      break;
    case B_DOWN:
      selectedItem++;
      if(selectedItem == 4)
      {
        selectedItem = 3;
        reagentScrollValue++;//unless reach end of list
        if(reagentScrollValue > NUM_REAGENTS-3)
          reagentScrollValue = NUM_REAGENTS-3;//dont let it go above max
        //redraw    
        lcdClearMenuItems();
        lcd.setCursor(0,1); lcd.print(" "); lcd.print(reagents[reagentScrollValue + 0]);
        lcd.setCursor(0,2); lcd.print(" "); lcd.print(reagents[reagentScrollValue + 1]);
        lcd.setCursor(0,3); lcd.print(" "); lcd.print(reagents[reagentScrollValue + 2]);
      }
      break;
    case B_SELECT:
      if(selectedItem == 1)
      {
        feeds[selectedFeed] = reagents[reagentScrollValue + 0];
        LCD_SetUp(B_NULL);
      }  
      if(selectedItem == 2)
      {
        feeds[selectedFeed] = reagents[reagentScrollValue + 1];
        LCD_SetUp(B_NULL);
      }
      if(selectedItem == 3)
      {
        feeds[selectedFeed] = reagents[reagentScrollValue + 2];
        LCD_SetUp(B_NULL);
      }
      break;
    case B_BACK: LCD_FeedStar(B_NULL); break;
  }
  lcd.setCursor(0,selectedItem);
  lcd.print(">");
}


int mixerScrollValue = 0;

void LCD_ChooseMixer(int button)//not done yet
{
  selectedMenu = M_CM;//we are here
  lcd.clear();
  //draw menu
  lcd.setCursor(0,0);
  lcd.print(" -Choose Mixer(F");
  lcd.print(selectedFeed+1);
  lcd.print(")- ");
          //"-----20---Chars-----"
  lcd.setCursor(0,1); lcd.print(" "); lcd.print(mixers[mixerScrollValue + 0]);
  lcd.setCursor(0,2); lcd.print(" "); lcd.print(mixers[mixerScrollValue + 1]);
  lcd.setCursor(0,3); lcd.print(" "); lcd.print(mixers[mixerScrollValue + 2]);
  switch(button)
  {
    case B_NULL:
      selectedItem = 1;//always default
      break;
    case B_UP:
      selectedItem--;
      if(selectedItem == 0)
      {
        selectedItem = 1;
        mixerScrollValue--;//unless reach beginnning of list
        if(mixerScrollValue < 0)
          mixerScrollValue = 0;//dont let it drop below 0
        //redraw    
        lcdClearMenuItems();
        lcd.setCursor(0,1); lcd.print(" "); lcd.print(mixers[mixerScrollValue + 0]);
        lcd.setCursor(0,2); lcd.print(" "); lcd.print(mixers[mixerScrollValue + 1]);
        lcd.setCursor(0,3); lcd.print(" "); lcd.print(mixers[mixerScrollValue + 2]);
      }  
      break;
    case B_DOWN:
      selectedItem++;
      if(selectedItem == 4)
      {
        selectedItem = 3;
        mixerScrollValue++;//unless reach end of list
        if(mixerScrollValue > NUM_MIXERS-3)
          mixerScrollValue = NUM_MIXERS-3;//dont let it go above max
        //redraw    
        lcdClearMenuItems();
        lcd.setCursor(0,1); lcd.print(" "); lcd.print(mixers[mixerScrollValue + 0]);
        lcd.setCursor(0,2); lcd.print(" "); lcd.print(mixers[mixerScrollValue + 1]);
        lcd.setCursor(0,3); lcd.print(" "); lcd.print(mixers[mixerScrollValue + 2]);
      }
      break;
    case B_SELECT:
      if(selectedItem == 1)
      {
        feeds[selectedFeed] = mixers[mixerScrollValue + 0];
        LCD_SetUp(B_NULL);
      }  
      if(selectedItem == 2)
      {
        feeds[selectedFeed] = mixers[mixerScrollValue + 1];
        LCD_SetUp(B_NULL);
      }
      if(selectedItem == 3)
      {
        feeds[selectedFeed] = mixers[mixerScrollValue + 2];
        LCD_SetUp(B_NULL);
      }
      break;
    case B_BACK: LCD_FeedStar(B_NULL); break;
  }
  lcd.setCursor(0,selectedItem);
  lcd.print(">");
}



// - - - U T I L S - - -
/*
LiquidCrystal()

begin()
clear()
home()
setCursor()
write()
print()
cursor()
noCursor()
blink()
noBlink()
display()
noDisplay()
scrollDisplayLeft()
scrollDisplayRight()
autoscroll()
noAutoscroll()
leftToRight()
rightToLeft()
createChar()
backlight();
noBacklight();
*/

void LCD_RemoveSelection()
{
  lcd.setCursor(0,0);
  lcd.print(" ");
  lcd.setCursor(0,1);
  lcd.print(" ");
  lcd.setCursor(0,2);
  lcd.print(" ");
  lcd.setCursor(0,3);
  lcd.print(" ");
}

void LCD_ShowSelection()
{
  lcd.setCursor(0,selectedItem);
  lcd.print(">");
}

void lcdClearMenuItems()
{//clear last 3 lines
  lcd.setCursor(0,1);
  lcd.print("                    ");
  lcd.setCursor(0,2);
  lcd.print("                    ");
  lcd.setCursor(0,3);
  lcd.print("                    ");
}





// - - - - - - - - - - - - - - - S E R I A L   M E N U - - - - - - - - - - - - - - -
//allow typing of commands
//"R 2 50" means reagent 2, 50 ml
//similarly A,W,D,O eg "A 0 50" would pump 50ml of air

void printMenu()
{
  Serial.println(" -- Hopo's Budget BarBot -- ");
  //Serial.print(" Brightness: ");
  //if(LEDBrightness == 000)
  //  Serial.println("AUTO");
  //else
  //  Serial.println(LEDBrightness);
  Serial.println(" - MENU - ");
  Serial.println(" [Item]: R=Reagent, E=End, C=Clean");
  Serial.println(" - ITEM:");
}

void doMenu(char selection)
{
  switch(selection)
  {
    case 'A'://air
    case 'W'://water
    case 'D'://dump
    case 'O'://output
      selectVolume(selection, 0);
	  break;
    case 'E': 
      Serial.println("End Sequence"); 
        end();
      break;
    case 'C':
      Serial.println("Clean Sequence"); 
        clean();
      break;
    case 'R': selectReagentMenu(); break;//Reagent
    default: Serial.println("Unknown command"); printMenu(); break;
  }
}

void selectReagentMenu()
{
  Serial.println(" - PLEASE ENTER REAGENT NUMBER - (1-6)");
  selectVolume('R', get1DigitFromSerial());
}

void selectVolume(char item, int value)
{//value only valid if its a reagent
  Serial.println(" - PLEASE ENTER VOLUME (ml) - (001-255)");
  int ml = get3DigitFromSerial();

  if(DEBUG){Serial.print(" ");}
  if(DEBUG){Serial.print(item);}
  if(DEBUG){Serial.print(" ");}
  if(DEBUG){Serial.print(value);}
  if(DEBUG){Serial.print(" ");}
  if(DEBUG){Serial.println(ml);}
  
//  if(Serial.read() == 'Y')
  {
    switch(item)
    {
//      case 'A': air(OPEN);   delay(PUMP_COEFICIENT * ml); air(CLOSE);   break;
//      case 'W': water(OPEN); delay(PUMP_COEFICIENT * ml); water(CLOSE); break;
//      case 'D': dump(OPEN);  delay(PUMP_COEFICIENT * ml); dump(CLOSE);  break;
//      case 'O': out(OPEN);   delay(PUMP_COEFICIENT * ml); out(CLOSE);   break;
      case 'R': dispense(value, ml); break;
      default: Serial.println("Unknown command"); break;
    }
  }
}

int getCharFromSerial()
{
  char retVal = 'X';
  while (!(Serial.available() > 0)); {retVal = Serial.read(); }
  return retVal;
}

int get1DigitFromSerial()
{
  int retVal = 0;
  while (!(Serial.available() > 0)); { retVal += (Serial.read() - '0'); }
  return retVal;
}

int get2DigitFromSerial()
{
  int retVal = 0;
  while (!(Serial.available() > 0)); { retVal += (10 * (Serial.read() - '0')); }
  while (!(Serial.available() > 0)); { retVal += (Serial.read() - '0'); }
  return retVal;
}

int get3DigitFromSerial()
{
  int retVal = 0;
  while (!(Serial.available() > 0)); { retVal += (100 * (Serial.read() - '0')); }
  while (!(Serial.available() > 0)); { retVal += (10 * (Serial.read() - '0')); }
  while (!(Serial.available() > 0)); { retVal += (Serial.read() - '0'); }
  return retVal;
}

