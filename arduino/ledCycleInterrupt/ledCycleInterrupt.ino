// avr-libc library includes
#include <avr/io.h>
#include <avr/interrupt.h>
#define LEDPIN 8

int daycycle = 14; // number of hours the day lasts (LEDs ON)
int nightcycle = 10; // number of hours the day lasts (LEDs OFF)

long hourseconds = 3600; // number of seconds in an hour
unsigned long daycycleseconds = hourseconds * daycycle;
unsigned long nightcycleseconds = hourseconds * nightcycle;

volatile boolean isday = true;
volatile unsigned long current = 0;

void setup()
{
  pinMode(LEDPIN, OUTPUT);
  // initialize Timer1
  cli();          // disable global interrupts
  TCCR1A = 0;     // set entire TCCR1A register to 0
  TCCR1B = 0;     // same for TCCR1B
  
  // set compare match register to desired timer count:
  OCR1A = 15624;
  
  // turn on CTC mode:
  TCCR1B |= (1 << WGM12);
  
  // Set CS10 and CS12 bits for 1024 prescaler:
  TCCR1B |= (1 << CS10);
  TCCR1B |= (1 << CS12);
  
  // enable timer compare interrupt:
  TIMSK1 |= (1 << OCIE1A);
  
  // enable global interrupts:
  sei();

  // switches LEDs ON
  digitalWrite(LEDPIN, HIGH);
}

void loop()
{
// nothing for now
}

ISR(TIMER1_COMPA_vect)
{
  current++;
  if(isday){
    if(current == daycycleseconds){
      apply();
    }
  }
  else{
    if(current == nightcycleseconds){
      apply();
    }
  }
}

void apply(){
  current = 0;
  isday = !isday; // switching day/night
  switchLeds();
}

void switchLeds(){
  digitalWrite(LEDPIN, !digitalRead(LEDPIN));
}

