#ifndef TOOLS_DATA_SAVE_H
#define TOOLS_DATA_SAVE_H

#include <iostream>
#include <fstream>
#include <filesystem>

using namespace std;

class DataSaver {
public:
	DataSaver();

	void createCsv(string name);

	void writeCsv(string content);

	void closeCsv();

private:
	fstream m_fout;

	string m_name;
};

#endif // TOOLS_DATA_SAVE_H