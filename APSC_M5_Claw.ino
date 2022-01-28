#include <Servo.h>

#define echoPin 5
#define trigPin 6
#define servoPin 9

unsigned int duration = 0;
unsigned int distance = 0;

int close_height = 40;
int open_height = 100;

bool flag_item = false;

Servo servo;

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
  digitalWrite(trigPin, HIGH); 
  delayMicroseconds (10); 
  digitalWrite(trigPin, LOW);
  
  duration = pulseIn (echoPin, HIGH);
  
  distance = duration*0.01715;
  
  if (distance < close_height) {
    servo.write(180);
    delay(15);
  }
  else if (distance > open_height) {
    servo.write(0);
  }
  
  Serial.print (distance);
  Serial.println ("cm");
  delay(100); 
}