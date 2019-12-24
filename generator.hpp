#ifndef GENERATOR_HPP
#define GENERATOR_HPP

#include <iostream>
#include <string>
#include <vector>
#include <fstream>

using namespace std;

class FileGenerator
{
private:
	string filename;
	string rowdelimiter = "";

	ofstream file;

public:
	virtual void generate() = 0;

	FileGenerator(string filename)
	{
		this->filename = filename;
		file.open(this->filename);
	}

	~FileGenerator()
	{
		file.close();
	}

	void addHeader(string header) { this->file << header << rowdelimiter << "\n"; }
	void addRow(string row) 
	{ 
		this->file << row << rowdelimiter << "\n";
		this->file.flush();
	}

	string getFileName() { return this->filename; }
};

#endif // !GENERATOR_HPP