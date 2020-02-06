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
 void driveFWD(int distance, int speed = 120) {
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

  pros::delay(abs(distance) * 20);
 }

 void turn(int degrees, int speed = 60) { // right = +1, left = -1
   // -=+=- Motor setup -=+=- //
   pros::Motor left_front_mtr(LEFTFRONTMTR); // forward
   pros::Motor right_front_mtr(RIGHTFRONTMTR); // reverse
   right_front_mtr.set_reversed(true);

  double ticksToRotate = degrees * 9.2;

  left_front_mtr.move_relative((ticksToRotate), speed);
  right_front_mtr.move_relative( - (ticksToRotate), speed);
 }

 void lift(int distance) { // postive is up
   pros::Motor left_lift_mtr(LEFTLIFTMTR); // positive is upards
 	 pros::Motor right_lift_mtr(RIGHTLIFTMTR);
 	 left_lift_mtr.set_reversed(true);

   left_lift_mtr.move_relative(distance * 50, 80);
   right_lift_mtr.move_relative(distance * 50, 80);
 }

 void driveLift(int dir) { // +1 up -1 down
   pros::Motor left_lift_mtr(LEFTLIFTMTR); // positive is upards
 	 pros::Motor right_lift_mtr(RIGHTLIFTMTR);
 	 left_lift_mtr.set_reversed(true);

   left_lift_mtr.move(80 * dir);
   right_lift_mtr.move(80 * dir);

 }

 void stopLift(int dir) { // +1 up -1 down
   pros::Motor left_lift_mtr(LEFTLIFTMTR); // positive is upards
    pros::Motor right_lift_mtr(RIGHTLIFTMTR);
    left_lift_mtr.set_reversed(true);

   left_lift_mtr.move(0);
   right_lift_mtr.move(0);

 }

 void moveRamp(int degrees) { // Pos tilts more vertical neg tilts more horizontal
  double degreesToMove = degrees * 20;

  pros::Motor ramp_mtr(RAMPMTR);
  ramp_mtr.set_reversed(true);

  ramp_mtr.move_relative(degreesToMove, 90);
 }

 void spinIntake(int dir, int speed = 100) { // -1 cubes move up +1 cubes move down
  pros::Motor left_intake_mtr(LEFTINTAKEMTR);
 	pros::Motor right_intake_mtr(RIGHTINTAKEMTR);
 	right_intake_mtr.set_reversed(true);

  right_intake_mtr.move(speed * dir);
  left_intake_mtr.move(speed * dir);
}

void stopIntake() {
  pros::Motor left_intake_mtr(LEFTINTAKEMTR);
 	pros::Motor right_intake_mtr(RIGHTINTAKEMTR);
 	right_intake_mtr.set_reversed(true);

  right_intake_mtr.move(0);
  left_intake_mtr.move(0);
 }

 void flipOutIntake() {
  moveRamp(30);
  pros::delay(200);
  pros::Motor left_intake_mtr(LEFTINTAKEMTR);
  left_intake_mtr.move(127);
  pros::delay(500);
  left_intake_mtr.move(0);
 }

 void moveLift(int time, int dir) { //dir +1 up -1 down
  pros::Motor left_lift_mtr(LEFTLIFTMTR); // positive is upards
 	pros::Motor right_lift_mtr(RIGHTLIFTMTR);
 	left_lift_mtr.set_reversed(true);

  left_lift_mtr.move(100 * dir);
  right_lift_mtr.move(100 * dir);
  pros::delay(time);
  left_lift_mtr.move(0);
  right_lift_mtr.move(0);
 }

void frontAuton() { // 100cm in 3s 20cm 600 10cm 300
  driveFWD(-50);
  driveFWD(50);
  flipOutIntake();
}

void redBackAuton() {
  driveFWD(30);
  driveFWD(-30, 70);
  flipOutIntake();
  moveRamp(-30);
  pros::delay(500);
  spinIntake(-1);
  driveFWD(80, 60);
  pros::delay(1000);
  stopIntake();
  driveFWD(-90);
  turn(-100);
  pros::delay(1000);
  driveFWD(50);
  spinIntake(1, 50);
  pros::delay(500);
  stopIntake();
  moveLift(1500, 1);
  spinIntake(1, 50);
  pros::delay(1000);
  stopIntake();
  driveFWD(-20);
 }

 void blueBackAuton() {
   driveFWD(30);
   driveFWD(-30, 70);
   flipOutIntake();
   moveRamp(-30);
   pros::delay(500);
   spinIntake(-1);
   driveFWD(80, 60);
   pros::delay(1000);
   stopIntake();
   driveFWD(-90);
   turn(100);
   pros::delay(1000);
   driveFWD(50);
   spinIntake(1, 50);
   pros::delay(500);
   stopIntake();
   moveLift(1500, 1);
   spinIntake(1, 50);
   pros::delay(1000);
   stopIntake();
   driveFWD(-20);
  }

void testAuton() {
  flipOutIntake();
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
    frontAuton();
  }
  else if (potValue < 40) {
    pros::lcd::print(1, "Auton: Red Back");
    redBackAuton();
  }
  else if (potValue < 60) {
    pros::lcd::print(1, "Auton: Blue Front");
    frontAuton();
  }
  else if (potValue < 80) {
    pros::lcd::print(1, "Auton: Blue Back");
    blueBackAuton();
  }
  else if (potValue < 95) {
    pros::lcd::print(1, "Auton: Testing");
    testAuton();
  }
  else {
    pros::lcd::print(1, "None");
  }
}
