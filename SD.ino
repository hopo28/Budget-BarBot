/* SD card attached to SPI bus as follows:
 ** MOSI - pin 11 (MEGA 50)
 ** MISO - pin 12 (MEGA 51)
 ** CLK - pin 13 (MEGA 52)
 ** CS - pin CUSTOM */

#include <SD.h>
//#include <SdFat.h>//supports long file names (https://github.com/greiman/SdFat-beta)
#include <SPI.h>
 
#define SD_CS_PIN 10



char FN_reagent[]="Reagents.txt";
char FN_mixer[]="Mixers.txt";

//do i need these???
File myFile;
File root;




void initSD()
{
  pinMode(SD_CS_PIN, OUTPUT);//this is needed to make SPI work properly
  if(!SD.begin(SD_CS_PIN)){Serial.println("SD Card Damaged or Missing!");}
//  else{Serial.println("SD Card OK");}
  //check required files
 /*
  myFile = sd.open(FN_reagent);
  if(!myFile)
  {
    lcd.setCursor(0,1);//line 2
    lcd.print("Reagent File Won't Open");
    Serial.println("Reagent File Won't Open");
  }
  //else{ Serial.println("Reagent File Opens!"); }
  if(!myFile.available())
  {
    lcd.setCursor(0,1);//line 2
    lcd.print("Reagent File Not Available");  
    Serial.println("Reagent File Not Available");
  }
  //else{ Serial.println("Reagent File is available!"); }
  myFile.close();
  */
  root = SD.open("/");
  
  lcd.setCursor(0,0); lcd.print("Building Lists...  ");

  Serial.println("Building Cocktail List...");
//  lcd.setCursor(0,0); lcd.print("Build Cocktail List ");
  getCocktailList();
  Serial.print(" - Number of Cocktails: "); Serial.println(NUM_COCKTAILS);
  lcd.setCursor(0,1); lcd.print("#Cocktails: "); lcd.setCursor(15,1); lcd.print(NUM_COCKTAILS);
 
  Serial.println("Building Reagents List...");
//  lcd.setCursor(0,0); lcd.print("Build Reagent List  ");
  getReagents();
  Serial.print(" - Number of Reagents: "); Serial.println(NUM_REAGENTS);
  lcd.setCursor(0,2); lcd.print("#Reagents: "); lcd.setCursor(15,2); lcd.print(NUM_REAGENTS);  
    
  Serial.println("Building Mixers List...");  
//  lcd.setCursor(0,0); lcd.print("Build Mixer List    ");
  getMixers();  
  Serial.print(" - Number of Mixers: "); Serial.println(NUM_MIXERS);
  lcd.setCursor(0,3); lcd.print("#Mixers: "); lcd.setCursor(15,3); lcd.print(NUM_MIXERS); 
  
  delay(500);
}

void getCocktailList()//may need to strip the dashes and the '.txt'
{
  //kill list first
  for(int i = 0; i < MAX_NUM_COCKTAILS; i++)
  {
    cocktails[i] = "C";
  } 
  while(true)
  {
    File entry =  root.openNextFile();
    if (! entry)
    {// no more files
      break;
    }
    String FN = entry.name();
    if(FN[0] == '-')//if its a cocktail
    {     
      for(int j = 0; j < MAX_NUM_COCKTAILS; j++)
      {
        if(cocktails[j] == "C")//eg not been assigned
        {
          cocktails[j] = "\0";//null it
          cocktails[j] = entry.readStringUntil('\n');
          cocktailFN[j] = FN;
          break;//pop out
        }
      }
    }
    entry.close();
  }
  //tidy up and count
  NUM_COCKTAILS = 0;
  for(int i = 0; i < MAX_NUM_COCKTAILS; i++)
  {
    cocktails[i].trim();
    cocktails[i].remove(0,1);//remove 1 char starting at index 0
    cocktails[i].remove(19,(cocktails[i].length()-19));//remove extra stuff, limit to 19 characters
    if(cocktails[i] != "\0")
      NUM_COCKTAILS++;
  } 
}

void getReagents()
{
  myFile = SD.open(FN_reagent);//or FN_mixer
  while(myFile.available())
  {
    for(int i = 0; i < MAX_NUM_REAGENTS; i++)
    {
      reagents[i] = "\0";
      reagents[i] = myFile.readStringUntil('\n');
      if(reagents[i] == "\0")//eg no write was performed
        break;
    }
  }
  myFile.close();
  //count up
  NUM_REAGENTS = 0;
  for(int i = 0; i < MAX_NUM_REAGENTS; i++)
  {
    reagents[i].trim();
    if(reagents[i] != "\0")
      NUM_REAGENTS++;
  } 
}

String getMixers()
{
  myFile = SD.open(FN_mixer);
  while(myFile.available())
  {
    for(int i = 0; i < MAX_NUM_MIXERS; i++)
    {
      mixers[i] = "\0";
      mixers[i] = myFile.readStringUntil('\n');
      if(mixers[i] == "\0")//eg no write was performed
        break;
    } 
  }
  myFile.close();
  //count up
  NUM_MIXERS = 0;
  for(int i = 0; i < MAX_NUM_MIXERS; i++)
  {
    mixers[i].trim();
    if(mixers[i] != "\0")
      NUM_MIXERS++;
  } 
}


