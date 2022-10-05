// microprobe_cmake.cpp : définit le point d'entrée de l'application.
//

#include "main.h"

using namespace std;

int main()
{
	cout << "Microprobe program - Version = 0.1" << endl << endl;
	
	Sensoray826 board;

	board.Open();
	board.AdcSetup();
	board.DioOut();

	int counter = 0, k = 0;
	while (counter < k) {
		counter++;
		board.AdcIn();
		Sleep(100);
	}

	system("pause");
	return 0;
}