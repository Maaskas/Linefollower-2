//////////////////GLOBALS//////////////////////
#include "SerialCommand.h"
#include "EEPROMAnything.h"
#define Baudrate 115200

//Sensor + PID
SerialCommand sCmd(Serial1);
unsigned long cycleTime = 1000000;
unsigned long previous;
const int sensor[] = {A11, A10, A9, A0, A1, A2, A3, A4};
int normalisatie[8]; //Array voor normalisatie waarden
float dalpuntdebug; //1 numerieke waarde definiëren voor sensor 
int calculationTime;
bool start = LOW;

//Motor A
const byte Ain1 = 5; 
const byte Ain2 = 4;
const int PWMA = 3;

//Motor B
const byte Bin1 = 7;
const byte Bin2 = 8;
const int PWMB = 11;
const byte notStandby = 6;


//////////////////STRUCT//////////////////////
//Deze struct wordt opgeslagen in het EEPROM geheugen
struct RobotSettings
{
  unsigned long cycleTime; 
  int power;
  float Kp;
  float diff;
  int black[8];
  int white[8];
} BotSettings;




//////////////////SETUP//////////////////////
void setup()
{
  //Sensor + PID
  Serial.begin(Baudrate);
  Serial1.begin(Baudrate);
  sCmd.addCommand("set", onSet);
  sCmd.addCommand("cali", onCalibrate);
  sCmd.addCommand("debug", onDebug);
  sCmd.addCommand("start", onStart);
  sCmd.addCommand("stop", onStop);
  sCmd.setDefaultHandler(onUnknownCommand); //Bij een onbekend commando, voer deze commando uit

  //EEPROM
  EEPROM_readAnything(0, BotSettings); //Op adres 0 wordt de struct BotSettings gestuurd

  //Motor Driver
  pinMode(Ain1, OUTPUT);
  pinMode(Ain2, OUTPUT);
  pinMode(PWMA, OUTPUT);
  pinMode(Bin1, OUTPUT);
  pinMode(Bin2, OUTPUT);
  pinMode(PWMB, OUTPUT);
  pinMode(notStandby, OUTPUT);
  digitalWrite(notStandby, HIGH); //Enable Motor Driver
  
}





///////////////////LOOP///////////////////////

void loop()
{
  sCmd.readSerial();
  unsigned long current = micros();

  //Code om cyclustijd te veranderen in de loop
  if ((current - previous) > BotSettings.cycleTime)
  {
    previous = current;


    //Effectief programma
      for (int i = 0; i < 8; i++) 
      {
        normalisatie[i] = map(analogRead(sensor[i]), BotSettings.black[i], BotSettings.white[i], 0, 1000);
      }
      
     
  
  
     //Zwartste Sensor vinden
     int index = 0;
  
     for(int i = 1; i < 8; i++) 
     {
        if (normalisatie[i] < normalisatie[index]) 
          {
            index = i;
          }
     }
   

     if(normalisatie[index] > 600) start = false; //Robot stoppen als de zwartste sensor geen lijn detecteert


     //1 numerieke waarde definiëren voor sensor 
      float dalpunt = 0; 
     //Rekening houden wanneer je zwartste punt op de buitenkant ligt
     if (index == 0) 
     {
        dalpunt = -30;
     }
     else if (index == 7)
     {
        dalpunt = 30;
     }
     else 
        {
          
          int sensNul   = normalisatie[index];
          int sensMin1  = normalisatie[index-1];
          int sensPlus1 = normalisatie[index+1];
  
  
          float b = sensPlus1-sensMin1;
                b = b / 2; 
          float a = (sensPlus1 - b - sensNul);
          float c = sensNul;

          //Zwartste punt op y-as en de afstand tussen de sensoren is 1
          dalpunt = -b / (2 * a); 
          dalpunt += index; //y-as op sensor 0 zetten
          dalpunt -= 3.5; //y-as op sensor 0 zetten

          dalpunt *= 9.525; //Sensoren zijn 9,525 mm van elkaar geplaatst
        }
        
        dalpuntdebug = dalpunt; //Globale variabele voor dalpunt





     //P Regelaar
     float error = -dalpunt;
     float output = error * BotSettings.Kp;

     output = constrain(output, -510, 510); //Uiterste waarden van output definiëren



      
     int powerLeft = 0;
     int powerRight = 0;

     //Power staat voor hoeveel vermogen je effectief wilt gebruiken
     if (start) if (output >= 0)
     {
        powerLeft = constrain(BotSettings.power + BotSettings.diff * output, -255, 255);
        powerRight = constrain(powerLeft - output, -255, 255);
        powerLeft = powerRight + output;
     }
     else 
     {
        powerRight = constrain(BotSettings.power - BotSettings.diff * output, -255, 255);
        powerLeft = constrain(powerRight + output, -255, 255);
        powerRight = powerLeft - output;
     }

/*
      //Kruispunt detectie
        bool kruispunt = LOW;
       //Kruispuntdetectie
       for(int i = 0; i < 8; i++) 
       {
            if (normalisatie[i] < 500) //Als de witste sensor toch zwart detecteert
                {
                  kruispunt = HIGH;
                }
            else
                {
                  kruispunt = LOW;
                }
       }

       if (kruispunt) 
       {
        powerRight = BotSettings.power;
        powerLeft = BotSettings.power;
       }

*/
     

     //Aansturing Linkermotor
     if (powerLeft >= 0) 
       {
          digitalWrite (Ain1, HIGH);
          digitalWrite (Ain2, LOW);

       }
     else if (powerLeft < 0) 
       {
          digitalWrite (Ain1, LOW);
          digitalWrite (Ain2, HIGH);
          
       }
analogWrite(PWMA, abs(powerLeft));

    //Aansturing Rechtermotor
    if (powerRight >= 0)
      {
        digitalWrite(Bin1, HIGH);
        digitalWrite(Bin2, LOW);

      }
    else if (powerRight < 0)
      {
        digitalWrite(Bin1, LOW);
        digitalWrite(Bin2, HIGH);
       
      }
       analogWrite(PWMB, abs(powerRight));

        
   }  
      
 //Berekenen van de calculationTime
    unsigned long difference = micros() - current;
    if (difference > calculationTime) calculationTime = difference;


  
}



