// microprobe_cmake.cpp : définit le point d'entrée de l'application.
//

#include "microprobe_cmake.h"
#include "test_class.h"
#include "826api.h"

using namespace std;

int main()
{
	cout << "Version 0.1" << endl;
	TestClass test;
	
	cout << S826_CM_XS_10HZ << endl;
	S826_SystemClose();

	string input;
	cin >> input;
	return 0;
}
