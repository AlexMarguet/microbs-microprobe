#ifndef TOOLS_DATA_SAVER_H
#define TOOLS_DATA_SAVER_H

#include <iostream>
#include <fstream>
#include <filesystem>

using namespace std;

class DataSaver {
public:
	static const char separator = '\t';
	
	DataSaver();

	void createTsv(string name);

	void writeTsv(string content);

	void writeTsv(int size, float* load);

	void closeTsv();

private:
	fstream m_fout;

	string m_name;
};

#endif // TOOLS_DATA_SAVE_H