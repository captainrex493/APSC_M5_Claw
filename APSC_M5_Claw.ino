#include <HCSR04.h>

#include <Servo.h>


#define echoPin 5
#define trigPin 6
#define servoPin 9

// Stores the most recent reading from the ultrasonic sensor
unsigned int distance = 0;

// The height in cm at which the claw should open and close
int close_height = 15;
int open_height = 60;

// Variables to keep track of how long the claw has been in position to open and close.
int close_counter = 0;
int open_counter = 0;

// How long the claw should be at the open or close height before moving in seconds.
int seconds_before_close = 5;
int seconds_before_open = 2;


// Variables to keep track of when to open and close the claw.
bool flag_open = false;
bool flag_close = false;

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
  servo.write(180);

  //Open serial connection.
  Serial.begin(9600);

}

void loop() {

  //Calculate when the next loop should begin.
  long next_loop = millis() + loop_time;

  //Get current distance.
  distance = hc.dist();

  //Check if the claw is in range to close. If it is, keep track of how long it's been there and then after a set period of time has passed, close it.
  if (distance < close_height) {
    open_counter = 0;
    if (flag_close == false) {
      close_counter++;
      if (close_counter > ((seconds_before_close*1000)/loop_time)) {
        flag_close = true;
        close_counter = 0;
      }
    }
    if (flag_close == true) {
      servo.write(0);
      Serial.println("Closing claw");
      delay(15);
      flag_close = false;

    }
  }
  //Same as above but for opening instead of closing.
  else if (distance > open_height) {
    close_counter = 0;
    if (flag_open == false) {
      open_counter++;
      if (open_counter > ((seconds_before_open*1000)/loop_time)) {
        flag_open = true;
        open_counter = 0;
      }
   }
    if (flag_open == true) {
      servo.write(180);
      Serial.println("Opening claw");
      delay(15);
      flag_open = false;
    }
  }

  //Record distance to serial monitor.
  Serial.print (distance);
  Serial.println (" cm");


  //Loop managment.
  long loop_delay = next_loop - millis();
  if ( loop_delay < 0) {
    Serial.println("Running too slow");
  }
  else {
    delay(loop_delay);
  }
}
