#include "826.h"

using namespace std;

Sensoray826::Sensoray826() {

}

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

void Sensoray826::DioIn() {
	int errcode;
	uint data[2];
	errcode = S826_DioInputRead(m_board, data);
}

// Pins are active-low: CLR = 5[V] and SET = 0[V]
void Sensoray826::DioOut() {
	int errcode;
	uint bitmask[2] = {0, 0};
	int chan = 23;

	//uint mode = S826_BITCLR;
	uint mode = S826_BITSET;
	//uint mode = S826_BITWRITE;

	if (0 <= chan && chan < 24) {
		bitmask[0] = bitmask[0] | 1 << chan;
	}
	else if (chan >= 24) {
		bitmask[1] = bitmask[1] | 1 << (chan-24);
	}

	cout << hex << showbase << bitmask[0] << dec << endl;
	

	errcode = S826_DioOutputWrite(m_board, bitmask, mode);
	printf("DioOut error code: %d\n", errcode);
}

int Sensoray826::WaitForDioFallingEdge(uint dio)
{
	uint rise[] = { 0, 0 }; // not interested in rising edge
	uint fall[] = { (dio < 24) << (dio % 24), (dio > 23) << (dio % 24) }; // interested in falling edge
	S826_DioCapEnablesWrite(m_board, rise, fall, S826_BITSET); // Enable falling edge detection.
	return S826_DioCapRead(m_board, fall, 0, S826_WAIT_INFINITE); // Block until falling edge.
}

void Sensoray826::AdcSetup() {
	#define TSETTLE -3 // -3 or 3 ? Not clear, page 31/107
	S826_AdcSlotConfigWrite(m_board, 0, 0, TSETTLE, S826_ADC_GAIN_1); // measuring gain 0 on slot 0
	S826_AdcSlotlistWrite(m_board, 1, S826_BITWRITE); // enable slot 0
	S826_AdcTrigModeWrite(m_board, 0); // trigger mode = continuous
	S826_AdcEnableWrite(m_board, 1); // start adc conversions
}

void Sensoray826::AdcIn() {
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
}

void Sensoray826::DacOut() {

}
