#include <Servo.h>
Servo myservo;
int servo = 9;
int sound = A0; 
const int threshold=200;

//Set the sound sensor as input and servo motor as output
void setup() {
     Serial.begin(9600);   
     myservo.attach(servo);
     pinMode(sound,INPUT);  
     pinMode(servo,OUTPUT);   
}

//When the sound value is greater than 200, the servo motor rotates 180, and the small is 0, so that the servo motor keeps rotating.
void loop() {
int statusSensor = analogRead(sound);
  
  if (statusSensor >= threshold)
  {
    myservo.write(180);
    delay(1000);

  }
  else

  {
    myservo.write(0);

  }
}
