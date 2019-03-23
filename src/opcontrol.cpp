#include "main.h"
#include <vector>
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
	pros::Motor Drive_FR(1);
	pros::Motor Drive_BR(2);
	pros::Motor Drive_FL(11,true);
	pros::Motor Drive_BL(12,true);
	pros::Motor Roller(13);
	pros::Motor Lift(14);
	pros::Motor Indexer(15);

  std::vector <pros::Motor> driveMotors={Drive_FR,Drive_FL,Drive_BR,Drive_BL};

	while (true) {
		int x=(master.get_analog(ANALOG_RIGHT_X)/127)*200; //makes motors go faster
		int y=(master.get_analog(ANALOG_RIGHT_Y)/127)*200;

		for(int i = 0; i < driveMotors.size(); i++) //Drive
		{
			pros::Motor m = driveMotors.at(i);
			if(i % 2 == 0) //Right motors
			{
				m.move_velocity(y + x);
			}
			else
			{
				m.move_velocity(y - x);
			}
		}

		if(master.get_digital(DIGITAL_L1)) //Roller intake
		{
			Roller.move_velocity(200);
			Indexer.move_velocity(200);
		}
		else if(master.get_digital(DIGITAL_L2)) //Cap flipper
		{
			Roller.move_velocity(-200);
		}

		if(master.get_digital(DIGITAL_R1)) //Lift up
		{
			Lift.move_velocity(200);
		}
		else if(master.get_digital(DIGITAL_R2)) //Lift down
		{
			Lift.move_velocity(-200);
		}

		if(master.get_digital(DIGITAL_A)) //Roller intake with indexer preventing ball going to flywheel
		{
			Roller.move_velocity(200);
			Indexer.move_velocity(-200);
		}
		pros::delay(20);
	}
}
