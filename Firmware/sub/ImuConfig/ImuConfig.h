/**
 * @file ImuConfig.h
 * @brief Namespace for IMU calibration constants
 * @author Dan Oates (WPI Class of 2020)
 */
#pragma once

/**
 * Namespace Declaration
 */
namespace ImuConfig
{
	const extern float gyr_x_cal;	// Gyroscope x offset [rad/s]
	const extern float gyr_y_cal;	// Gyroscope y offset [rad/s]
	const extern float gyr_z_cal;	// Gyroscope z offset [rad/s]
	const extern float gyr_x_var;	// Gyroscope x variance [(rad/s)^2]
	const extern float gyr_y_var;	// Gyroscope y variance [(rad/s)^2]
	const extern float gyr_z_var;	// Gyroscope z variance [(rad/s)^2]
	const extern float acc_x_var;	// Accelerometer x variance [(m/s^2)^2]
	const extern float acc_y_var;	// Accelerometer y variance [(m/s^2)^2]
	const extern float acc_z_var;	// Accelerometer z variance [(m/s^2)^2]
}