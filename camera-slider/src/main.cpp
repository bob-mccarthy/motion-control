#include <Arduino.h>
#include "MotionController.h"
const int stepsPerRev = 200;//steps in revolution
const int stepMode = 8;//1/64 Stepping Mode
const float diameter = 12.7;//steps in revolution
MotionController moco;
hw_timer_t *timer = NULL;
unsigned long pastRead = 0;

void ARDUINO_ISR_ATTR onTimer() {
  moco.runInterruptHandler();
}

void setup() {
  Serial.begin(115200);
  moco.addAxis((stepsPerRev*stepMode)/(PI*diameter), D4, D5);
  moco.setSpeed(0, 20);//move axis 0 at 0.5mm/s
  unsigned long interruptDelay = moco.getInterruptInterval();
  // Set timer frequency to 1Mhz
  timer = timerBegin(1000000);
  // Attach onTimer function to our timer.
  timerAttachInterrupt(timer, &onTimer);
  // Set alarm to call onTimer function every second (value in microseconds).
  // Repeat the alarm (third parameter) with unlimited count = 0 (fourth parameter).
  timerAlarm(timer, interruptDelay, true, 0);

}

void loop() {
  int num = -1;
  if (Serial.available() > 0){
    num = Serial.parseInt(); 
    Serial.println("Num: " + (String)num);
  }
  if(num == -1 || num == 0){
    return;
  }
  moco.setTargetPos(0, num);
}
