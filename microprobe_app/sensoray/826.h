#ifndef SENSORAY_826_H
#define SENSORAY_826_H

#include <iostream>
#include <iomanip>
#include <cstdio>

#include "826api.h"

#define MIN_ALLOWED_PWM_T	100

enum Motor {probe, tendon_r, tendon_l};
enum VoltLevel {low = S826_BITSET, high = S826_BITCLR};

class Sensoray826 {
public:
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
	uint m_step_per_tour = 200;

	uint m_board = 0;

	uint m_probe_motor_chan = 1;
	uint m_tendon_r_motor_chan = 11;
	uint m_tendon_l_motor_chan;

	uint m_probe_dir_chan = 0;
	uint m_tendon_r_dir_chan = 12;
	uint m_tendon_l_dir_chan;

	uint m_probe_motor_ctr = 1;
	uint m_tendon_r_motor_ctr = 3;
	uint m_tendon_l_motor_ctr;

};

#endif // #ifndef SENSORAY_826_H