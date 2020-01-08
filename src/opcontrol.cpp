#include "main.h"

/**
 * Runs the operator control code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the operator
 * control mode.
 *
 * If no competition control is connected, this function will run immediately
 * following initialize().
 *
 * If the robot is disabled or communications is lost, the
 * operator control task will be stopped. Re-enabling the robot will restart the
 * task, not resume it from where it left off.
 */

void opcontrol() {
	pros::Controller master(pros::E_CONTROLLER_MASTER);
	// -=+=- Motor setup -=+=- //
	pros::Motor left_front_mtr(LEFTFRONTMTR); // forward
	pros::Motor right_front_mtr(RIGHTFRONTMTR); // reverse
	pros::Motor left_back_mtr(LEFTBACKMTR); // forward
	pros::Motor right_back_mtr(RIGHTBACKMTR); // reverse
	right_back_mtr.set_reversed(true);
	right_front_mtr.set_reversed(true);

	pros::Motor left_lift_mtr(LEFTLIFTMTR); // positive is upards
	pros::Motor right_lift_mtr(RIGHTLIFTMTR);
	left_lift_mtr.set_reversed(true);

	pros::Motor intake_mtr(INTAKEMTR); // positive pulls cubes upwards

	pros::ADIPotentiometer autonPot(1);

	int threshold = 20;
	int joysticks [4] = {0, 0, 0, 0};
	float multiplier = 1.0;

	while (true) {
		// -=+=- Drive Control -=+=- //
		if (master.get_digital(DIGITAL_X)) {
			multiplier = 0.4;
		}
		else {
			multiplier = 1.0;
		}

		joysticks[0] = master.get_analog(ANALOG_RIGHT_X); // joysticks[0]
		joysticks[1] = master.get_analog(ANALOG_RIGHT_Y); // joysticks[1]
		joysticks[2] = master.get_analog(ANALOG_LEFT_Y); // joysticks[2]
		joysticks[3] = master.get_analog(ANALOG_LEFT_X); // joysticks[3]

		int i = 0;
		for (i = 0; i < 2; i++) {
			if (abs(joysticks[i]) < threshold) {
				joysticks[i] = 0;
			}
		}

		right_front_mtr.move( (joysticks[2] - joysticks[3]) * multiplier );
		right_back_mtr.move( (joysticks[2] + joysticks[3]) * multiplier );
		left_front_mtr.move( (joysticks[2] + joysticks[3]) * multiplier );
		left_back_mtr.move( (joysticks[2] - joysticks[3]) * multiplier );

		// -=+=- Lift Control -=+=- //
		left_lift_mtr.move(joysticks[1]);
		right_lift_mtr.move(joysticks[1]);

		// -=+=- Intake Control -=+=- //
		if (master.get_digital(DIGITAL_R1)) {
			intake_mtr.move(127);
		}
		else if (master.get_digital(DIGITAL_R2)){
			intake_mtr.move(-127);
		}
		else {
			intake_mtr.move(0);
		}

		// -=+=- Autonomous Selector -=+=- //
		int potValue = autonPot.get_value() / 40.96;
		pros::lcd::print(2, "Pot: %d", potValue);
		if (potValue < 20) {
			pros::lcd::print(1, "Auton: Red Front");
		}
		else if (potValue < 40) {
			pros::lcd::print(1, "Auton: Red Back");
		}
		else if (potValue < 60) {
			pros::lcd::print(1, "Auton: Blue Front");
		}
		else if (potValue < 80) {
			pros::lcd::print(1, "Auton: Blue Back");
		}
		else if (potValue < 95) {
			pros::lcd::print(1, "Auton: Testing");
		}
		else {
			pros::lcd::print(1, "None");
		}

		pros::delay(20);
	}
}
