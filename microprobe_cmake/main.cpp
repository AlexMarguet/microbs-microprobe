// microprobe_cmake.cpp : définit le point d'entrée de l'application.
//

#include "main.h"

using namespace std;

int main()
{
	cout << "Microprobe program - Version = 0.1" << endl << endl;
	
	// Sensoray826 board;

	// board.Open();
	// board.AdcSetup();
	// //board.DioOut();
	// uint ontime = 1000, offtime = 1000;
	// //board.CreatePWM(0, ontime, offtime);
	// board.RouteCounterOutput(0, 16);
	// //board.StartPWM(0);
	// //board.DioSourceReset();

	// //int counter = 0, k = 100;
	// //while (counter < k) {
	// //	counter++;
	// //	board.AdcIn();
	// //	Sleep(100);
	// //}

	fstream file;

	file.open("test.txt", fstream::out);

	//if (!file) cout << "error" << endl;

	file << flush;
	file.close();

	printf("%d\n", (int)file.is_open());
	system("pause");
	return 0;
}