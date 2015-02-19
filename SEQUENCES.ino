
#define PIPE_VOLUME 20//in ml - CHECK THIS ###################
#define DUMP_VOLUME 20//in ml - CHECK THIS ###################

// - - - - - - - - - - - - - - - S E Q U E N C E S - - - - - - - - - - - - - - -

void doCocktail(int number)
{//gets details of cocktail and orchestrates 'pouring'
  String name = cocktails[number];
  Serial.print("DoCocktail: ");
  Serial.print(name);
  Serial.print(" from file: ");
  Serial.println(cocktailFN[number]);
  char cFN[cocktailFN[number].length()+1];
  cocktailFN[number].toCharArray(cFN,cocktailFN[number].length()+1);
  
  
  Serial.print ("FN: ");
  Serial.println(cocktailFN[number]);
  Serial.print ("cFN: ");
  Serial.println(cFN);
  
  
  
  
  boolean ok = checkIngredients(number);
  
  
  //check lists
  if(ok)
  {
    File cocktail =  SD.open(cFN);
    String reagent;
    String qty;
    int requiredFeed;
    
    //read line of cocktail - actually DO COCKTAIL
    while(cocktail.available())
    {
      //read line in
      reagent = cocktail.readStringUntil(' ');
      qty = cocktail.readStringUntil('\n');
      //tidy up
      reagent.trim();
      qty.trim();
      qty.remove(0,1);//remove 1 char starting at index 0, remove the dash
      
      if(reagent.substring(0,2) == "END")//we have finished
      {
        break;//pop out
      }
      else
      {
        //identify reagent/mixer from line
        //look for r/m in feed name list
        requiredFeed = 200;
        for(int i = 0; i < NUM_FEEDS; i++)
        {
          if(reagent == feeds[i])
            requiredFeed = i;
        }
        if(qty.substring(0,6) == "ToTaste")//if not on the pumps and is a totaste
        {
          //display totatste message
          Serial.println("totatse message");
        }
        else if(requiredFeed != 200)//then we have a feed
        {
          //dispense
          Serial.println("dispense here");
        }
        else//eg reqfeed == 200
        {
          //display qty message
          Serial.println("add in x qty of y message");
        }
      }
    }
    //Done message(enjoy your cocktail), poss flash backlight/leds
    Serial.println("done pouring, enjoy");// - crashes hereafter-----------------------
    cocktail.close();
  }
  else
  {
    //'not all ingredients there' message
              Serial.println("missing reagents x,y,z");
  }

  //always end by returning to menu
  LCD_MainMenu(B_NULL);
}


boolean checkIngredients(int number)
{
  char cFN[cocktailFN[number].length()+1];
  cocktailFN[number].toCharArray(cFN,cocktailFN[number].length()+1);
  File recipe =  SD.open(cFN);
  String reagent = "fudge";
  String qty;
  boolean retVal = true;//assume all ok until something missing
  
  Serial.print ("FN: ");
  Serial.println(cocktailFN[number]);
  Serial.print ("cFN: ");
  Serial.println(cFN);
  Serial.print ("name: ");
  String tempName = "\0";//null it
          tempName = recipe.readStringUntil('\n');//this does not work!!!
          
  Serial.println(tempName);
  
  
//  Serial.println(recipe.readStringUntil('\n'));//get rid of the title
  
  while(reagent != "END")  //for every line until END
  {
    boolean error = false;
    reagent = recipe.readStringUntil(' ');
    Serial.println(reagent);
    qty = recipe.readStringUntil('\n');//dont really need for now
    reagent.trim();
    
    //is reagent in reagentList
    boolean reagentFound = false;
    for(int i = 0; i < NUM_REAGENTS; i++)
    {
      if(reagents[i] == reagent)
        reagentFound = true;
    }
    
    //is reagent in mixer list
    boolean mixerFound = false;
    for(int i = 0; i < NUM_MIXERS; i++)
    {
      if(mixers[i] == reagent)
        mixerFound = true;
    }
    
    if(!(mixerFound || reagentFound))//if either neither found or both found
    {
      retVal = false;//we are missing something
      Serial.print("Missing Reagent: ");//rem do this for LCD as well###
      Serial.println(reagent);
    }
  }
//  retVal=true;//testing
  return retVal;
}





void dispense(int reagent, int ml)
{
  if(DEBUG){Serial.print(" Dispense ");}
  if(DEBUG){Serial.print(ml);}
  if(DEBUG){Serial.print("ml of ");}
  if(DEBUG){Serial.println(feeds[reagent-1]);}
  
  lcd.setCursor(0,1);
  lcd.print("Dispense ");
  lcd.print(ml);
  lcd.print("ml of ");
  lcd.print(feeds[reagent-1]);
  
  out(OPEN);
  valve(reagent + 3, OPEN);//+3 to get the valve number
  pump(ml);
  valve(reagent + 3, CLOSE);
  
  lcd.setCursor(0,1);
  lcd.print("                    ");//blank-it
}

void end()
{
  if(DEBUG){Serial.println("End...");}
  
  lcd.setCursor(0,1);
  lcd.print("Finish up...");
  
  out(OPEN);  //just in case
  air(OPEN);
  pump(PIPE_VOLUME);
  air(CLOSE);
  out(CLOSE);
  
  lcd.setCursor(0,1);
  lcd.print("                    ");//blank-it
}

void clean()
{
  if(DEBUG){Serial.println("Cleaning...");}
  
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(" ---- CLEANING ---- ");
          //"-----20---Chars-----"
  out(CLOSE);  //just to make sure
  dump(OPEN);
  water(OPEN);
  pump(DUMP_VOLUME);
  water(CLOSE);
  air(OPEN);
  pump(PIPE_VOLUME);
  air(CLOSE);
  dump(CLOSE);
  
  LCD_MainMenu(B_NULL);
}

