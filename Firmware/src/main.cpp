/**
 * @file main.cpp
 * @brief Main control code for BalBot
 * @author Dan Oates (WPI Class of 2020)
 */

// External Libraries
#include <Arduino.h>
#include <Timer.h>

// Project Libraries
#include <Bluetooth.h>
#include <Imu.h>
#include <MotorL.h>
#include <MotorR.h>
#include <MotorConfig.h>
#include <Controller.h>
using MotorConfig::Vb;
using Controller::t_ctrl;

// Global Variables
uint32_t loop_count = 0;	// Control loop counter
Timer timer;				// Controller timer

/**
 * @brief Initializes Balbot.
 */
void setup()
{
	// Initialize subsystems
	Bluetooth::init();
	Imu::init();
	MotorL::init();
	MotorR::init();
	Controller::init();

#if defined(CALIBRATE_IMU)

	// Print IMU calibration values
	Imu::calibrate();
	while(1);

#endif

	// Start loop timing
	timer.start();
}

/**
 * Balbot Control Loop.
 */
void loop()
{
	// Reset loop timer
	timer.reset();

	// Update subsystems
	Bluetooth::update();
	Imu::update();
	MotorL::update();
	MotorR::update();
	Controller::update();

#if defined(SERIAL_DEBUG)

	// Print debug info to serial
	MotorL::set_voltage(0.0f);
	MotorR::set_voltage(0.0f);
	if (loop_count % 25 == 0)
	{
		Serial.println("Motor L Angle [rad]: " + String(MotorL::get_angle(), 2));
		Serial.println("Motor R Angle [rad]: " + String(MotorR::get_angle(), 2));
		Serial.println("Pitch Angle [rad]: " + String(Imu::get_pitch(), 2));
		Serial.println("Voltage L [V]: " + String(Controller::get_motor_L_cmd(), 2));
		Serial.println("Voltage R [V]: " + String(Controller::get_motor_R_cmd(), 2));
		Serial.println();
	}

#elif defined(MOTOR_SPEED_TEST)

	// Send max motor voltages and print velocities
	MotorL::set_voltage(MotorConfig::Vb);
	MotorR::set_voltage(MotorConfig::Vb);
	if (loop_count % 25 == 0)
	{
		Serial.println("Velocities [rad/s]:");
		Serial.println("L: " + String(MotorL::get_velocity(), 2));
		Serial.println("R: " + String(MotorR::get_velocity(), 2));
		Serial.println();
	}

#elif defined(GET_MAX_CTRL_FREQ)

	// Estimate maximum possible control frequency
	const float f_ctrl_max = 1.0f / timer.read();
	MotorL::set_voltage(0.0f);
	MotorR::set_voltage(0.0f);
	Serial.println("GET_MAX_CTRL_FREQ");
	Serial.println("Max ctrl freq: " + String(f_ctrl_max));
	while(1);

#else

	// Send voltage commands to motors
	MotorL::set_voltage(Controller::get_motor_L_cmd());
	MotorR::set_voltage(Controller::get_motor_R_cmd());

#endif

	// Maintain loop timing
	loop_count++;
	while (timer.read() < t_ctrl);
}