/////////////////FUNCTIONS/////////////////////
void onSet ()
{

  char* parameter = sCmd.next ();
  char* value = sCmd.next ();


  //Met deze functie kan je getallen inlezen
  if (strcmp(parameter, "cycle") == 0) 
  {
    BotSettings.cycleTime = atol(value); 
    Serial1.print("Cycle Time Set To ");
    Serial1.println(BotSettings.cycleTime);
  }
  
  else if (strcmp(parameter, "power") == 0) //Stelt snelheid in van de motor
  {
    BotSettings.power = atol(value);
    Serial1.print("Power Set To ");
    Serial1.println(BotSettings.power);
  }
  
  else if (strcmp(parameter, "diff") == 0) //Sneller/Trager rijden wanneer error Groter/kleiner is
  {
    BotSettings.diff = atof(value);
    Serial1.print("Diff Set To ");
    Serial1.println(BotSettings.diff);
  }
  else if (strcmp(parameter, "kp") == 0) //Stelt in hoe sterk er proportioneel geregeld wordt
  {
    BotSettings.Kp = atof(value); 
    Serial1.print("Kp Set To ");
    Serial1.println(BotSettings.Kp);
  }
  else Serial1.println ("Unknown value for function: onSet");
  
  
  //Alle aangepaste settings terugschrijven in het EEPROM geheugen
  EEPROM_writeAnything(0, BotSettings); 
}


void onStart ()
{
  start = HIGH;
}

void onStop ()
{
  start = LOW;
}

void onDebug ()
{
  Serial1.print("Cali Black: ");
  for (int i = 0; i < 8; i++) 
  {
    Serial1.print(BotSettings.black[i]);
    Serial1.print (" ");
  }
  Serial1.println(" ");

  Serial1.print("Cali White: ");
  for (int i = 0; i < 8; i++) 
  {
    Serial1.print(BotSettings.white[i]);
    Serial1.print(" ");
  }
  Serial1.println(" ");

  Serial1.print("Norm Values: ");
  for (int i=0; i < 8; i++)
  {
    Serial1.print(normalisatie[i]);
    Serial1.print(" ");
  }
  Serial1.println("");

  Serial1.print("Power: ");
  Serial1.println(BotSettings.power);

  Serial1.print("Diff: ");
  Serial1.println(BotSettings.diff);

  Serial1.print("Kp: ");
  Serial1.println(BotSettings.Kp);

  Serial1.print("Cycle Time: ");
  Serial1.println(BotSettings.cycleTime);

  Serial1.print("Calculation Time: ");
  Serial1.println(calculationTime);
  
  Serial1.print("Position: ");
  Serial1.println(dalpuntdebug);

  Serial1.println(""); //Voor beetje overzicht op de BT module
  Serial1.println("");
}




void onCalibrate()
{
  char* parameter = sCmd.next();

  if (strcmp(parameter, "black") == 0) 
  {
    Serial1.print("Start Calibrating Black... ");
    for (int i = 0; i < 8; i++) 
    {
      BotSettings.black[i] = analogRead(sensor[i]);
    }
    Serial1.println("Done!");
  }

  
  if (strcmp(parameter, "white") == 0) 
  {
    Serial1.print("Start Calibrating White... ");
    for (int i = 0; i < 8; i++) 
    {
      BotSettings.white[i] = analogRead(sensor[i]);
    }
    Serial1.println("Done!");
  }
  
  //Struct met Calibratie waarden in, schrijven naar EEPROM;
  EEPROM_writeAnything(0, BotSettings);
  
}



void onUnknownCommand(char* command) 
{
  Serial1.print("Unknown Command: \"");
  Serial1.print(command);
  Serial1.println("\"");
}
