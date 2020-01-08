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

int averageArray(int length, int array[]) {
	int total = 0;
	for (int i = 0; i < length; i++) {
		total = total + array[i];
	}
	return (total / length);
}

void opcontrol() {
	pros::Controller master(pros::E_CONTROLLER_MASTER);
	// -=+=- Motor setup -=+=- //
	pros::Motor left_front_mtr(20); // forward
	pros::Motor right_front_mtr(19); // reverse
	pros::Motor left_back_mtr(10); // forward
	pros::Motor right_back_mtr(9); // reverse
	right_back_mtr.set_reversed(true);
	right_front_mtr.set_reversed(true);

	pros::Vision vision_sensor(11, pros::E_VISION_ZERO_CENTER);

	int threshold = 20;
	int joysticks [4] = {0, 0, 0, 0};
	int camPos [20]  = {0};
	int camCurrent = 0;

	while (true) {
		// -=+=- Drive Control -=+=- //
		joysticks[0] = master.get_analog(ANALOG_LEFT_X); // joysticks[0]
		joysticks[1] = master.get_analog(ANALOG_LEFT_Y); // joysticks[1]
		joysticks[2] = master.get_analog(ANALOG_RIGHT_Y); // joysticks[2]
		joysticks[3] = master.get_analog(ANALOG_RIGHT_X); // joysticks[3]

		int i = 0;
		for (i = 0; i < 4; i++) {
			if (abs(joysticks[i]) < threshold) {
				joysticks[i] = 0;
			}
		}

		left_front_mtr.move( (joysticks[2] + joysticks[0] - joysticks[3]) );
		right_front_mtr.move( (joysticks[2] - joysticks[0] + joysticks[3]) );
		left_back_mtr.move( (joysticks[2] + joysticks[0] + joysticks[3]) );
		right_back_mtr.move( (joysticks[2] - joysticks[0] - joysticks[3]) );

		if (master.get_digital(DIGITAL_B)) {
			autonomous();
		}

		pros::vision_object_s_t rtn = vision_sensor.get_by_size(0);

		if (rtn.signature == 1) {
			pros::lcd::print(0, "Current: %d", rtn.x_middle_coord);
			camPos[camCurrent] = rtn.x_middle_coord;
			pros::lcd::print(2, "Current: %d", camPos[camCurrent]);
			camCurrent++;
			if (camCurrent == 20) {
				camCurrent = 0;
			}
		}
		else {
			pros::lcd::print(2, "                 ");
		}

		int currentAVG = averageArray(20, camPos);
		pros::lcd::print(1,"Vision Sensor: %d", currentAVG);

		if (master.get_digital(DIGITAL_A)) {
			if (currentAVG > 50) {
				left_front_mtr.move(80);
				right_front_mtr.move(-80);
				left_back_mtr.move(80);
				right_back_mtr.move(-80);
			}
			else if (currentAVG < -50) {
				left_front_mtr.move(-80);
				right_front_mtr.move(80);
				left_back_mtr.move(-80);
				right_back_mtr.move(80);
			}
			else {
				left_front_mtr.move(40);
				right_front_mtr.move(40);
				left_back_mtr.move(40);
				right_back_mtr.move(40);
			}
		}

		pros::delay(20);
	}
}
