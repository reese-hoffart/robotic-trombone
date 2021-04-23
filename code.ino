/* CODE FOR REESE'S SELF-PLAYING TROMBONE ISP
 * 
 * Changes I wasn't able to implement (both in code and with build) in time:
 *  - Dynamically change balloon angle / air frequency while playing song.
 *    Reason: Insufficient accuraccy of stepper assembly.
 *  - Tounging and similar articulations.
 *    Reason: Must start air when slide is in 1st position and move slide from
 *            there to get accurate and consistent notes.
 *  - Play whole song in one go
 *    Reason: Limited air compressor capacity, can't run compressor while
 *            playing b/c compressor is loud.
 */


// *** ALWAYS START SLIDE AT 1st POSITION AND STEPPER PERPINDICULAR TO BELL ***

#include <Wire.h>
#include <Servo.h> 
#include <Adafruit_MotorShield.h>
#include <AccelStepper.h>

Adafruit_MotorShield ms = Adafruit_MotorShield();
Adafruit_DCMotor *dc = ms.getMotor(1);
Servo servo;

/* The song[] array holds slide positions
 *
 * The partial[] array holds partials (balloon angles / frequency) but
 * wasn't implmeneted as stated above.
 * 
 * song array layout: [SlidePosition, ...]
 * partial array layout: [Partial, Partial, ...] (0 is F-Bb, 1 is B-F, 2 is F#-Bb, 3 is B-D, 4 is Eb-F)
 */

//int song[] =    {6,6,6,6,3,3,4,6,3,3,4,6,6,6,3,3,   3,3,3,   3,4,4,7,7}; // Whole Chorus
int song[] =    {6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,3,3,3,3,3,3,3,3,4,4,4,4,4,4,4,4}; // Pre-Chorus
//int song[] =    {1,1,3,1,3,1,3,1,3}; //Post-Chorus

//int partial[] = {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,   1,1,1,   1,1,1,1,1}; // Chorus
int partial[] = {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}; // Pre-Chorus
//int partial[] = {1,1,1,1,1,1,1,1,1}; //Post-Chorus
int tempo = 800; //ms/beat

int currentPos = 1;

void setup() {
  while(!Serial);
  Serial.begin(9600);
  ms.begin();
  Serial.println("Motor Shield Attached");
  servo.attach(9);

  dc->setSpeed(255);
  dc->run(RELEASE);
  servo.write(0);

  //Release servo at start
  servo.write(50);
  delay(2000);
  
  Serial.println("Song Start");
  for(int i = 0; i < sizeof(song)/sizeof(song[0]); i+=1) {
    int startTime = millis();
    moveSlide(song[i]);
    while(millis()-startTime <= tempo);
  }

  //Close servo
  servo.write(0);
  Serial.println("Song End");
}

void moveSlide(int pos) {
  int distance = pos - currentPos;
  int playTime = abs(distance)*130;
  
  int startTime = millis();
  if(distance > 0) dc->run(BACKWARD);
  else if(distance < 0) dc->run(FORWARD);
  while(millis() - startTime <= playTime);
  dc->run(RELEASE);
  
  currentPos = pos;
}

void loop() {}
