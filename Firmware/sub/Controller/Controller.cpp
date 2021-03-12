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
	

	// Controller Constants
	const float dr_div_2 = dr/2.0f;	// Half wheel radius [m]
	const float pitch_max = 0.8f;	// Max pitch angle [rad]
	const float px = 20.0f;			// Pitch-velocity pole [1/s]
	const float pz = 80.0f;			// Yaw-velocity pole [1/s]


	// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% //
	// %%%%% INSERT YOUR CONTROLLER GAINS BELOW %%%%%% //
	// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% //
	const float k1 = 0.0f;
	const float k2 = 0.0f;
	const float k3 = 0.0f;
	// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% //

	// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% //
	// %%%%% INSERT YOUR PID CONTROLLER GAINS BELOW %%%%%% //
	// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% //
	const float Kp = 0.0f;
	const float Ki = 0.0f;
	const float Kd = 0.0f;
	// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% //

	// State Variables
	float lin_vel = 0.0f;		// Linear velocity [m/s]
	float lin_vel_cmd = 0.0f;	// Linear velocity command [m/s]
	float yaw_vel = 0.0f;		// Yaw velocity [rad/s]
	float yaw_vel_cmd = 0.0f;	// Yaw velocity command [rad/s]
	float v_cmd_L = 0.0f;		// L motor voltage cmd [V]
	float v_cmd_R = 0.0f;		// R motor voltage cmd [V]

	// Controllers
	PID yaw_pid(Kp, Ki, Kd, -Vb, Vb, f_ctrl);
	ClampLimiter volt_limiter(Vb);

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

	// Estimate linear velocity
	lin_vel = dr_div_2 * (MotorL::get_velocity() + MotorR::get_velocity());

	// Pitch-Velocity State-Space Control
	const float v_avg_ref = Gv * lin_vel_cmd;
	float v_avg = v_avg_ref + k1 * (0.0f - Imu::get_pitch_vel()) +
				  k2 * (0.0f - Imu::get_pitch()) +
				  k3 * (lin_vel_cmd - lin_vel);

	// Clamp the voltage within the limits
	v_avg = clamp(v_avg, -Vb, Vb);

	// Yaw velocity control
	const float yaw_ff = Gw * yaw_vel_cmd;
	const float yaw_error = yaw_vel_cmd - Imu::get_yaw_vel();
	const float v_diff = yaw_pid.update(yaw_error, yaw_ff);

	// Motor voltage commands
	v_cmd_L = volt_limiter.update(v_avg - v_diff);
	v_cmd_R = volt_limiter.update(v_avg + v_diff);

	// Disable motors if tipped over
	if(fabsf(Imu::get_pitch()) > pitch_max)
	{
		v_cmd_L = 0.0f;
		v_cmd_R = 0.0f;
	}

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