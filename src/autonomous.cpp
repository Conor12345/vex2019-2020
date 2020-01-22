#include "main.h"

/**
 * Runs the user autonomous code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the autonomous
 * mode. Alternatively, this function may be called in initialize or opcontrol
 * for non-competition testing purposes.
 *
 * If the robot is disabled or communications is lost, the autonomous task
 * will be stopped. Re-enabling the robot will restart the task, not re-start it
 * from where it left off.
 */

 // 900 ticks per revolution
 void driveFWD(int distance, int speed = 80) {
   // -=+=- Motor setup -=+=- //
 	pros::Motor left_front_mtr(LEFTFRONTMTR); // forward
 	pros::Motor right_front_mtr(RIGHTFRONTMTR); // reverse
 	right_front_mtr.set_reversed(true);

  double wheelDiameterCM = 10.16;
  double circumference = wheelDiameterCM * M_PI;
  double degreesToRotate = (360 * distance) /circumference;
  double ticksToRoate = (degreesToRotate / 360) * 900;

  left_front_mtr.move_relative(ticksToRoate, speed);
  right_front_mtr.move_relative(ticksToRoate, speed);

  pros::delay(3000);
 }

 void turn(int degrees, int speed = 60) { // right = +1, left = -1
   // -=+=- Motor setup -=+=- //
   pros::Motor left_front_mtr(LEFTFRONTMTR); // forward
   pros::Motor right_front_mtr(RIGHTFRONTMTR); // reverse
   right_front_mtr.set_reversed(true);

  double ticksToRoate = degrees * 45;

  left_front_mtr.move_relative((ticksToRoate), speed);
  right_front_mtr.move_relative( (ticksToRoate), speed);
 }

 void lift(int distance) { // postive is up
   pros::Motor left_lift_mtr(LEFTLIFTMTR); // positive is upards
 	 pros::Motor right_lift_mtr(RIGHTLIFTMTR);
 	 left_lift_mtr.set_reversed(true);

   left_lift_mtr.move_relative(distance * 50, 80);
   right_lift_mtr.move_relative(distance * 50, 80);
 }

 void moveRamp(int degrees) {
  double degreesToMove = degrees * 7;

  pros::Motor ramp_mtr(RAMPMTR);
  ramp_mtr.set_reversed(true);

  ramp_mtr.move_relative(degreesToMove, 90);
 }

 void spinIntake(double time) {
  pros::Motor left_intake_mtr2(LEFTINTAKEMTR);
 	pros::Motor right_intake_mtr2(RIGHTINTAKEMTR);
 	right_intake_mtr2.set_reversed(true);

 }

void frontAuton(bool red) {
  int dir = 0;
  if (red == true){
    int dir = 1; }
  else {
    int dir = -1; }
  driveFWD(100);
  driveFWD(-50);
  moveRamp(100);
  turn(-30 * dir);
}

void backAuton(bool red) {
  if (red == true){
    int dir = 1; }
  else {
    int dir = -1; }
  driveFWD(50);
  driveFWD(-50);

  pros::Motor right_intake_mtr2(LEFTINTAKEMTR);
 	right_intake_mtr2.set_reversed(true);
  right_intake_mtr2.move(80);
  pros::delay(500);
  right_intake_mtr2.move(0);
 }

void testAuton() {
  moveRamp(130);
  pros::Motor right_intake_mtr2(LEFTINTAKEMTR);
 	right_intake_mtr2.set_reversed(true);
  right_intake_mtr2.move(80);
  pros::delay(500);
  right_intake_mtr2.move(0);
}

void autonomous() {
  pros::delay(200);
  pros::lcd::clear_line(1);
  pros::ADIPotentiometer autonPot(1);
  pros::delay(200);
  // -=+=- Autonomous Selector -=+=- //
  int potValue = autonPot.get_value() / 40.96;
  pros::lcd::print(2, "Pot: %d", potValue);
  if (potValue < 20) {
    pros::lcd::print(1, "Auton: Red Front");
    frontAuton(true);
  }
  else if (potValue < 40) {
    pros::lcd::print(1, "Auton: Red Back");
    backAuton(true);
  }
  else if (potValue < 60) {
    pros::lcd::print(1, "Auton: Blue Front");
    frontAuton(false);
  }
  else if (potValue < 80) {
    pros::lcd::print(1, "Auton: Blue Back");
    backAuton(false);
  }
  else if (potValue < 95) {
    pros::lcd::print(1, "Auton: Testing");
    testAuton();
  }
  else {
    pros::lcd::print(1, "None");
  }
}
