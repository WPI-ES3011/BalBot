/**
 * @file Bluetooth.h
 * @brief Subsystem for Bluetooth communication
 * @author Dan Oates (WPI Class of 2020)
 */
#pragma once

/**
 * Namespace Declaration
 */
namespace Bluetooth
{
	void init();
	void update();
	float get_lin_vel_cmd();
	float get_yaw_vel_cmd();
}