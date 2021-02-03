/**
 * @file Imu.cpp
 * @author Dan Oates (WPI Class of 2020)
 */
#include <Imu.h>
#include <ImuConfig.h>
#include <MPU6050.h>
#include <Controller.h>
#include <DigitalOut.h>
#include <GRV.h>
using Controller::t_ctrl;

namespace Imu
{
	// IMU Hardware Interface
	TwoWire* const wire = &Wire;
	MPU6050 imu(wire);

	// State Variables
	bool first_frame = true;
	GRV pitch, pitch_vel;
	float yaw_vel;

	// Error LED
	const uint8_t pin_led = 13;
	DigitalOut led(pin_led);

	// Init Flag
	bool init_complete = false;
}

/**
 * @brief Initializes I2C and IMU
 */
void Imu::init()
{
	if (!init_complete)
	{
		// Init I2CC
		wire->begin();

		// Init IMU
		bool success = imu.init();
		led = !success;
		if (!success) while(1);
		imu.gyr_x_cal = ImuConfig::gyr_x_cal;
		imu.gyr_y_cal = ImuConfig::gyr_y_cal;
		imu.gyr_z_cal = ImuConfig::gyr_z_cal;

		// Set init flag
		init_complete = true;
	}
}

/**
 * @brief Reads IMU and updates state estimates
 */
void Imu::update()
{
	// Get new readings from IMU
	imu.update();
	const float acc_y = imu.get_acc_y();
	const float acc_z = imu.get_acc_z();
	const float gyr_x = imu.get_gyr_x();
	const float gyr_y = imu.get_gyr_y();
	const float gyr_z = imu.get_gyr_z();

	// Estimate pitch from accelerometer
	GRV grv_acc_y(acc_y, ImuConfig::acc_y_var);
	GRV grv_acc_z(acc_z, ImuConfig::acc_z_var);
	GRV pitch_acc = atan2(grv_acc_y, grv_acc_z);

	// Check special first frame condition
	if(first_frame)
	{
		// Use accelerometer only
		first_frame = false;
		pitch = pitch_acc;
	}
	else
	{
		// Fuse accelerometer and gyro integration
		pitch_vel = GRV(gyr_x, ImuConfig::gyr_x_var);
		GRV pitch_gyr = pitch + pitch_vel * t_ctrl;
		pitch = fuse(pitch_gyr, pitch_acc);
	}

	// Yaw velocity estimation
	yaw_vel =
		gyr_z * cosf(pitch.mean) +
		gyr_y * sinf(pitch.mean);
}

/**
 * @brief Returns IMU pitch estimate computed via Kalman filter
 */
float Imu::get_pitch()
{
	return pitch.mean;
}

/**
 * @brief Returns IMU pitch velocity measurement
 */
float Imu::get_pitch_vel()
{
	return pitch_vel.mean;
}

/**
 * @brief Returns IMU yaw velocity estimate
 */
float Imu::get_yaw_vel()
{
	return yaw_vel;
}

/**
 * @brief Calibrates IMU and prints values to Serial
 */
void Imu::calibrate()
{
	imu.calibrate();
	Serial.println("IMU Calibration Code:");
	Serial.println("const float gyr_x_cal = " + String(imu.gyr_x_cal, 13) + "f;");
	Serial.println("const float gyr_y_cal = " + String(imu.gyr_y_cal, 13) + "f;");
	Serial.println("const float gyr_z_cal = " + String(imu.gyr_z_cal, 13) + "f;");
	Serial.println("const float gyr_x_var = " + String(imu.get_gyr_x_var(), 14) + "f;");
	Serial.println("const float gyr_y_var = " + String(imu.get_gyr_y_var(), 14) + "f;");
	Serial.println("const float gyr_z_var = " + String(imu.get_gyr_z_var(), 14) + "f;");
	Serial.println("const float acc_x_var = " + String(imu.get_acc_x_var(), 14) + "f;");
	Serial.println("const float acc_y_var = " + String(imu.get_acc_y_var(), 14) + "f;");
	Serial.println("const float acc_z_var = " + String(imu.get_acc_z_var(), 14) + "f;");
}