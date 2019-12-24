#ifndef FILEREADERPROCESS_HPP
#define FILEREADERPROCESS_HPP

#include <iostream>
#include <string>
#include <fstream>
#include "boost/asio.hpp"
#include "soa.hpp"

using namespace boost::asio::ip;
using namespace std;

template <typename T>
class FileReaderProcess 
{
public:
	FileReaderProcess(Connector<T>* _connector)
	{
		this->connector = _connector;
	}

	void readData(string fileName)
	{
		ifstream infile(fileName);
		while (infile)
		{
			string s;
			if (!getline(infile, s)) break;

			this->connector->Publish(s);
		}

		this->connector->close();
		std::cout << "[FileReaderProcess] Completed reading records from " << fileName << " and sent to socket\n" ;
	};

private:
	Connector<T>* connector;
};

#endif
