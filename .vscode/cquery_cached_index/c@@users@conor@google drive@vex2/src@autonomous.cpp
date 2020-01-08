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
 void driveFWD(int distance) {
   // -=+=- Motor setup -=+=- //
 	pros::Motor left_front_mtr(20); // forward
 	pros::Motor right_front_mtr(19); // reverse
 	pros::Motor left_back_mtr(10); // forward
 	pros::Motor right_back_mtr(9); // reverse
 	right_back_mtr.set_reversed(true);
 	right_front_mtr.set_reversed(true);

  double wheelDiameterCM = 10.16;
  double circumference = wheelDiameterCM * M_PI;
  double degreesToRotate = (360 * distance) /circumference;
  double ticksToRoate = (degreesToRotate / 360) * 900;

  left_front_mtr.move_relative(ticksToRoate, 127);
  right_front_mtr.move_relative(ticksToRoate, 127);
  right_back_mtr.move_relative(ticksToRoate, 127);
  left_back_mtr.move_relative(ticksToRoate, 127);

  pros::delay(3000);
 }

 void driveSIDE(int distance) { // right is positive
   // -=+=- Motor setup -=+=- //
 	pros::Motor left_front_mtr(20); // forward
 	pros::Motor right_front_mtr(19); // reverse
 	pros::Motor left_back_mtr(10); // forward
 	pros::Motor right_back_mtr(9); // reverse
 	right_back_mtr.set_reversed(true);
 	right_front_mtr.set_reversed(true);
  double ticksToRoate = distance * 45;

  left_front_mtr.move_relative( - ticksToRoate, 127);
  right_front_mtr.move_relative(ticksToRoate, 127);
  right_back_mtr.move_relative( - ticksToRoate, 127);
  left_back_mtr.move_relative(ticksToRoate, 127);

  pros::delay(3000);
 }

void autonomous() {
  driveFWD(100);
  driveSIDE(100);
  driveFWD(-100);
  driveSIDE(-100);
}
