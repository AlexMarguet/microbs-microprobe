#include "data_save.h"

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

void DataSaver::closeCsv() {
    m_fout.close();
}