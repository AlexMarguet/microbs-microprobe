#include "826.h"

using namespace std;

const Sensoray826::VoltLevel Sensoray826::motor_direction[3][2] = {{low, high}, {high, low}, {high, low}};

const uint Sensoray826::min_pulse_ontime = 0; //! TBD

const uint Sensoray826::motor_pulse_dio[] = {0, 1, 2};		// board layout: {47, 45, 43}
const uint Sensoray826::motor_dir_dio[] = {10, 11, 12};		// board layout: {27, 25, 23}
const uint Sensoray826::motor_ctr_chan[] = {0, 1, 2};

// const uint Sensoray826::adc_gain = S826_ADC_GAIN_2;	// -5 <-> +5 [V]
const uint Sensoray826::adc_gain = S826_ADC_GAIN_1;	// -10 <-> +10 [V] -- sensor range
const int Sensoray826::adc_t_settle = -3;			// -3 or 3 ? Not clear, page 31/107
const uint Sensoray826::adc_in_chan[] = {0, 1};		// board layout: {4, 6} on J1 {black, green}
const float Sensoray826::sensor_range = 10.;

const float Sensoray826::tendon_f_max = -12.;	// [mN]
const float Sensoray826::tendon_f_min = -5.;	// [mN]

const float Sensoray826::deg_per_step = 1.8;
const float Sensoray826::rad_per_step = 0.01 * M_PI;
const uint Sensoray826::step_per_tour = 200;

const uint Sensoray826::ustep[] = {256, 16, 16};
const float Sensoray826::shaft_radius[] = {5., 2.5, 2.5};	// [mm]
const float Sensoray826::v_manual[] = {1., 1.5, 1.5};		// [mm/s]
const float Sensoray826::correction_coef[] = {0.66, 1., 1.1};
const float Sensoray826::pulse_ontime_manual[] = { (1 / (120 * ustep[0] * step_per_tour *((v_manual[0] / shaft_radius[0]) / M_PI)) * 10e7),
													(1 / (120 * ustep[1] * step_per_tour *((v_manual[1] / shaft_radius[1]) / M_PI)) * 10e7),
													(1 / (120 * ustep[2] * step_per_tour *((v_manual[2] / shaft_radius[2]) / M_PI)) * 10e7) };

Sensoray826::Sensoray826() {
	this->open();
	this->motorsSetup();
	this->adcSetup();
}

// Sensoray826::~Sensoray826() {
// 	this->DioSourceReset();
// 	this->Close();
// }

void Sensoray826::open() {
	// Open boards
	int board_flags = S826_SystemOpen();
	if (board_flags < 0)
		printf("S826_SystemOpen returned error code %d", board_flags);
	else if (board_flags == 0)
		printf("No boards were detected");

	cout << "Opening system (bitflags): " << board_flags << endl;

	// Version read
	cout << endl << "VersionRead" << endl;
	uint api = 0, driver = 0, bdrev = 0, fpgarev = 0;
	uint* api_p = &api;
	uint* driver_p = &driver;
	uint* bdrev_p = &bdrev;
	uint* fpgarev_p = &fpgarev;

	S826_VersionRead(m_board, api_p, driver_p, bdrev_p, fpgarev_p);
	cout << showbase;
	cout << "Board: " << m_board << endl << "API version: " << hex << api << endl << "Driver version: " << driver << endl << "Board rev: " << bdrev << endl << "FPGA rev: " << fpgarev << dec << endl;
}

void Sensoray826::close() {
	// Close and return
	cout << "Closing system: " << S826_SystemClose() << endl;
}

void Sensoray826::motorsSetup() {
	this->createPWM(motor_ctr_chan[probe], pulse_ontime_manual[probe], pulse_ontime_manual[probe]);
	this->createPWM(motor_ctr_chan[tendon_u], pulse_ontime_manual[tendon_u], pulse_ontime_manual[tendon_u]);
	this->createPWM(motor_ctr_chan[tendon_d], pulse_ontime_manual[tendon_d], pulse_ontime_manual[tendon_d]);

	this->startPWM(motor_ctr_chan[probe]);
	this->startPWM(motor_ctr_chan[tendon_u]);
	this->startPWM(motor_ctr_chan[tendon_d]);
}

void Sensoray826::motorOn(Motor motor) {
	uint ctr = motor_ctr_chan[motor];
	uint dio = motor_pulse_dio[motor];
	this->routeCounterOutput(ctr, dio);
}

