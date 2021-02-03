/**
 * @file Controller.h
 * @brief Subsystem for self-balancing control system
 * @author Dan Oates (WPI Class of 2020)
 */
#pragma once

/**
 * Namespace Declaration
 */
namespace Controller
{
	// Fields
	extern const float f_ctrl;	// Control frequency [Hz]
	extern const float t_ctrl;	// Control period [s]

	// Methods
	void init();
	void update();
	float get_lin_vel();
	float get_motor_L_cmd();
	float get_motor_R_cmd();
}