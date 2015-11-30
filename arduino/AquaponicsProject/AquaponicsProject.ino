/*
 *  Clock mechanism to execute an action every hour
 *  
 *  TODO Switch action reset hours to 9
 *  TODO Safety extra sensor
 */

#include <TimerOne.h>
#include <avr/io.h>
#include <avr/interrupt.h>

// Pin definitions
#define LED           13    // For Leonardo built in led
// -Relays
#define PUMP_IN       2     // Pin of the pump 1
#define PUMP_OUT      3    // Pin of the pump 2
#define LAMP          4     // Pin of the lamp relay
#define FOG           5     // Pin of the lamp relay
// -Sensors
#define WATER_UP      9     // Pin of the water sensor 1
#define WATER_DOWN    8     // Pin of the water sensor 2

// Time definition
#define DAY_TIME      14    // Number of hours of daylight (LAMP ON)
#define DAY_START     9     // Hour of the day that the day starts
#define PUMP_CYCLE    15    // Time between 2 pump cycles
#define WATER_UP_TIME 5     // Time to water at high level

#define SIMULATION true //Boolean to activate the simulation (reduce time 1mn = 1s)

// Time counter variables
volatile int hoursCounter;   // 0 to 23
volatile int secondsCounter;  // 0 to 59
volatile int minutesCounter;  // 0 to 59

bool ledState = 0;            // led status memory
bool waterLevelState = 0;     // water level status memory 
#define DOWN      0
#define FILLING   1
#define UP        3
#define CLEARING  4


/* void setup(void) 
 *  Setup software run only once
 *  Allows to setup all the software and hardware
*/
void setup(void)
{
  // Setup the pin function TODO select good pin
  pinMode(LAMP, OUTPUT);
  pinMode(LED, OUTPUT);
  pinMode(PUMP_IN, OUTPUT);
  pinMode(PUMP_OUT, OUTPUT);
  pinMode(WATER_UP, INPUT);
  digitalWrite(WATER_UP, HIGH);   //Setup pull up
  pinMode(WATER_DOWN, INPUT);
  digitalWrite(WATER_DOWN, HIGH); //Setup pull up
  
  // initialize Timer1 for 1 second interval
  Timer1.initialize(1000000);     // Set 1 second to timer 1
  Timer1.attachInterrupt(seconds_timer);

  hoursCounter = DAY_START;      // Init the time to DAY_START at startup
  minutesCounter = 1;
  secondsCounter = 1;
  digitalWrite(LAMP, HIGH);
}


/* void loop(void) 
 *  Main program automatically loaded
 *  
*/
void loop(void)
{
  switch (waterLevelState) {
    
    case DOWN:
        //Wait for pump cycle time to activate pump
        if(0 == (minutesCounter % PUMP_CYCLE))
        {
          digitalWrite(PUMP_IN, 1);   // Turn ON the filling pump
          waterLevelState = FILLING;
        }
      break;
      
    case FILLING:
       // wait for water up sensor to be activated
       if(0 == digitalRead(WATER_UP))
        {
          digitalWrite(PUMP_IN, 0); // Turn OFF the filling pump
          waterLevelState = UP;
        }
      break;
      
    case UP:
        //Wait for level up time passed to clear 
        if(0 == ((minutesCounter % PUMP_CYCLE) % WATER_UP_TIME))
        {
          digitalWrite(PUMP_OUT, 1);   // Turn ON the clearing pump
          waterLevelState = CLEARING;
        }
      break;
      break;
      
    case CLEARING:
        // wait for water down sensor to be activated
        if(0 == digitalRead(WATER_DOWN))
        {
          digitalWrite(PUMP_OUT, 0); // Turn OFF the clearing pump
          waterLevelState = DOWN;
        }
      break;
      
    default:
      // default is optional
    break;
  }
}


/* seconds_timer(void) 
 *  Timer executed every seconds for time count
 *  This function allows to make a basic clock which count hours, minutes and seconds
*/
void seconds_timer(void)
{
  
  toggle_led(); //Toggle the card LED every seconds

  // In case of SIMULATION increment minutes to go quicker
  if (SIMULATION)
  {
    minutesCounter ++;      // Increment the minutes counter
  }
  else
  {// In case of normal use, increment seconds
    secondsCounter++;       // Increment the seconds counter
  }

  // Every 60s we increment the minutes counters (not used in SIMULATION)
  if (0 == secondsCounter % 60)
  {
    minutesCounter ++;      //Increment the minutes counter
    secondsCounter = 0;     // Clear the seconds counter
  }

  // Every  60min increment the hours counter
  if (0 == minutesCounter % 60)
  {
    hoursCounter ++;        // Increments the hours counter
    minutesCounter = 0;     // Clears the minutes counter
    
    // At the end of the day reset the hours counter
    if (23 < hoursCounter)
    {
      hoursCounter = 0;     // Clear the hours counter
    }

    // Execute following code once every hour
    hours_action();
  }
}

/* void hour_action(void) 
 *  Program executed  one time every hours
 *  
*/
void hours_action(void)
{
  //Check if something has to be done
  if (DAY_START == hoursCounter)
    digitalWrite(LAMP, 1);

  if ((DAY_START + DAY_TIME) == hoursCounter)
    digitalWrite(LAMP, 0);
}

void toggle_led(void)
{
   ledState = ledState ^ 1;
    digitalWrite(LED, ledState);
}