void Sensoray826::motorOff(Motor motor) {
	uint ctr = motor_ctr_chan[motor];
	uint dio = motor_pulse_dio[motor];
	this->unrouteCounterOutput(ctr, dio);
}

void Sensoray826::setMotorDirection(Motor motor, uint direction) {
	uint dio = motor_dir_dio[motor];

	VoltLevel level = low;
	if (direction) {
		level = high;
	}

	this->dioOut(dio, level);
}

void Sensoray826::setMotorDirection(Motor motor, Direction direction) {
	uint dio = motor_dir_dio[motor];
	VoltLevel level = Sensoray826::motor_direction[motor][direction];

	this->dioOut(dio, level);
}

void Sensoray826::setMotorSpeed(Motor motor, float speed) {
	if (speed == 0.) {
		return;
	}
	if (speed <= 0) {
		speed = -speed;
	}

	float period =  1000000 * (shaft_radius[motor] * rad_per_step) / (speed * ustep[motor]); // [s] to [us]

	if (period >= UINT_MAX) {
		period = UINT_MAX; // limit reached at ~1e-7 [mm/s]
	}

	uint pulse_ontime, pulse_offtime; // us
	if (period >= min_pulse_ontime * 2) {
		pulse_ontime = (uint) period/2;
		pulse_offtime = (uint) period/2;
	}
	//! Also block for uint overflow at low speed

	uint ctr = motor_ctr_chan[motor];

	// cout << "speed =" << speed << " and period =" << period << endl;
	this->setPWM(ctr, pulse_ontime, pulse_offtime);
}

void Sensoray826::motorOn(Motor motor, float speed) {
	if (speed == 0.) {
		this->motorOff(motor);
		return;
	}
	Direction dir;

	if (motor == probe) {
		if (speed <= 0) {
			dir = backward;
			speed = -speed;
		} else {
			dir = forward;
		}
	} else {
		if (speed <= 0) {
			dir = reel;
			speed = -speed;
		} else {
			dir = release;
		}
	}

	float period =  1000000 * correction_coef[motor] * (shaft_radius[motor] * rad_per_step) / (speed * ustep[motor]); // [s] to [us]

	if (period >= UINT_MAX) {
		period = UINT_MAX; // limit reached at ~1e-7 [mm/s]
	}

	uint pulse_ontime, pulse_offtime; // us
	if (period >= min_pulse_ontime * 2) {
		pulse_ontime = (uint) period/2;
		pulse_offtime = (uint) period/2;
	}
	//! Also block for uint overflow at low speed

	uint ctr = motor_ctr_chan[motor];

	// cout << "speed =" << speed << " and period =" << period << endl;
	this->setMotorDirection(motor, dir);
	this->setPWM(ctr, pulse_ontime, pulse_offtime);
	this->motorOn(motor);
}

void Sensoray826::dioIn() {
	int errcode;
	uint data[2];
	errcode = S826_DioInputRead(m_board, data);
}

// Pins are active-low: CLR = 5[V] and SET = 0[V]
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

int Sensoray826::waitForDioFallingEdge(uint dio)
{
	uint rise[] = { 0, 0 }; // not interested in rising edge
	uint fall[] = { (dio < 24) << (dio % 24), (dio > 23) << (dio % 24) }; // interested in falling edge
	S826_DioCapEnablesWrite(m_board, rise, fall, S826_BITSET); // Enable falling edge detection.
	return S826_DioCapRead(m_board, fall, 0, S826_WAIT_INFINITE); // Block until falling edge.
}

