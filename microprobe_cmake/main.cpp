// microprobe_cmake.cpp : définit le point d'entrée de l'application.
//

#include "main.h"
#include "test_class.h"
#include "sensoray/826api.h"

using namespace std;

void AdcHandler(void);

int main()
{
	cout << "Microprobe program - Version = 0.1" << endl << endl;
	
	TestClass test;
	
	// Open boards
	int board_flags = S826_SystemOpen();
	if (board_flags < 0)
		printf("S826_SystemOpen returned error code %d", board_flags);
	else if (board_flags == 0)
		printf("No boards were detected");

	cout << "Opening system (bitflags): " << board_flags << endl;

	// Version read
	cout << endl << "VersionRead" << endl;
	uint board = 0, api = 0, driver = 0, bdrev = 0, fpgarev = 0;
	uint* api_p = &api;
	uint* driver_p = &driver;
	uint* bdrev_p = &bdrev;
	uint* fpgarev_p = &fpgarev;

	S826_VersionRead(board, api_p, driver_p, bdrev_p, fpgarev_p);
	cout << showbase;
	cout << "Board: " << board << endl << "API version: " << hex << api << endl << "Driver version: " << driver << endl << "Board rev: " << bdrev << endl << "FPGA rev: " << fpgarev << dec << endl;

	// Analog input config
	cout << endl << "AdcSlotConfigRead" << endl;
	uint slot = 0, chan = 0, tsettle = 0, range = 0;
	uint* chan_p = &chan, *tsettle_p = &tsettle, *range_p = &range;
	S826_AdcSlotConfigRead(board, slot, chan_p, tsettle_p, range_p);
	cout << "Channel: " << chan << endl << "TSettle: " << tsettle << endl << "Range: " << range << endl;
	

	// ADC Read
	int buf[16];
	uint tstamp[16];
	uint slotlist = 0;
	uint* slotlist_p = &slotlist;
	uint tmax = 100;
	
	#define TSETTLE -3 // -3 or 3 ? Not clear, page 31/107
	S826_AdcSlotConfigWrite(board, 0, 0, TSETTLE, S826_ADC_GAIN_1); // measuring gain 0 on slot 0
	S826_AdcSlotlistWrite(board, 1, S826_BITWRITE); // enable slot 0
	S826_AdcTrigModeWrite(board, 0); // trigger mode = continuous
	S826_AdcEnableWrite(board, 1); // start adc conversions

	int counter = 0, k = 1000;
	while (counter < k) {
		counter++;
		AdcHandler();
		Sleep(100);
	}


	// Close and return
	cout << "Closing system: " << S826_SystemClose() << endl;

	system("pause");
	return 0;
}

void AdcHandler(void) // From sensoray manual, 31/107
{
	int errcode;
	int slotval[16]; // buffer must be sized for 16 slots
	uint slotlist = 1; // only slot 0 is of interest in this example
	errcode = S826_AdcRead(0, slotval, NULL, &slotlist, 100); // wait for IRQ

	if (errcode != S826_ERR_OK) {
		printf("AdcHandler error code: %d", errcode);
	}
	else {
		int16_t converted_data = (int)(slotval[0] & 0xFFFF);
		printf("%d\n", converted_data);
	}
	//printf("Raw adc data = %d", slotval[0] & 0xFFFF);
	
}