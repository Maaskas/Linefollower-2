#include <SoftwareSerial.h>

SoftwareSerial mySerial(10, 11); // RX, TX

void setup() {
  
  mySerial.begin(115200);
  Serial.begin(9600);
  
}
  void loop()
{
  
  if (Serial.available() > 0) 
  {
    String teststr = Serial.readString();  //read until timeout
    teststr.trim();                        // remove any \r \n whitespace at the end of the String
    mySerial.println(teststr);
  }


  if (mySerial.available())

 Serial.write(mySerial.read());
   
  
}
