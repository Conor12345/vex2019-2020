#include "main.h"
#include "sys/time.h"
#include <string>
#include "chrono"

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
	pros::Controller controller(pros::E_CONTROLLER_MASTER);

	// -=+=- Motor setup -=+=- //
	pros::Motor left_front_mtr(LEFTFRONTMTR); // forward
	pros::Motor right_front_mtr(RIGHTFRONTMTR); // reverse
	right_front_mtr.set_reversed(true);

	pros::Motor left_lift_mtr(LEFTLIFTMTR); // positive is upards
	pros::Motor right_lift_mtr(RIGHTLIFTMTR);
	left_lift_mtr.set_reversed(true);

	pros::Motor left_intake_mtr(LEFTINTAKEMTR);
	pros::Motor right_intake_mtr(RIGHTINTAKEMTR);
	right_intake_mtr.set_reversed(true);

	pros::Motor ramp_mtr(RAMPMTR);
	ramp_mtr.set_brake_mode(MOTOR_BRAKE_HOLD);

	pros::ADIPotentiometer autonPot(1);

	int threshold = 20;
	int joysticks [4] = {0, 0, 0, 0};
	float multiplier = 1.0;
	bool slow = false;

	std::string auton = "";

	while (true) {
		// -=+=- Drive Control -=+=- //
		if (slow) {
			multiplier = 0.5;
		}
		else {
			multiplier = 1.0;
		}

		if (controller.get_digital(DIGITAL_UP)) {
			slow = true;
		}

		if (controller.get_digital(DIGITAL_DOWN)) {
			slow = false;
		}

		joysticks[0] = controller.get_analog(ANALOG_RIGHT_X); // joysticks[0]
		joysticks[1] = controller.get_analog(ANALOG_RIGHT_Y); // joysticks[1]
		joysticks[2] = controller.get_analog(ANALOG_LEFT_Y); // joysticks[2]
		joysticks[3] = controller.get_analog(ANALOG_LEFT_X); // joysticks[3]

		right_front_mtr.move( (joysticks[2] - joysticks[3]));
		left_front_mtr.move( (joysticks[2] + joysticks[3]));

		// -=+=- Lift Control -=+=- //
		left_lift_mtr.move(joysticks[1]);
		right_lift_mtr.move(joysticks[1]);

		// -=+=- Intake Control -=+=- //
		if (controller.get_digital(DIGITAL_R2)) {
			left_intake_mtr.move(75 * multiplier);
			right_intake_mtr.move(75 * multiplier);
		}
		else if (controller.get_digital(DIGITAL_R1)){
			left_intake_mtr.move(-127 * multiplier);
			right_intake_mtr.move(-127 * multiplier);
		}
		else {
			left_intake_mtr.move(0);
			right_intake_mtr.move(0);
		}

		// -=+=- Intake Control -=+=- //
		if (controller.get_digital(DIGITAL_L1)) {
			ramp_mtr.move(50);
		}
		else if (controller.get_digital(DIGITAL_L2)){
			ramp_mtr.move(-50);
		}
		else {
			ramp_mtr.move(0);
		}

		// -=+=- Autonomous Selector -=+=- //
		int potValue = autonPot.get_value() / 40.96;
		pros::lcd::print(2, "Pot: %d", potValue);
		if (potValue < 20) {
			auton =  "Auton: Red Front";
		}
		else if (potValue < 40) {
			auton =  "Auton: Red Back";
		}
		else if (potValue < 60) {
			auton =  "Auton: Blue Front";
		}
		else if (potValue < 80) {
			auton =  "Auton: Blue Back";
		}
		else if (potValue < 95) {
			auton =  "Auton: Testing";
		}
		else {
			auton =  "None";
		}

		pros::lcd::print(1, "Auton: %s %d", auton, potValue);
		pros::lcd::print(2, "Left Lift Temp %f", left_lift_mtr.get_temperature());
		pros::lcd::print(3, "Right Lift Temp %f", right_lift_mtr.get_temperature());
		pros::lcd::print(4, "Left Drive Temp %f", left_front_mtr.get_temperature());
		pros::lcd::print(5, "Right Drive Temp %f", right_front_mtr.get_temperature());

		pros::delay(20);
	}
}
