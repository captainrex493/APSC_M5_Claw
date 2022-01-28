#include <HCSR04.h>

#include <Servo.h>

#define echoPin 5
#define trigPin 6
#define servoPin 9


unsigned int distance = 0;

int close_height = 15;
int open_height = 60;

bool flag_open = false;
bool flag_close = false;

int close_counter = 0;
int open_counter = 0;

Servo servo;

HCSR04 hc(trigPin,echoPin);

void setup()
{
  pinMode(LED_BUILTIN, OUTPUT);

  pinMode (echoPin, INPUT);
  pinMode (trigPin, OUTPUT);
  
  servo.attach(servoPin, 500, 2500);
  servo.write(0);
  
  Serial.begin(9600);

}

void loop() {
  
  distance = hc.dist();
  
  if (distance < close_height) {
    open_counter = 0;
    if (flag_close == false) {
      close_counter++;
      if (close_counter > 25) {
        flag_close = true;
        close_counter = 0;
      }
    }
    if (flag_close == true) {
      servo.write(90);
      delay(15);
      flag_close = false;

    }
  }
  else if (distance > open_height) {
    close_counter = 0;
    if (flag_open == false) {
      open_counter++;
      if (open_counter > 25) {
        flag_open = true;
        open_counter = 0;
      }
   }
    if (flag_open == true) {
      servo.write(0);
      delay(15);
      flag_open = false;
    }
  }
  
  Serial.print (distance);
  Serial.println (" cm");
  delay(15);
}