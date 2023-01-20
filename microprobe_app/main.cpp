#include "main.h"

using namespace std;

int main(int argc, char *argv[])
{
    cout << "Microprobe program - Version = 0.1" << endl << endl;
	
	Sensoray826 board;
    DataSaver data_saver;
    Controller controller(board, data_saver);
	// system("pause");

    QApplication a(argc, argv);
    MainWindow w(board, controller, data_saver);
    w.show();
    return a.exec();
}
