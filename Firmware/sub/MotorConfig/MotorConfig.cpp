/**
 * @file MotorConfig.cpp
 * @author Dan Oates (WPI Class of 2020)
 */
#include <MotorConfig.h>

/**
 * Namespace Definitions
 */
namespace MotorConfig
{
	// Universal Constants
	const float Vb = 12.0f;		// Battery voltage [V]
	const float i_ST = 1.0f;	// Stall current [A]
	const float i_NL = 0.12f;	// No-load current [A]
	const float R = 5.4f;		// Resistance [Ohm]

	// Robot-Specific Constants
	// direction = Motor direction [+1, -1]
	// tr = Torque ratio [(N*m)/(N*m)]
	#if ES3011_BOT_ID <= 1
		const float direction = +1.0f;
		const float tr = +30.0f;
	#else
		const float direction = -1.0f;
		const float tr = +56.0f;
	#endif

	// Derived Constants
	const float w_NL = 1047.0f / tr;			// No-load speed [rad/s]
	const float t_ST = 0.015f * tr;				// Stall torque [N*m]
	const float Kv = (Vb - R * i_NL) / w_NL;	// Voltage constant [V/(rad/s)]
	const float Kt = t_ST * R / Vb;				// Torque constant [N*m/A]
	const float enc_cpr = 44.0 * tr;			// Encoder resolution [cnt/rev]
}