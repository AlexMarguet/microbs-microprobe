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
#include <limits>

#include "826api.h"

#define MIN_ALLOWED_PWM_T	100

class Sensoray826 {
public:
	// Constants
	enum Motor {probe, tendon_u, tendon_d};
	enum VoltLevel {low = S826_BITSET, high = S826_BITCLR};
	enum LoadSensor {load_sensor_u, load_sensor_d};
	enum Direction {release, reel, backward = 0, forward};

	static const VoltLevel motor_direction[3][2];

	static const uint min_pulse_ontime;

	static const uint motor_pulse_dio[];
	static const uint motor_dir_dio[];
	static const uint motor_ctr_chan[];

	static const uint adc_gain;
	static const int adc_t_settle;
	static const uint adc_in_chan[];
	static const float sensor_range;

	static const float tendon_f_max;
	static const float tendon_f_min;

	static const float deg_per_step;
	static const float rad_per_step;
	static const uint step_per_tour;

	static const uint ustep[];
	static const float shaft_radius[];
	static const float v_manual[];
	static const float correction_coef[];
	static const float pulse_ontime_manual[];

	Sensoray826();

	// ~Sensoray826(); //Didnt work because I was making a copy and not an &

	void open();

	void close();

	void motorsSetup();

	void motorOn(Motor motor);
	void motorOn(Motor motor, float speed);

	void motorOff(Motor motor);

	void setMotorDirection(Motor motor, uint direction);
	void setMotorDirection(Motor motor, Direction direction);

	void setMotorSpeed(Motor motor, float speed);

	void dioIn();

	void dioOut(uint chan, VoltLevel level);

	int waitForDioFallingEdge(uint dio);

	int createHwTimer(uint chan, uint period);

	int routeCounterOutput(uint ctr, uint dio);

	int unrouteCounterOutput(uint ctr, uint dio);

	void createPWM(uint ctr, uint ontime, uint offtime);
	
	int startPWM(uint ctr);
	
	void setPWM(uint ctr, uint ontime, uint offtime);

	void dioSourceReset();

	void loadSensorCalibration(LoadSensor load_sensor);

	void loadSensorOffsetCalibration(LoadSensor load_sensor);

	float getLoadSensor(LoadSensor load_sensor);

	void adcSetup();

	int16_t adcIn(int slot);

	void dacOut();

private:
	uint m_board = 0;

	float m_load_sensor_offset[2] = {-107, -142}; // [mN]
	float m_load_sensor_sensibility[2] = {100, 116.3}; // [mN/V]
};

#endif // #ifndef SENSORAY_826_H