#include <webots/Robot.hpp>
#include <webots/Motor.hpp>
#include <webots/DistanceSensor.hpp>


#define MAX_SPEED 6.28

//64 Milliseconds
#define TIME_STEP 64 

using namespace webots;

int main(int argc, char **argv) {
 Robot *robot = new Robot();

 // get a handler to the motors and set target position to infinity (speed control)
 Motor *leftMotor = robot->getMotor("left wheel motor");
 Motor *rightMotor = robot->getMotor("right wheel motor");
 leftMotor->setPosition(INFINITY);
 rightMotor->setPosition(INFINITY);
 double t=0.0;
 
 double l_direction=1.0;
 double r_direction=1.0;
 
 while(true) {
    

    leftMotor->setVelocity( l_direction*MAX_SPEED*0.1);
    rightMotor->setVelocity( r_direction*MAX_SPEED*0.1);
    robot->step(TIME_STEP) ;   
    t+= TIME_STEP;

     printf("t: %f\n", t);
     if ( t > 2000 ) {
       r_direction*=-1.0;
     }
     if( t > 4000) {
       r_direction = 1.0;
       t = 0.0;
     }
  }
  
  delete robot;

 return 0;
}