// Configure a counter as a 20 ns pulse generator and start it running.
int Sensoray826::createHwTimer(uint chan, uint period) // period in microseconds, channel 0 to 5
{
	S826_CounterModeWrite(m_board, chan, // Configure counter mode:
		S826_CM_K_1MHZ | // clock source = 1 MHz internal
		S826_CM_PX_START | S826_CM_PX_ZERO | // preload @start and counts==0
		S826_CM_UD_REVERSE | // count down
		S826_CM_OM_NOTZERO); // ExtOut = (counts!=0)
	S826_CounterPreloadWrite(m_board, chan, 0, period); // Set period in microseconds.
	return S826_CounterStateWrite(m_board, chan, 1); // Start the timer running.
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

int Sensoray826::unrouteCounterOutput(uint ctr, uint dio) //!! NOT THREAD-SAFE, check 66/107
{
	uint data[2]; // dio routing mask
	uint full_chan = 0xFFFF;

	if ((dio >= S826_NUM_DIO) || (ctr >= S826_NUM_COUNT))
		return S826_ERR_VALUE; // bad channel number
	if ((dio & 7) != ctr)
		return S826_ERR_VALUE; // counter output can't be routed to dio

	// Route counter output to DIO pin:
	S826_SafeWrenWrite(m_board, S826_SAFEN_SWE); // Enable writes to DIO signal router.
	S826_DioOutputSourceRead(m_board, data); // Route counter output to DIO
	data[dio > 23] &= (full_chan - (1 << (dio % 24))); // without altering other routes.
	S826_DioOutputSourceWrite(m_board, data);
	return S826_SafeWrenWrite(m_board, S826_SAFEN_SWD); // Disable writes to DIO signal router.
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

int Sensoray826::startPWM(uint ctr)
{
	return S826_CounterStateWrite(m_board, ctr, 1); // Start the PWM generator.
}

void Sensoray826::setPWM(uint ctr, uint ontime, uint offtime)
{
	S826_CounterPreloadWrite(m_board, ctr, 0, ontime); // On time in us.
	S826_CounterPreloadWrite(m_board, ctr, 1, offtime); // Off time in us.
}

void Sensoray826::dioSourceReset() {
	uint data[2] = { 0, 0 };
	S826_SafeWrenWrite(m_board, S826_SAFEN_SWE); // Enable writes to DIO signal router.
	S826_DioOutputSourceWrite(m_board, data);
	S826_SafeWrenWrite(m_board, S826_SAFEN_SWD); // Disable writes to DIO signal router.
}

void Sensoray826::loadSensorCalibration(LoadSensor load_sensor) {
	// float expected_weight_1 = -14.9, expected_weight_2 = -25.0, expected_weight_3 = -36.9;

	// cout << "Remove weights from the sensor" << endl;
	// Sleep(10000);
	// m_load_sensor_offset = this->getLoadSensor(load_sensor);
	// cout << "Offset: " << m_load_sensor_offset << endl;

	// cout << "Put weight 1 on the sensor" << endl;
	// Sleep(10000);
	// float weight_1 = this->getLoadSensor(load_sensor);
	// cout << "Weight 1: " << weight_1 << endl;

	// cout << "Put weight 2 on the sensor" << endl;
	// Sleep(10000);
	// float weight_2 = this->getLoadSensor(load_sensor);
	// cout << "Weight 2: " << weight_2 << endl;

	// cout << "Put weight 3 on the sensor" << endl;
	// Sleep(10000);
	// float weight_3 = this->getLoadSensor(load_sensor);
	// cout << "Weight 3: " << weight_3 << endl;

	// float drift = (weight_1/expected_weight_1 + weight_2/expected_weight_2 + weight_3/expected_weight_3) / 3;
	// cout << "Drift: " << drift << endl;

	// if (drift < 1.) {
	// 	cout << "Drift too low, corrected to 1." << endl;
	// 	drift = 1.;
	// }
	// m_load_sensor_drift = drift;
	
	// cout << "Corrected weights: " << weight_1/drift << " " <<weight_2/drift << " " << weight_3/drift << endl;
}

void Sensoray826::loadSensorOffsetCalibration(LoadSensor load_sensor) {
	m_load_sensor_offset[load_sensor] -= this->getLoadSensor(load_sensor);
}


float Sensoray826::getLoadSensor(LoadSensor load_sensor) {
	float adc_val = (float)this->adcIn(load_sensor);
	int max_val;
	if (adc_val < 0) {
		max_val = 0x8000;
	} else {
		max_val = 0x7FFF;
	}
	
	float out = (adc_val * sensor_range) / max_val;

	// return -(out - m_load_sensor_offset) / m_load_sensor_drift;
	return -(out * m_load_sensor_sensibility[load_sensor]  - m_load_sensor_offset[load_sensor]);
}

void Sensoray826::adcSetup() {
	S826_AdcSlotConfigWrite(m_board, 0, adc_in_chan[0], adc_t_settle, adc_gain); // measuring on slot 0
	S826_AdcSlotConfigWrite(m_board, 1, adc_in_chan[1], adc_t_settle, adc_gain); // measuring on slot 1
	// S826_AdcSlotlistWrite(m_board, 1, S826_BITWRITE); // enable slot 0
	S826_AdcSlotlistWrite(m_board, 0xFFFF, S826_BITWRITE); // enable slot 0 and 1
	S826_AdcTrigModeWrite(m_board, 0); // trigger mode = continuous
	S826_AdcEnableWrite(m_board, 1); // start adc conversions
}

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

void Sensoray826::dacOut() {

}
