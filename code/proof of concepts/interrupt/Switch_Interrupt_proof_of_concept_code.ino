//Dit is de juiste code voor switch interrupt
const byte LED = 15;
const byte BUTTON = 2;

bool Run;

// Interrupt Service Routine (ISR)
void switchPressed ()
{

  Run = !Run;
  
}  // end of switchPressed



void setup ()
{
  pinMode (LED, OUTPUT);  // so we can update the LED
 // digitalWrite (BUTTON, HIGH);  // internal pull-up resistor
  attachInterrupt (digitalPinToInterrupt (BUTTON), switchPressed, RISING);  // attach interrupt handler
}  // end of setup

void loop ()
{
  // loop doing nothing 
  if (Run)
  {
    digitalWrite(LED, HIGH);
    delay(500);
    digitalWrite(LED, LOW);
    delay(500);
  }
  else digitalWrite(LED, LOW);
  
} 
