const byte debounce_delay = 200;  //Tijd tussen eerste en tweede check.
const byte btn_pin = 2;
const byte ledPin = 14;
bool btn_state = HIGH;
bool btn_prev = HIGH;
bool stateLED = LOW;
unsigned long last_debounce_time = 0;  //Wordt gebruikt als een timestamp
volatile byte state = LOW;

void setup() {
  Serial.begin(115200);
  pinMode(btn_pin, INPUT_PULLUP);
  pinMode(ledPin, OUTPUT);
  attachInterrupt(digitalPinToInterrupt(btn_pin), ISRbtn, FALLING);
  last_debounce_time = 0;
 
}

void ISRbtn() {
  
  if (millis() > last_debounce_time + debounce_delay) {  //Wanneer de huidige millis grote wordt als last_debounce_time + de delay, wordt de code pas uitgevoerd
   last_debounce_time = millis();
   GreenLed();

  }
  //Remember the previous state for the next loop

}

void GreenLed() {
  stateLED = !stateLED;
  Serial.println ("LED werkt");
}


void loop (){
   digitalWrite(ledPin, stateLED);

}
