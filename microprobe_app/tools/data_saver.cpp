#include "data_saver.h"

using namespace std;
namespace fs = std::filesystem;

DataSaver::DataSaver() {}

void DataSaver::createTsv(string name) {
    m_name = name + ".tsv";
    // fs::current_path("C:\\Users\\Alexandre\\Documents\\Cours\\PDM\\microbs-microprobe\\microprobe_cmake\\out\\data");
    fs::current_path("C:\\Users\\MAGNETO\\Desktop\\Lucio\\AMarguet\\microbs-microprobe\\build-microprobe_app-Desktop_Qt_6_4_0_MinGW_64_bit-Debug\\Data");
    cout << "File created at " << fs::current_path() << endl;
    
    m_fout.open(m_name, fstream::out);
    m_fout << "time" << separator << "x" << separator << "f_ref_u" << separator << "f_ref_d" << separator << "load_u" <<
            separator << "load_d" << separator << "v_probe" << separator << "v_tendon_u" << separator << "v_tendon_d" << endl;
}

void DataSaver::writeTsv(string content) {
    //m_fout.open(m_name, fstream::app);
    m_fout << content << endl;
}

void DataSaver::writeTsv(int size, float* load) {
    for (int i = 0; i < size; i++) {
        m_fout << i << separator << load[i] << endl;
    }
}

void DataSaver::closeTsv() {
    m_fout.close();
}