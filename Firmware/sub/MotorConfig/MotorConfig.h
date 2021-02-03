/**
 * @file MotorConfig.h
 * @brief Namespace for motor constants
 * @author Dan Oates (WPI Class of 2020)
 */
#pragma once

/**
 * Namespace Declaration
 */
namespace MotorConfig
{
	const extern float Vb;			// Battery voltage [V]
	const extern float R;			// Resistance [Ohm]
	const extern float Kv;			// Voltage constant [V/(rad/s)]
	const extern float Kt;			// Torque constant [N*m/A]
	const extern float direction;	// Motor direction [+1, -1]
	const extern float enc_cpr;		// Encoder resolution [cnt/rev]
}