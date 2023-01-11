#include "main.h"

using namespace std;

int main(int argc, char *argv[])
{
    cout << "Microprobe program - Version = 0.1" << endl << endl;
	
	Sensoray826 board;
    Controller controller(board);
	// system("pause");

    QApplication a(argc, argv);
    MainWindow w(board, controller);
    w.show();
    return a.exec();
}
