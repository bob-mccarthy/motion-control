#ifndef MOTIONCONTROLLER_H
#define MOTIONCONTROLLER_H
#include <Arduino.h>


class MotionController{
    public:
        /*
            Motion Controller Constructor
        */
        MotionController(){}
        /*
            addAxis
                allows a new axis of control and adds pins to connect to stepper motor
                params:
                    unitsToSteps: the conversions from what ever units the cart is moving in to stepper motor steps (ie millimeters to steps)
                    dirPin: the pin that controls the direction of the motor
                    stepPin: the pin that controls the stepping of the stepper motor
                
        */
        void addAxis(double unitsToSteps, uint8_t dirPin, uint8_t stepPin );
        /*
            setSpeed
                sets the speed for the for the given axis 
                params:
                    axisNum: the index of the axis
                    speed: the speed of the axis in the units/s (with the units specified in the addAxis function)
                
        */
        void setSpeed(uint8_t axisNum, double speed);

        /*
            setTargetPos
                sets the target position for the for the given axis 
                params:
                    axisNum: the index of the axis
                    position: the target position in the units specified in addAxis
                
        */
        void setTargetPos(uint8_t axisNum, double position);

        /*
            getInterruptInterval
                gets how quickly the interrupt should be triggered, but needs to be called after setting the speed
                returns:
                    how often the interrupt should be triggered in microseconds
        */
        unsigned long getInterruptInterval();

        /*
            runInterruptHandler
                this runs in an interrupt with a resolution of 1Mhz and at the frequency specified by getInterruptInterval. This is run a step 
                if a step is due, but needs to be in 
        */
        void runInterruptHandler();

         /*
            isMoving
                returns how far the cart is from its target position
                params:
                    axisNum: the index of the axis
                returns:
                    returns a signed number of how far it from the cart (positive if the currPos is behind targetPos, and negative otherwise)

        */
        long distanceToGo(uint8_t axisNum);

        
    private:
        /*  
            calcNextStepTime:
                takes in a position and axis and  calculates the time until the next step needs to be taken
                params:
                    axisNum: the index of the axis
                    pos: position (in steps)
                returns:
                    returns the number of time until the next step in interrupt cycles
        */
        unsigned long calcNextStepTime(uint8_t axisNum, unsigned long pos);
        //all the information to allow motion control along a new axis
        struct axis{
            double unitsToSteps; // conversion of whatever units are being used to steps
            uint8_t dirPin; // dir pin of stepper motor (LOW is backward, HIGH is forward)
            uint8_t stepPin; // step pin of stepper motor
            unsigned long speed = 0; // speed of axis in (steps/sec)
            unsigned long targetPos = 0; // target position of the camera (in steps)
            unsigned long currPos = 0; // current position of the camera (in steps)
        };

        uint8_t numAxes = 0; //the number of axes being used 
        axis axesArr[10];
        unsigned long interruptInterval; 
        unsigned long cyclesTilStep = 1; //the number of interrupt calls until the next step happens (1 mean that it with trigger on the next interrupt)
        
        

};
#endif 