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
		std::cout << "[Server] Read records and sent to socket. File - " << fileName << std::endl;
	};

private:
	Connector<T>* connector;
};

#endif
