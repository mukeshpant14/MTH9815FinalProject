#ifndef GUISERVICE_HPP
#define GUISERVICE_HPP

#include <string>
#include <map>
#include "boost/date_time/posix_time/posix_time.hpp"
#include <boost/chrono.hpp>
#include <boost/thread/thread.hpp> 
#include "baseservice.hpp"
#include "price.hpp"
#include "generator.hpp"
#include <fstream>

class GuiService : public BaseService<Price<Bond>>
{
public:
	GuiService() 
	{
		file.open("gui.txt");
	}

	~GuiService()
	{
		this->file.close();
	}

	// Get data on our service given a key
	Price<Bond>& GetData(string key) { return priceMap[key]; };

	// The callback that a Connector should invoke for any new or updated data
	void OnMessage(Price<Bond> &data) 
	{ 
		auto time = boost::posix_time::microsec_clock::local_time();
		string message = "[GuiService - " + to_simple_string(time) + "] " + data.GetProduct().GetProductId() + ","
			+ data.GetAsk().toString() + "," + data.GetBid().toString();
		this->addMessage(message);
	};

	// generate file with prices
	void addMessage(string message)
	{
		this->file << message << "\n";
	}

	static GuiService* Instance()
	{
		if (!m_pInstance)   // Only allow one instance of class to be generated.
			m_pInstance = new GuiService();
		return m_pInstance;
	}

private:
	static GuiService* m_pInstance;
	ofstream file;

	map<string, Price<Bond>> priceMap;
};

GuiService* GuiService::m_pInstance = NULL;

class GuiServiceListener : public ServiceListener<Price<Bond>>
{
public:
	GuiServiceListener(long _throttleTime, GuiService* _guiService)
	{
		this->throttleTime = _throttleTime;
		this->guiService = _guiService;
		this->startTime = boost::posix_time::microsec_clock::local_time();
	}

	// Listener callback to process an add event to the Service
	void ProcessAdd(Price<Bond> &data)
	{ 
		if((boost::posix_time::microsec_clock::local_time() - startTime).total_milliseconds() < throttleTime) 
			return; //throttle messages

		if (++processedUpdates > noOfUpdatesToProcess)
			return;

		guiService->OnMessage(data);
		startTime = boost::posix_time::microsec_clock::local_time(); // reset
	};

	// Listener callback to process a remove event to the Service
	void ProcessRemove(Price<Bond> &data) {};

	// Listener callback to process an update event to the Service
	void ProcessUpdate(Price<Bond> &data) {};

private:
	GuiService* guiService;
	boost::posix_time::ptime startTime;

	long throttleTime;
	long processedUpdates = 0;
	long noOfUpdatesToProcess = 100;
};

#endif // !GUISERVICE_HPP
