#include <SoftwareSerial.h>

SoftwareSerial mySerial(1, 0); // RX, TX

void setup() {
  
  Serial1.begin(115200);
  Serial.begin(115200);
  Serial.println("Ready");
  
}


  void loop()
{
  
  if (Serial.available() > 0) 
  {
    String teststr = Serial.readString();  //read until timeout
    teststr.trim();                        // remove any \r \n whitespace at the end of the String
    Serial1.println(teststr);
  }


  if (Serial1.available())
  {
  Serial.write(Serial1.read());
  }
  
}
