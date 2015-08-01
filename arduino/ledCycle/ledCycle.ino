int LEDPin = 8;

long anHour = 1000 * 60 * 60;
long twelveHours = 12 * anHour;

int aSecond = 1000;
/*
 * setup() - this function runs once when you turn your Arduino on
 * We set the motors pin to be an output (turning the pin high (+5v) or low (ground) (-))
 * rather than an input (checking whether a pin is high or low)
 */
void setup()
{
 pinMode(LEDPin, OUTPUT); 
}

/*
 * loop() - this function will start after setup finishes and then repeat
 * we call a function called motorOnThenOff()
 */

void loop()                     // run over and over again
{
 ledsOnThenOff();
}

/*
 * motorOnThenOff() - turns motor on then off 
 * (notice this code is identical to the code we used for
 * the blinking LED)
 */
void ledsOnThenOff(){  
  digitalWrite(LEDPin, HIGH);
  delay(aSecond);      
  digitalWrite(LEDPin, LOW);
  delay(aSecond);
  
  digitalWrite(LEDPin, HIGH);
  delay(anHour);      
  digitalWrite(LEDPin, LOW);
  delay(anHour);               
}
