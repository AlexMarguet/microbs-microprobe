#ifndef SENSORAY_826_H
#define SENSORAY_826_H

#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif

#include <iostream>
#include <iomanip>
#include <cstdio>
#include <cmath>

#include "826api.h"

#define MIN_ALLOWED_PWM_T	100

class Sensoray826 {
public:
	// Constants
	enum Motor {probe, tendon_r};
	enum VoltLevel {low = S826_BITSET, high = S826_BITCLR};
	enum LoadSensor {load_sensor_r};

	static const uint max_pwm_freq;

	static const uint motor_pulse_dio[];
	static const uint motor_dir_dio[];
	static const uint motor_ctr_chan[];

	static const uint adc_gain;
	static const int adc_t_settle;
	static const uint adc_in_chan;
	static const float sensor_range;

	static const float tendon_f_max;
	static const float tendon_f_min;

	static const float deg_per_step;
	static const uint step_per_tour;

	static const uint probe_ustep;
	static const float probe_radius;
	static const uint probe_v_manual;
	static const float probe_pulse_ontime;

	static const uint tendon_ustep;
	static const float tendon_radius;
	static const uint tendon_v_manual;
	static const float tendon_pulse_ontime;

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

	void loadSensorCalibration(LoadSensor load_sensor);

	float getLoadSensor(LoadSensor load_sensor);

	void AdcSetup();

	int16_t AdcIn();

	void DacOut();

private:
	uint m_board = 0;

	float m_load_sensor_offset = -27.7023;
	float m_load_sensor_drift = 2.40586;
};

#endif // #ifndef SENSORAY_826_H