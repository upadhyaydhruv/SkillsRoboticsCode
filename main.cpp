/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       main.cpp                                                  */
/*    Author:       C:\Users\4248                                             */
/*    Created:      Tue Mar 03 2020                                           */
/*    Description:  V5 project                                                */
/*                                                                            */
/*----------------------------------------------------------------------------*/

// ---- START VEXCODE CONFIGURED DEVICES ----
// Robot Configuration:
// [Name]               [Type]        [Port(s)]
// Drivetrain           drivetrain    9, 2            
// ArmMotor             motor         8               
// ClawMotor            motor         3               
// LeftTracker          line          A               
// RightTracker         line          B               
// MiddleTracker        line          C               
// ---- END VEXCODE CONFIGURED DEVICES ----

#include "vex.h"

using namespace vex;

class LightSensors {
public:
  LightSensors(line& left, line& middle, line& right, int threshold);
  bool leftOn();
  bool middleOn();
  bool rightOn();
  void setThreshold(int threshold);

private:
  line& left;
  line& middle;
  line& right;
  int threshold;
};

LightSensors::LightSensors(line& left, line& middle, line& right, int threshold)
  : left(left), middle(middle), right(right), threshold(threshold) {
  
};

bool LightSensors::leftOn() {
  return this->left.reflectivity() > this->threshold;
}

bool LightSensors::middleOn() {
  return this->middle.reflectivity() > this->threshold;
}

bool LightSensors::rightOn() {
  return this->right.reflectivity() > this->threshold;
}

void LightSensors::setThreshold(int threshold) {
  this->threshold = threshold;
}

class PickerUpper {
public:
  PickerUpper(motor& claw, motor& arm);
  void pickUp();
  void putDown();
  bool getIsUp();

private:
  motor& claw;
  motor& arm;
  bool isUp;
};

PickerUpper::PickerUpper(motor& claw, motor& arm)
: claw(claw), arm(arm) {

}

void PickerUpper::pickUp() {
  if (this->isUp)
    return;
  
  // TODO: remember to ensure it is blocking (i.e. it waits for it to finish) here
  Drivetrain.stop();
  ArmMotor.spinFor(60, degrees);

  this->isUp = true;
}

void PickerUpper::putDown() {
  if (!this->isUp)
    return;

  // TODO: remember to ensure it is blocking (i.e. it waits for it to finish) here
  Drivetrain.stop();
  ClawMotor.spinFor(20, degrees);
  Drivetrain.driveFor(-1, inches);
  ArmMotor.spinFor(-60, degrees);
  this->isUp = false;
}

bool PickerUpper::getIsUp() {
  return this->isUp;
}

