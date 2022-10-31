#include "data_saver.h"

using namespace std;

DataSaver::DataSaver() {

}

void DataSaver::createCsv(string name) {
    m_name = name;
    // filesystem::current_path("C:\\Users\\Alexandre\\Documents\\Cours\\PDM\\microbs-microprobe\\microprobe_cmake\\out\\data");
    std::cout << filesystem::current_path() << endl;
    filesystem::current_path("C:\\Users\\MAGNETO\\Desktop\\Lucio\\AMarguet\\microbs-microprobe\\build-microprobe_app-Desktop_Qt_6_4_0_MinGW_64_bit-Debug\\Data");
    
    m_fout.open(m_name, fstream::out);
    m_fout << "time" << separator << "load" << endl;
}

void DataSaver::writeCsv(string content) {
    //m_fout.open(m_name, fstream::app);
    m_fout << content << endl;
}

void DataSaver::writeCsv(int size, float* load) {
    for (int i = 0; i < size; i++) {
        m_fout << i << separator << load[i] << endl;
    }
}

void DataSaver::closeCsv() {
    m_fout.close();
}