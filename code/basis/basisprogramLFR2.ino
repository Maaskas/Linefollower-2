#include "SerialCommand.h"
#include "EEPROMAnything.h"
#define Baudrate 115200

SerialCommand sCmd(Serial);
bool debug = false;
unsigned long cycleTime = 1000000;
unsigned long previous;

//Deze struct wordt opgeslagen in het EEPROM geheugen
struct RobotSettings
{
  unsigned long cycleTime; 
} BotSettings;



void setup()
{
  Serial.begin(Baudrate);
  sCmd.addCommand("set", onSet);
  sCmd.setDefaultHandler(onUnknownCommand); //Bij een onbekend commando, voer deze commando uit

  //Alle settings halen uit het EEPROM geheugen
  EEPROM_readAnything(0, BotSettings); //Op adres 0 wordt de struct BotSettings gestuurd
  
  Serial.println("ready");
}




void loop()
{
  sCmd.readSerial();
  unsigned long current = micros();

  //Code om cyclustijd te veranderen in de loop, zonder een wait toe te passen
  //Als de if niet voldaan is, gaat het programma gewoon verder
  if ((current - previous) > BotSettings.cycleTime)
  {
    previous = current;

    if (debug)
      {
        Serial.println("Debug mode ON");
      }
  } 
}







void onSet ()
{

  char* parameter = sCmd.next ();
  char* value = sCmd.next ();


  if (strcmp(parameter, "debug") == 0) 
  {
      debug = (strcmp(value, "on") == 0);
  }

  else if (strcmp(parameter, "cycle") == 0)
  {
    BotSettings.cycleTime = atol(value); //Met deze functie kan je getallen inlezen
    
  }
  
  else
  
  Serial.println ("Unknown value for function: onSet");
  
  //Alle aangepaste settings terugschrijven in het EEPROM geheugen
  EEPROM_writeAnything(0, BotSettings);
  
}







void onUnknownCommand(char* command) 
{
  Serial.print("Unknown Command: \"");
  Serial.print(command);
  Serial.println("\"");
}
