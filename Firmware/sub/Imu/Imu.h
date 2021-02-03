/**
 * @file Imu.h
 * @brief Subsystem for IMU-based state estimation
 * @author Dan Oates (WPI Class of 2020)
 */
#pragma once

/**
 * Namespace Declaration
 */
namespace Imu
{
	void init();
	void update();
	float get_pitch();
	float get_pitch_vel();
	float get_yaw_vel();
	void calibrate();
}