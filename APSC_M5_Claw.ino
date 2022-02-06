#include <HCSR04.h>
#include <Servo.h>

#define echoPin 5
#define trigPin 6
#define servoPin 9
#define servoOpen 180
#define servoClosed 0

// Stores the most recent reading from the ultrasonic sensor
unsigned int distance = 0;

// The height in cm at which the claw should open and close
int close_height = 15;
int open_height = 20;
int transition_height = 50;

// Variables to keep track of how long the claw has been in position to open and close.
int close_counter = 0;
int open_counter = 0;
int transition_counter;

// How long the claw should be at the open or close height before moving in seconds.
int seconds_before_close = 5;
int seconds_before_open = 3;
int seconds_before_transition = 3;


// Variables to keep track of when to open and close the claw.
bool flag_open = false;
bool flag_close = false;
bool flag_transition = false;


//Current position of the claw
// false -> Open
// true -> Closed 
bool flag_current = false;

bool transition = true;

// How long each loop should take in milliseconds. 
int loop_time = 60;

// Define servo and ultrasonic sensor.
Servo servo;
HCSR04 hc(trigPin,echoPin);

void setup()
{
  //Set pinmode.
  pinMode (echoPin, INPUT);
  pinMode (trigPin, OUTPUT);

  //Set up servo.
  servo.attach(servoPin, 500, 2500);
  servo.write(servoOpen);

  //Open serial connection.
  Serial.begin(9600);

}

void loop() {

  //Calculate when the next loop should begin.
  long next_loop = millis() + loop_time;

  //Get current distance.
  distance = hc.dist();

  if ( distance <= close_height && !flag_current && transition) {
    open_counter = 0;
    transition_counter = 0;
    
    if ( !flag_close) {
      close_counter++;
      flag_open = false;
      flag_transition = false;
      Serial.println(close_counter);
      
      if (close_counter > ((seconds_before_close*1000)/loop_time)) {
        flag_close = true;
        close_counter = 0;
      }
    }
    if (flag_close) {
      servo.write(servoClosed);
      delay(15);
      Serial.println("Closing Claw");
      
      flag_close = false;
      transition = false;
      flag_current = true;
    }
  }

  else if (distance <= open_height && flag_current && transition) {
    close_counter = 0;
    transition_counter = 0;
    
    if ( !flag_open) {
      open_counter++;
      flag_close = false;
      flag_transition = false;
      
      if (open_counter > ((seconds_before_open*1000)/loop_time)) {
        flag_open = true;
        open_counter = 0;
      }
    }
    if (flag_open) {
      servo.write(servoOpen);
      Serial.println("Closing Claw");
      delay(15);
      flag_open = false;
      transition = false;
      flag_current = false;
    }
  }
  else if (distance >= transition_height && !transition) {
    open_counter = 0;
    close_counter = 0;

    if (!flag_transition) {
      transition_counter++;
      flag_close = false;
      flag_open = false;

      if (transition_counter > ((seconds_before_transition*1000)/loop_time)) {
        transition_counter = 0;
        flag_transition = true;
      }
    }
    if (flag_transition) {
      transition = true;
      flag_transition = false;
      Serial.println("Transition achieved");
    }

    
  }
  

  
  //Record distance to serial monitor.
  Serial.print(distance);
  Serial.println(" cm");


  //Loop managment.
  long loop_delay = next_loop - millis();
  if ( loop_delay < 0) {
    Serial.println("Running too slow");
  }
  else {
    delay(loop_delay);
  }
}
