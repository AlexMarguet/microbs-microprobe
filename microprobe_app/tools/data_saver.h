#ifndef TOOLS_DATA_SAVER_H
#define TOOLS_DATA_SAVER_H

#include <iostream>
#include <fstream>
#include <filesystem>

using namespace std;

class DataSaver {
public:
	static const char separator;
	
	static const int header_size;

	static const string header[];

	DataSaver();

	void createCsv(string name);

	void writeHeader();
	void writeHeader(string add);

	void writeCsv(string content);

	void writeDataLine(float* data, int size);

	void closeCsv();

private:
	// fstream m_file_out;
	string m_file_name;
};

#endif // TOOLS_DATA_SAVE_H