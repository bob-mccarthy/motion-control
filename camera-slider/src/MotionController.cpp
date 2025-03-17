#include "MotionController.h"
#include <Arduino.h>

void MotionController::addAxis(double unitsToSteps, uint8_t dirPin, uint8_t stepPin ){
    axis a;
    a.unitsToSteps = unitsToSteps;
    a.dirPin = dirPin;
    a.stepPin = stepPin;
    pinMode(dirPin, OUTPUT);
    pinMode(stepPin, OUTPUT);
    this->axesArr[this->numAxes] = a;
    this->numAxes++;
}

void MotionController::setSpeed(uint8_t axisNum, double speed){
    this->axesArr[axisNum].speed = speed * this->axesArr[axisNum].unitsToSteps;

    //recalculate the new interrupt interval now that a new speed has been set
    unsigned long maxSpeed = 0;
    for (int i = 0; i < numAxes; i++)
    {
        maxSpeed = max(this->axesArr[i].speed, maxSpeed);
    }
    // invert maxSpeed to get sec per step then multiply by 1,000,000 to get usec per step
    this->interruptInterval = 1000000/maxSpeed;
}

void MotionController::setTargetPos(uint8_t axisNum, double targetPos){
    this->axesArr[axisNum].targetPos = targetPos * this->axesArr[axisNum].unitsToSteps;
}

unsigned long MotionController::getInterruptInterval(){
    return this->interruptInterval;
}


void MotionController::runInterruptHandler(){
    for (int i = 0; i < this->numAxes; i++)
    {
        if (cyclesTilStep > 0)
        {
            cyclesTilStep--;
        }
        int8_t dir = this->axesArr[i].targetPos > this->axesArr[i].currPos ? 1: -1;
        digitalWrite(this->axesArr[i].dirPin, dir == 1? HIGH: LOW);
        if (this->axesArr[i].targetPos != this->axesArr[i].currPos && cyclesTilStep == 0)
        {
            digitalWrite(this->axesArr[i].stepPin, HIGH);
            digitalWrite(this->axesArr[i].stepPin, LOW);
            this->axesArr[i].currPos += dir;
            this->cyclesTilStep = calcNextStepTime(i, this->axesArr[i].currPos);
        }
        
    }
    
}

unsigned long MotionController::calcNextStepTime(uint8_t axisNum, unsigned long pos){
    if (pos == this->axesArr[axisNum].targetPos){
        return 0;
    }

    return (100000 / this->axesArr[axisNum].currPos) / this->interruptInterval;
}

long MotionController::distanceToGo(uint8_t axisNum){
    return this->axesArr[axisNum].targetPos - this->axesArr[axisNum].currPos;
}
