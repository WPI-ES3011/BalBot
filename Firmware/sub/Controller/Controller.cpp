/**
 * @file Controller.cpp
 * @author Dan Oates (WPI Class of 2020)
 */
#include <Controller.h>
#include <MotorConfig.h>
#include <Bluetooth.h>
#include <Imu.h>
#include <MotorL.h>
#include <MotorR.h>
#include <CppUtil.h>
#include <SlewLimiter.h>
#include <PID.h>
using MotorConfig::Vb;
using MotorConfig::Kv;
using MotorConfig::Kt;
using MotorConfig::R;
using CppUtil::clamp;

/**
 * Namespace Definitions
 */
namespace Controller
{

	// External Constants
	const float f_ctrl = 100.0f;
	const float t_ctrl = 1.0f / f_ctrl;
	
	// State Variables
	float lin_vel = 0.0f;		// Linear velocity [m/s]
	float lin_vel_cmd = 0.0f;	// Linear velocity command [m/s]
	float yaw_vel = 0.0f;		// Yaw velocity [rad/s]
	float yaw_vel_cmd = 0.0f;	// Yaw velocity command [rad/s]
	float v_cmd_L = 0.0f;		// L motor voltage cmd [V]
	float v_cmd_R = 0.0f;		// R motor voltage cmd [V]

	// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% //
	// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% //
	// %%%%%% YOUR MODEL PARAMETERS GO IN HERE %%%%%%% //
	// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% //
	// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% //
	

	// Init Flag
	bool init_complete = false;
}

/**
 * @brief Initializes controller subsystems
 */
void Controller::init()
{
	if (!init_complete)
	{
		// Init dependent subsystems
		Bluetooth::init();
		Imu::init();
		MotorL::init();
		MotorR::init();

		// Set init flag
		init_complete = true;
	}
}

/**
 * @brief Runs one control loop iteration
 */
void Controller::update()
{
	// Get teleop commands
	lin_vel_cmd = Bluetooth::get_lin_vel_cmd();
	yaw_vel_cmd = Bluetooth::get_yaw_vel_cmd();


	// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% //
	// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% //
	// %%%%%%%% YOUR CONTROLLER GOES IN HERE %%%%%%%%% //
	// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% //
	// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% //

}

/**
 * @brief Returns linear velocity estimate [m/s]
 */
float Controller::get_lin_vel()
{
	return lin_vel;
}

/**
 * @brief Returns left motor voltage command [V]
 */
float Controller::get_motor_L_cmd()
{
	return v_cmd_L;
}

/**
 * @brief Returns right motor voltage command [V]
 */
float Controller::get_motor_R_cmd()
{
	return v_cmd_R;
}