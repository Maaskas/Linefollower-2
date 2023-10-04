 //Motor A
  const int Ain1 = 5; 
  const int Ain2 = 4;
  const int PWMA = 3;

  //Motor B
  const int Bin1 = 7;
  const int Bin2 = 8;
  const int PWMB = 11;

  const int notStandby = 6;

void setup() {
  
  //set all pins as output

  Serial.begin (9600);
  
  pinMode(Ain1, OUTPUT);
  pinMode(Ain2, OUTPUT);
  pinMode(PWMA, OUTPUT);
  pinMode(Bin1, OUTPUT);
  pinMode(Bin2, OUTPUT);
  pinMode(PWMB, OUTPUT);
  pinMode(notStandby, OUTPUT);


  //Enable Motor Driver
  digitalWrite(notStandby, HIGH);

}

void loop() {
  
//Full cycle of left motor
  digitalWrite (Ain1, HIGH);
  digitalWrite (Ain2, LOW);

    for (int i = 0; i <= 255; i++) {
      analogWrite (PWMA,  i);
      delay (10);
      Serial.println(PWMA);
      }

  digitalWrite (Ain1, LOW);
  digitalWrite (Ain2, LOW);

  delay( 500);

  digitalWrite (Ain1, LOW);
  digitalWrite (Ain2, HIGH);

    for (int g = 0; g <= 255; g++) {
      analogWrite (PWMA,  g);
      delay (10);
      Serial.println(PWMA);
      }

  digitalWrite (Ain1, LOW);
  digitalWrite (Ain2, LOW);
  delay( 500);





//Full cycle of right motor
  digitalWrite (Bin1, LOW);
  digitalWrite (Bin2, HIGH);

      for (int f = 0; f <= 255; f++) {
      analogWrite (PWMB,  f);
      delay (10);
      Serial.println(PWMB);
      }

  digitalWrite (Bin1, LOW);
  digitalWrite (Bin2, LOW);
  delay(500);


  digitalWrite (Bin1, HIGH);
  digitalWrite (Bin2, LOW);

      for (int k = 0; k <= 255; k++) {
      analogWrite (PWMB,  k);
      delay (10);
      Serial.println(PWMB);
      }

  digitalWrite (Bin1, LOW);
  digitalWrite (Bin2, LOW);
  delay(500);




  
}
