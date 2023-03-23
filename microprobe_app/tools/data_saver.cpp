#include "data_saver.h"

using namespace std;
namespace fs = std::filesystem;


const char DataSaver::separator = ',';
	
const int DataSaver::header_size = 9;

const string DataSaver::header[header_size] = {"time", "x_probe", "f_ref_u", "f_ref_d", "load_u", "load_d", "v_probe", "v_tendon_u", "v_tendon_d"};


DataSaver::DataSaver() {}

void DataSaver::createCsv(string name) {
    m_file_name = name + ".csv";
    fs::current_path("C:\\Users\\MAGNETO\\Desktop\\Lucio\\AMarguet\\microbs-microprobe\\build-microprobe_app-Desktop_Qt_6_4_0_MinGW_64_bit-Debug\\Data");
    
    cout << "File created at " << fs::current_path() << endl;
}

void DataSaver::writeHeader() {
    fstream file_stream;
    file_stream.open(m_file_name, fstream::out);

    for (int i = 0; i < header_size; i++) {
        file_stream << header[i] << separator;
    }
    file_stream << endl;
}

void DataSaver::writeHeader(string add) {
    fstream file_stream;
    file_stream.open(m_file_name, fstream::out);

    for (int i = 0; i < header_size; i++) {
        file_stream << header[i] << separator;
    }

    file_stream << add;
    file_stream << endl;
}

void DataSaver::writeCsv(string content) {
    fstream file_stream;
    file_stream.open(m_file_name, fstream::app);
    file_stream << content << endl;
}

void DataSaver::writeDataLine(float* data, int size) {
    fstream file_stream;
    file_stream.open(m_file_name, fstream::app);

    for (int i = 0; i < size; i++) {
        file_stream << data[i] << separator;
    }
    file_stream << endl;
}