int16_t Sensoray826::adcIn(int slot) {
	int errcode;
	int slotval[16]; // buffer must be sized for 16 slots
	// uint slotlist = 1; // only slot 0 is of interest in this example
	// uint slotlist = 0x0003; // slot 0 and 1
	uint slotlist = 0xFFFF; // all slots
	errcode = S826_AdcRead(m_board, slotval, NULL, &slotlist, 100); // wait for IRQ

	// if (errcode != S826_ERR_OK) {
	// 	printf("AdcIn error code: %d\n", errcode);
	// }
	// else {
	// 	int16_t converted_data = (int)(slotval[0] & 0xFFFF);
	// 	printf("%d\n", converted_data);
	// }

	// cout << "slot 0: " << (int16_t)(slotval[slot] & 0xFFFF) << "slot 1: " << (int16_t)(slotval[1] & 0xFFFF) << endl;
	return (int16_t)(slotval[slot] & 0xFFFF);
}

void Sensoray826::adcSetup() {
	S826_AdcSlotConfigWrite(m_board, 0, adc_in_chan[0], adc_t_settle, adc_gain); // measuring on slot 0
	S826_AdcSlotConfigWrite(m_board, 1, adc_in_chan[1], adc_t_settle, adc_gain); // measuring on slot 1
	// S826_AdcSlotlistWrite(m_board, 1, S826_BITWRITE); // enable slot 0
	S826_AdcSlotlistWrite(m_board, 0xFFFF, S826_BITWRITE); // enable slot 0 and 1
	S826_AdcTrigModeWrite(m_board, 0); // trigger mode = continuous
	S826_AdcEnableWrite(m_board, 1); // start adc conversions
}

void Sensoray826::createPWM(uint ctr, uint ontime, uint offtime)
{
	S826_CounterModeWrite(m_board, ctr, // Configure counter for PWM:
		S826_CM_K_1MHZ | // clock = internal 1 MHz
		S826_CM_UD_REVERSE | // count down
		S826_CM_PX_START | S826_CM_PX_ZERO | // preload @startup and counts==0
		S826_CM_BP_BOTH | // use both preloads (toggle)
		S826_CM_OM_PRELOAD); // assert ExtOut during preload0 interval
	setPWM(ctr, ontime, offtime); // Program initial on/off times.
}

int Sensoray826::routeCounterOutput(uint ctr, uint dio) //!! NOT THREAD-SAFE, check 66/107
{
	uint data[2]; // dio routing mask
	if ((dio >= S826_NUM_DIO) || (ctr >= S826_NUM_COUNT))
		return S826_ERR_VALUE; // bad channel number
	if ((dio & 7) != ctr)
		return S826_ERR_VALUE; // counter output can't be routed to dio
	// Route counter output to DIO pin:
	S826_SafeWrenWrite(m_board, S826_SAFEN_SWE); // Enable writes to DIO signal router.
	S826_DioOutputSourceRead(m_board, data); // Route counter output to DIO
	data[dio > 23] |= (1 << (dio % 24)); // without altering other routes.
	S826_DioOutputSourceWrite(m_board, data);
	return S826_SafeWrenWrite(m_board, S826_SAFEN_SWD); // Disable writes to DIO signal router.
}

void Sensoray826::dioOut(uint dio, VoltLevel level) {
	int errcode;
	uint data[2] = {0, 0};

	//uint mode = S826_BITCLR;
	//uint mode = S826_BITSET;
	//uint mode = S826_BITWRITE;

	if (dio >= S826_NUM_DIO) {
		return; //S826_ERR_VALUE; // bad channel number
	}
		
	data[dio > 23] |= (1 << (dio % 24));

	errcode = S826_DioOutputWrite(m_board, data, level);
	// printf("dioOut error code: %d\n", errcode);
}

//-----------------------------------------------------------------

float Sensoray826::getLoadSensor(LoadSensor load_sensor) {
	float adc_val = (float)this->adcIn(load_sensor);
	int max_val;
	if (adc_val < 0) {
		max_val = 0x8000;
	} else {
		max_val = 0x7FFF;
	}
	
	float out = (adc_val * sensor_range) / max_val;

	return -(out * m_load_sensor_sensibility[load_sensor]  - m_load_sensor_offset[load_sensor]);
}

void Sensoray826::setPWM(uint ctr, uint ontime, uint offtime)
{
	S826_CounterPreloadWrite(m_board, ctr, 0, ontime); // On time in us.
	S826_CounterPreloadWrite(m_board, ctr, 1, offtime); // Off time in us.
}

enum Motor {probe, tendon_u, tendon_d};
enum VoltLevel {low = S826_BITSET, high = S826_BITCLR};
enum LoadSensor {load_sensor_u, load_sensor_d};
enum Direction {release, reel, backward = 0, forward};

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