#ifndef SENSORAY_826_H
#define SENSORAY_826_H

#include <iostream>
#include <iomanip>
#include <cstdio>

#include "826api.h"

class Sensoray826 {
public:
	Sensoray826();

	//~Sensoray826();

	void Open();

	void Close();

	void DioIn();

	void DioOut();

	int WaitForDioFallingEdge(uint dio);

	int CreateHwTimer(uint chan, uint period);

	int RouteCounterOutput(uint ctr, uint dio);

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