int main() {
  // Initializing Robot Configuration. DO NOT REMOVE!
  vexcodeInit();
    
  LightSensors ls(LeftTracker, MiddleTracker, RightTracker, 90);
  PickerUpper pu(ClawMotor, ArmMotor);

  // for testing; remove this after implementing picking up automatically
  pu.pickUp();

  while (true) {  
    // check if it's at the end, if so, do an action (blocking, i.e. wait for it to finish), and "continue" the loop
    while (true){
      // read the sensors
      bool leftOn = ls.leftOn();
      bool rightOn = ls.rightOn();
      bool middleOn = ls.middleOn();
      while (leftOn&&!middleOn){
        Drivetrain.setTurnVelocity(10, rpm);
        Drivetrain.turn(right);
      }

      while (rightOn&&!middleOn){
        Drivetrain.setTurnVelocity(10, rpm);
        Drivetrain.turn(left);
      }

      while(middleOn){
        Drivetrain.setDriveVelocity(10, rpm);
        Drivetrain.drive(forward);
      }
      if (leftOn && middleOn && rightOn) {
          break;
        }
      }

      if (pu.getIsUp()) {
          // THIS SHOULD NEVER HAPPEN, show an error
          Brain.Screen.print("ERROR - pickerUpper is up");
          pu.putDown();
      pu.pickUp();

      Drivetrain.turnFor(180, degrees);

    while (true){
      // read the sensors
      bool leftOn = ls.leftOn();
      bool rightOn = ls.rightOn();
      bool middleOn = ls.middleOn();
      while (leftOn&&!middleOn){
        Drivetrain.setTurnVelocity(10, rpm);
        Drivetrain.turn(right);
      }

      while (rightOn&&!middleOn){
        Drivetrain.setTurnVelocity(10, rpm);
        Drivetrain.turn(left);
      }

      while(middleOn){
        Drivetrain.setDriveVelocity(10, rpm);
        Drivetrain.drive(forward);
      }
      if (leftOn && middleOn && rightOn) {
        break;
      }
    }

    //Assuming in front of tee, put the ball down.
    pu.putDown();

    //At this point, the first ball should have been placed in the first green

    Drivetrain.turnFor(90, degrees); //Turning right towards line leading to the next tee
    // read the sensors
    bool leftOn = ls.leftOn();
    bool rightOn = ls.rightOn();
    bool middleOn = ls.middleOn();
    while (!leftOn||!rightOn||!middleOn){
      Drivetrain.drive(forward);
    }

    while (true) {
      // read the sensors
      bool leftOn = ls.leftOn();
      bool rightOn = ls.rightOn();
      bool middleOn = ls.middleOn();
      while (!middleOn&&leftOn){
        Drivetrain.turn(right);
      }
      while(!middleOn&&rightOn){
        Drivetrain.turn(left);
      }
      if (!leftOn&&!middleOn&&!rightOn)
        break;
    }

    while (true){
      // read the sensors
      bool leftOn = ls.leftOn();
      bool rightOn = ls.rightOn();
      bool middleOn = ls.middleOn();

      while (!leftOn||!middleOn||!rightOn) Drivetrain.drive(forward);
      Drivetrain.turn(left); //Repositions robot to the left of the line leading to second green

      while(!rightOn){ //Robot turns right until it finds line again
        Drivetrain.turn(right);
      }

      while (true) { //Robot continues to follow line until it encounters horizontal line and 2nd green
        // read the sensors
        bool leftOn = ls.leftOn();
        bool rightOn = ls.rightOn();
        bool middleOn = ls.middleOn();

        while (leftOn&&!middleOn){
        Drivetrain.setTurnVelocity(10, rpm);
        Drivetrain.turn(right);
      }

      while (rightOn&&!middleOn){
        Drivetrain.setTurnVelocity(10, rpm);
        Drivetrain.turn(left);
      }

      while(middleOn){
        Drivetrain.setDriveVelocity(10, rpm);
        Drivetrain.drive(forward);
      }
      if (leftOn && middleOn && rightOn) {
          break;
        }
      }

      if (pu.getIsUp()) {
          // THIS SHOULD NEVER HAPPEN, show an error
          Brain.Screen.print("ERROR - pickerUpper is up");
          pu.putDown();
      pu.pickUp();

      Drivetrain.turnFor(180, degrees);

    while (true){
      // read the sensors
      bool leftOn = ls.leftOn();
      bool rightOn = ls.rightOn();
      bool middleOn = ls.middleOn();
      
      while (leftOn&&!middleOn){
        Drivetrain.setTurnVelocity(10, rpm);
        Drivetrain.turn(right);
      }

      while (rightOn&&!middleOn){
        Drivetrain.setTurnVelocity(10, rpm);
        Drivetrain.turn(left);
      }

      while(middleOn){
        Drivetrain.setDriveVelocity(10, rpm);
        Drivetrain.drive(forward);
      }
      if (leftOn && middleOn && rightOn) {
        break;
      }
    }

    //Assuming in front of tee, put the ball down.
    pu.putDown();

    //At this point, the second ball should have been placed in the second green

    Drivetrain.turnFor(-90, degrees);

    while(leftOn||rightOn||middleOn){
      Drivetrain.drive(forward); //Robot drives until it finds the line leading to third tee
    }

    Drivetrain.turnFor(90, degrees); //Robot turns right towards the tee
     
    //CONTINUE WORK FROM HERE!!
      }

    }

    

      // for testing:
      Drivetrain.driveFor(5, inches);

      // remember to turn if necessary here, as the line following loop will continue
      continue; // read the sensors again before continuing
    }
    // check any other conditions, same thing, "continue" after
    
    Drivetrain.setDriveVelocity(20, rpm);
    Drivetrain.drive(forward);

    // START moving the motor to follow the line (i.e. don't wait for it to finish moving here, just set the speed)    
  
  }

  while (ls.leftOn()&&!ls.middleOn()&&!ls.rightOn()){
    Drivetrain.turn(left);
  }
  while (!ls.leftOn()&&!ls.middleOn()&&ls.rightOn()){
    Drivetrain.turn(right);
  }
  while ((ls.leftOn()&&ls.middleOn())||ls.leftOn()) {
    if(ls.rightOn()){
      Drivetrain.turn(left);
    }
  }
}
