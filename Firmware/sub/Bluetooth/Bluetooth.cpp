/**
 * @file Bluetooth.cpp
 * @author Dan Oates (WPI Class of 2020)
 */
#include <Bluetooth.h>
#include <Imu.h>
#include <Controller.h>
#include <SerialStruct.h>

/**
 * Namespace Definitions
 */
namespace Bluetooth
{
	// Hardware interfaces
	SerialStruct serial(&Serial);
	const uint32_t baud = 57600;

	// Received commands
	float lin_vel_cmd = 0.0f;	// Linear velocity [m/s]
	float yaw_vel_cmd = 0.0f;	// Yaw velocity [rad/s]

	// Init flag
	bool init_complete = false;
}

/**
 * @brief Initializes Bluetooth module and serial
 */
void Bluetooth::init()
{
	if (!init_complete)
	{
		// Init dependent subsystems
		Imu::init();

		// Init serial
		Serial.begin(baud);
		serial.flush();

		// Set init flag
		init_complete = true;
	}
}

/**
 * @brief Checks Bluetooth serial buffer for commands
 */
void Bluetooth::update()
{
	if(Serial.available() >= 8)
	{
		serial.rx(lin_vel_cmd);
		serial.rx(yaw_vel_cmd);
		serial.tx(Controller::get_lin_vel());
		serial.tx(Imu::get_yaw_vel());
		serial.tx(Controller::get_motor_L_cmd());
		serial.tx(Controller::get_motor_R_cmd());
	}
}

/**
 * @brief Returns linear velocity command [m/s]
 */
float Bluetooth::get_lin_vel_cmd()
{
	return lin_vel_cmd;
}

/**
 * @brief Returns yaw velocity command [rad/s]
 */
float Bluetooth::get_yaw_vel_cmd()
{
	return yaw_vel_cmd;
}