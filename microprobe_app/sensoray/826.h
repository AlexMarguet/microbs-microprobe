#ifndef SENSORAY_826_H
#define SENSORAY_826_H

#include <iostream>
#include <iomanip>
#include <cstdio>
#include <cmath>

#include "826api.h"

#define MIN_ALLOWED_PWM_T	100

class Sensoray826 {
public:
	// Constants
	enum Motor {probe, tendon_r, tendon_l};
	enum VoltLevel {low = S826_BITSET, high = S826_BITCLR};

	static const uint motor_pulse_dio[] = {1, 11};
	static const uint motor_dir_dio[] = {0, 12};
	static const uint motor_ctr_chan[] = {1, 3};

	static const uint adc_range = 1;
	static const uint adc_in_chan = 0;

	static const uint tendon_f_max = 225;	// [mN], 90%*250
	static const uint tendon_f_min = 20;	// [mN]

	static const float deg_per_step = 1.8;
	static const uint step_per_tour = 200;

	static const uint probe_ustep = 256;
	static const float probe_radius = 5.;	// [mm]
	static const uint probe_v_manual = 3;	// [mm/s]
	static const float probe_pulse_ontime = (1 / (120 * probe_ustep * step_per_tour *((probe_v_manual / probe_radius) / M_PI)) * 10e7;

	static const uint tendon_ustep = 16;
	static const float tendon_radius = 2.5;	// [mm]
	static const uint tendon_v_manual = 5;	// [mm/s]
	static const float tendon_pulse_ontime = (1 / (120 * tendon_ustep * step_per_tour *((tendon_v_manual / tendon_radius) / M_PI)) * 10e7;

	Sensoray826();

	// ~Sensoray826(); //Didnt work because I was making a copy and not an &

	void Open();

	void Close();

	void MotorsSetup();

	void MotorOn(Motor motor);

	void MotorOff(Motor motor);

	void SetMotorDirection(Motor motor, uint direction);

	void SetMotorSpeed(Motor motor, uint speed);

	void DioIn();

	void DioOut(uint chan, VoltLevel level);

	int WaitForDioFallingEdge(uint dio);

	int CreateHwTimer(uint chan, uint period);

	int RouteCounterOutput(uint ctr, uint dio);

	int UnrouteCounterOutput(uint ctr, uint dio);

	void CreatePWM(uint ctr, uint ontime, uint offtime);
	
	int StartPWM(uint ctr);
	
	void SetPWM(uint ctr, uint ontime, uint offtime);

	void DioSourceReset();

	void AdcSetup();

	void AdcIn();

	void DacOut();

private:
	uint m_board = 0;
};

#endif // #ifndef SENSORAY_826_H