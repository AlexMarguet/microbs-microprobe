#include "826.h"

using namespace std;

const uint Sensoray826::max_pwm_freq = 100;

const uint Sensoray826::motor_pulse_dio[] = {1, 11};
const uint Sensoray826::motor_dir_dio[] = {0, 12};
const uint Sensoray826::motor_ctr_chan[] = {1, 3};

const uint Sensoray826::adc_gain = S826_ADC_GAIN_2;	// -5 <-> +5 [V]
const int Sensoray826::adc_t_settle = -3;	// -3 or 3 ? Not clear, page 31/107
const uint Sensoray826::adc_in_chan = 0;
const float Sensoray826::sensor_range = 250.;

const float Sensoray826::tendon_f_max = 225.;	// [mN], 90%*250
const float Sensoray826::tendon_f_min = 20.;	// [mN]

const float Sensoray826::deg_per_step = 1.8;
const uint Sensoray826::step_per_tour = 200;

const uint Sensoray826::probe_ustep = 256;
const float Sensoray826::probe_radius = 5.;	// [mm]
const uint Sensoray826::probe_v_manual = 3;	// [mm/s]
const float Sensoray826::probe_pulse_ontime = (1 / (120 * probe_ustep * step_per_tour *((probe_v_manual / probe_radius) / M_PI)) * 10e7);

const uint Sensoray826::tendon_ustep = 16;
const float Sensoray826::tendon_radius = 2.5;	// [mm]
const uint Sensoray826::tendon_v_manual = 5;	// [mm/s]
const float Sensoray826::tendon_pulse_ontime = (1 / (120 * tendon_ustep * step_per_tour *((tendon_v_manual / tendon_radius) / M_PI)) * 10e7);

Sensoray826::Sensoray826() {
	this->Open();
	this->MotorsSetup();
}

// Sensoray826::~Sensoray826() {
// 	this->DioSourceReset();
// 	this->Close();
// }

