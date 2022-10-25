#include "data_saver.h"

using namespace std;

DataSaver::DataSaver() {

}

void DataSaver::createCsv(string name) {
    m_name = name;
    filesystem::current_path("C:\\Users\\Alexandre\\Documents\\Cours\\PDM\\microbs-microprobe\\microprobe_cmake\\out\\data");
    m_fout.open(m_name, fstream::out);
}

void DataSaver::writeCsv(string content) {
    //m_fout.open(m_name, fstream::app);
    m_fout << content << endl;
}

void DataSaver::writeCsv(int size, float* load) {
    m_fout << "time;load" << endl;
    
    for (int i = 0; i < size; i++) {
        m_fout << i << ';' << load[i] << endl;
    }
}

void DataSaver::closeCsv() {
    m_fout.close();
}