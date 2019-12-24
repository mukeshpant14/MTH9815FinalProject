#ifndef SOCKETCONNECTOR_HPP
#define SOCKETCONNECTOR_HPP

#include <string>
#include <boost/asio.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <iostream>
#include <vector>
#include "soa.hpp"

using namespace boost::asio::ip;
using namespace boost::asio;
using namespace std;

string ADDRESS = "127.0.0.1";
//string ADDRESS = "localhost";

template <typename T>
class SocketConnector : public Connector<T>
{
public:
	SocketConnector(int _port) : Connector<T>()
	{
		this->port = _port;
		this->ioservice = new io_service();
		tcp::endpoint endpoint(address::from_string(ADDRESS), this->port);
		//tcp::endpoint endpoint(tcp::v4(), this->port);
		this->socket = new tcp::socket(*ioservice);
	}

	/*virtual ~SocketConnector()
	{
		this->close();
		delete this->socket;
		std::cout << "Socket Closed on port " << this->port << std::endl;
	}*/

	void close() { this->socket->close(); }

protected:
	int port;
	io_service* ioservice;
	tcp::socket* socket;
	// The size of a fixed length header.
	enum { header_length = 8 };

	char inbound_header_[header_length];
};

template <typename T>
class SubscribeSocketConnector : public SocketConnector<T>
{
public:
	SubscribeSocketConnector(int _port) : SocketConnector<T>(_port) 
	{
		tcp::endpoint endpoint(address::from_string(ADDRESS), _port);
		//tcp::endpoint endpoint(tcp::v4(), _port);
		tcp::acceptor acceptor(*this->ioservice, endpoint);
		std::cout << "[Client] Trying to accept connection " << this->port << std::endl;
		acceptor.accept(*this->socket);
		std::cout << "[Client] Connection successful. Socket: " << ADDRESS << "," << _port << std::endl;
	}

	/*~SubscribeSocketConnector() {}
*/
	// Publish data to the Connector
	void Publish(T &data) { /*no op*/ };

	virtual void onData(T& data) = 0;
	
	void onDataComplete() {};

	// read data from socket
	void Subscribe()
	{
		std::cout << "Reading records from socket: " << ADDRESS << "," << this->port << std::endl;
		this->processedRecords = 0;
		int batch = 1;
		while (true) {
			boost::system::error_code error;
			read(*this->socket, boost::asio::buffer(this->inbound_header_), error);

			std::istringstream is(std::string(this->inbound_header_, this->header_length));
			std::size_t inbound_data_size = 0;
			is >> std::hex >> inbound_data_size;

			//// Start call to receive the data.
			inbound_data_.resize(inbound_data_size);
			read(*this->socket, boost::asio::buffer(inbound_data_), error);

			std::string archive_data(&inbound_data_[0], inbound_data_.size());
			std::istringstream archive_stream(archive_data);
			boost::archive::text_iarchive archive(archive_stream);
			T data;
			archive >> data;

			this->onData(data);
			
			// print some message to show how many records are processed
			++processedRecords;
			if ((processedRecords / (500000 * batch)) >= 1)
			{
				++batch;
				std::cout << "Processed: " << processedRecords << " records from socket: " << ADDRESS << "," << this->port << std::endl;
			}

			if (error == boost::asio::error::eof) break;
			if (error) 
			{
				std::cout << "Status: " << error.message() << std::endl;
				break;
			};
		};

		this->onDataComplete();
	}

private:
	std::vector<char> inbound_data_; 	/// Holds the inbound data.

	long processedRecords;
};

template <typename T>
class PublishSocketConnector : public SocketConnector<T>
{
public:
	PublishSocketConnector(int _port) : SocketConnector<T>(_port) 
	{
		tcp::endpoint endpoint(address::from_string(ADDRESS), this->port);
		//tcp::endpoint endpoint(tcp::v4(), this->port);
		std::cout << "[Server] Trying to connect " << this->port << std::endl;
		this->socket->connect(endpoint);
		std::cout << "[Server] Connection established" << this->port << std::endl;
	}

	void Subscribe() { /*no op */}  // may be throw?

	void Publish(T& data)
	{
		// Serialize the data first so we know how large it is.
		std::ostringstream archive_stream;
		boost::archive::text_oarchive archive(archive_stream);
		archive << data;
		string outbound_data = archive_stream.str();

		// Format the header.
		std::ostringstream header_stream;
		header_stream << std::setw(this->header_length) << std::hex << outbound_data.size();
		string outbound_header = header_stream.str();

		// Write the serialized data to the socket. We use "gather-write" to send
		// both the header and the data in a single write operation.
		std::vector<boost::asio::const_buffer> buffers;
		buffers.push_back(boost::asio::buffer(outbound_header));
		buffers.push_back(boost::asio::buffer(outbound_data));
		write(*this->socket, buffers);
	}
};

template <typename T, typename K>
class ServiceSubscribeSocketConnector : public SubscribeSocketConnector<T>
{
public:
	ServiceSubscribeSocketConnector(int _port, Service<string, K>* _service) : SubscribeSocketConnector<T>(_port), service(_service) {}

	//virtual bool convertData(T& data, K& convertedData) = 0;
	virtual K* convertData(T& data) = 0;

	void onData(T& data)
	{
		K* convertedData = this->convertData(data);
		if(NULL != convertedData)
			this->service->OnMessage(*convertedData);
	}

private:
	Service<string, K>* service;
};

#endif