void Sensoray826::Open() {
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

void Sensoray826::Close() {
	// Close and return
	cout << "Closing system: " << S826_SystemClose() << endl;
}

void Sensoray826::MotorsSetup() {
	this->CreatePWM(motor_ctr_chan[probe], probe_pulse_ontime, probe_pulse_ontime);
	this->CreatePWM(motor_ctr_chan[tendon_r], tendon_pulse_ontime, tendon_pulse_ontime);
	// this->CreatePWM(motor_ctr_chan[tendon_l], tendon_pulse_ontime, tendon_pulse_ontime);

	this->StartPWM(motor_ctr_chan[probe]);
	this->StartPWM(motor_ctr_chan[tendon_r]);
	// this->StartPWM(motor_ctr_chan[tendon_l]);
}

void Sensoray826::MotorOn(Motor motor) {
	uint ctr = motor_ctr_chan[motor];
	uint dio = motor_pulse_dio[motor];
	this->RouteCounterOutput(ctr, dio);
}

void Sensoray826::MotorOff(Motor motor) {
	uint ctr = motor_ctr_chan[motor];
	uint dio = motor_pulse_dio[motor];
	this->UnrouteCounterOutput(ctr, dio);
}

void Sensoray826::SetMotorDirection(Motor motor, uint direction) {
	uint dio = motor_pulse_dio[motor];

	VoltLevel level = low;
	if (direction) {
		level = high;
	}

	this->DioOut(dio, level);
}

void Sensoray826::SetMotorSpeed(Motor motor, uint speed) {
	float period = 1/ (speed * step_per_tour);
	uint pulse_ontime, pulse_offtime;
	if (period >= MIN_ALLOWED_PWM_T * 2) {
		pulse_ontime = (uint) period/2;
		pulse_offtime = (uint) period/2;
	}

	uint ctr = motor_ctr_chan[motor];
	uint dio = motor_pulse_dio[motor];
	this->SetPWM(ctr, pulse_ontime, pulse_offtime);
}

void Sensoray826::DioIn() {
	int errcode;
	uint data[2];
	errcode = S826_DioInputRead(m_board, data);
}

// Pins are active-low: CLR = 5[V] and SET = 0[V]
void Sensoray826::DioOut(uint dio, VoltLevel level) {
	int errcode;
	uint data[2] = {0, 0};

	//uint mode = S826_BITCLR;
	//uint mode = S826_BITSET;
	//uint mode = S826_BITWRITE;

	if (dio >= S826_NUM_DIO) {
		return; //S826_ERR_VALUE; // bad channel number
	}
		
	data[dio > 23] |= (1 << (dio % 24));

	cout << hex << showbase << data[0] << dec << endl;
	

	errcode = S826_DioOutputWrite(m_board, data, level);
	// printf("DioOut error code: %d\n", errcode);
}

int Sensoray826::WaitForDioFallingEdge(uint dio)
{
	uint rise[] = { 0, 0 }; // not interested in rising edge
	uint fall[] = { (dio < 24) << (dio % 24), (dio > 23) << (dio % 24) }; // interested in falling edge
	S826_DioCapEnablesWrite(m_board, rise, fall, S826_BITSET); // Enable falling edge detection.
	return S826_DioCapRead(m_board, fall, 0, S826_WAIT_INFINITE); // Block until falling edge.
}

// Configure a counter as a 20 ns pulse generator and start it running.
int Sensoray826::CreateHwTimer(uint chan, uint period) // period in microseconds, channel 0 to 5
{
	S826_CounterModeWrite(m_board, chan, // Configure counter mode:
		S826_CM_K_1MHZ | // clock source = 1 MHz internal
		S826_CM_PX_START | S826_CM_PX_ZERO | // preload @start and counts==0
		S826_CM_UD_REVERSE | // count down
		S826_CM_OM_NOTZERO); // ExtOut = (counts!=0)
	S826_CounterPreloadWrite(m_board, chan, 0, period); // Set period in microseconds.
	return S826_CounterStateWrite(m_board, chan, 1); // Start the timer running.
}

int Sensoray826::RouteCounterOutput(uint ctr, uint dio) //!! NOT THREAD-SAFE, check 66/107
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

int Sensoray826::UnrouteCounterOutput(uint ctr, uint dio) //!! NOT THREAD-SAFE, check 66/107
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

void Sensoray826::CreatePWM(uint ctr, uint ontime, uint offtime)
{
	S826_CounterModeWrite(m_board, ctr, // Configure counter for PWM:
		S826_CM_K_1MHZ | // clock = internal 1 MHz
		S826_CM_UD_REVERSE | // count down
		S826_CM_PX_START | S826_CM_PX_ZERO | // preload @startup and counts==0
		S826_CM_BP_BOTH | // use both preloads (toggle)
		S826_CM_OM_PRELOAD); // assert ExtOut during preload0 interval
	SetPWM(ctr, ontime, offtime); // Program initial on/off times.
}

int Sensoray826::StartPWM(uint ctr)
{
	return S826_CounterStateWrite(m_board, ctr, 1); // Start the PWM generator.
}

void Sensoray826::SetPWM(uint ctr, uint ontime, uint offtime)
{
	S826_CounterPreloadWrite(m_board, ctr, 0, ontime); // On time in us.
	S826_CounterPreloadWrite(m_board, ctr, 1, offtime); // Off time in us.
}

void Sensoray826::DioSourceReset() {
	uint data[2] = { 0, 0 };
	S826_SafeWrenWrite(m_board, S826_SAFEN_SWE); // Enable writes to DIO signal router.
	S826_DioOutputSourceWrite(m_board, data);
	S826_SafeWrenWrite(m_board, S826_SAFEN_SWD); // Disable writes to DIO signal router.
}

float Sensoray826::getLoadSensor(LoadSensor load_sensor) {
	float adc_val = (float)this->AdcIn();
	int max_val;
	if (adc_val < 0) {
		max_val = 0x8000;
	} else {
		max_val = 0x7FFF;
	}
	
	return (adc_val * sensor_range) / max_val;
}

void Sensoray826::AdcSetup() {
	S826_AdcSlotConfigWrite(m_board, 0, 0, adc_t_settle, adc_gain); // measuring on slot 0
	S826_AdcSlotlistWrite(m_board, 1, S826_BITWRITE); // enable slot 0
	S826_AdcTrigModeWrite(m_board, 0); // trigger mode = continuous
	S826_AdcEnableWrite(m_board, 1); // start adc conversions
}

int Sensoray826::AdcIn() {
	int errcode;
	int slotval[16]; // buffer must be sized for 16 slots
	uint slotlist = 1; // only slot 0 is of interest in this example
	errcode = S826_AdcRead(m_board, slotval, NULL, &slotlist, 100); // wait for IRQ

	if (errcode != S826_ERR_OK) {
		printf("AdcIn error code: %d\n", errcode);
	}
	else {
		int16_t converted_data = (int)(slotval[0] & 0xFFFF);
		printf("%d\n", converted_data);
	}
	//printf("Raw adc data = %d", slotval[0] & 0xFFFF);
	return (int)(slotval[0] & 0xFFFF);
}

void Sensoray826::DacOut() {

}
