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

	void AdcSetup();

	void AdcIn();

	void DacOut();

private:
	uint m_board = 0;

};

#endif // #ifndef SENSORAY_826_H