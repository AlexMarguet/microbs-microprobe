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
	void writeHeader(string add); //For example pid parameters.

	void writeCsv(string content);

	void writeDataLine(float* data, int size);

private:
	/*
	fstream member is not compatible with constructor (or only copy-constructor ? to check), so we can't keep the file open and need to use open() each time we write.
	*/
	string m_file_name;
};

#endif // TOOLS_DATA_SAVE_H