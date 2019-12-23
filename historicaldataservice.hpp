#ifndef HISTORICALDATASERVICE_HPP
#define HISTORICALDATASERVICE_HPP

#include "soa.hpp"
#include "products.hpp"
#include "position.hpp"
#include "baseservice.hpp"
#include "boost/date_time/posix_time/posix_time.hpp"
#include <fstream>
#include <map>

class HistoricalDataFileConnector : public Connector<string>
{
public:
	HistoricalDataFileConnector(string _fileName, string header)
	{
		this->fileName = _fileName;
		this->file.open(_fileName);

		this->file << header << "\n";
	}

	~HistoricalDataFileConnector()
	{
		this->file.flush();
		this->close();
	}

	// Publish data to the Connector
	void Publish(string &data)
	{
		this->file << data << "\n";
		this->file.flush();
	}

	void Subscribe() {};
	void close() { this->file.close(); };

private:
	string fileName;
	ofstream file;
};

/**
* Service for processing and persisting historical data to a persistent store.
* Keyed on some persistent key.
* Type T is the data type to persist.
*/
template<typename T>
class HistoricalDataService : public BaseService<T>
{
public:
	HistoricalDataService(HistoricalDataFileConnector* _connector) : connector(_connector) {}

	// Persist data to a store
	virtual void PersistData(string persistKey, const T& data) = 0;

	// Get data on our service given a key
	T& GetData(string key) { return dataMap[key]; };

protected:
	HistoricalDataFileConnector* connector;

	map<string, T> dataMap;
};

template <typename T>
class HistoricalDataServiceListener : public ServiceListener<T>
{
public:
	HistoricalDataServiceListener(HistoricalDataService<T>* _service) : service(_service) {}

	// Listener callback to process an add event to the Service
	void ProcessAdd(T &data) { this->service->OnMessage(data); };

	// Listener callback to process a remove event to the Service
	void ProcessRemove(T &data) {};

	// Listener callback to process an update event to the Service
	void ProcessUpdate(T &data) {};

private:
	HistoricalDataService<T>* service;
};

class BondPositionHistoricalDataService : public HistoricalDataService<Position<Bond>>
{
public:
	BondPositionHistoricalDataService() 
		: HistoricalDataService(new HistoricalDataFileConnector("hist_positions.txt", "persistKey,productId,book,position,aggregatedPosition")) {}

	void OnMessage(Position<Bond> &data)
	{  
		auto time = boost::posix_time::microsec_clock::local_time();
		string persistKey = to_iso_extended_string(time) + "-" + data.GetProduct().GetProductId();
		this->dataMap.emplace(persistKey, data);
		this->PersistData(persistKey, data);
	};

	void PersistData(string persistKey, const Position<Bond>& data)
	{
		map<string, long> positions = data.getPositions();
		for (auto it = positions.begin(); it != positions.end(); ++it)
		{
			// persistKey,productId,book,position,aggregatedPosition
			string message = persistKey + "," + data.GetProduct().GetProductId() + "," + (*it).first + "," + std::to_string((*it).second) + "," 
				+ std::to_string(data.GetAggregatePosition());
			this->connector->Publish(message);
		};
	}
};

class BondRiskHistoricalDataService : public HistoricalDataService<PV01<Bond>>
{
public:
	BondRiskHistoricalDataService()
		: HistoricalDataService(new HistoricalDataFileConnector("hist_risk.txt", "persistKey,productId,tenor,pv01,quantity")) {}

	void OnMessage(PV01<Bond> &data)
	{
		auto time = boost::posix_time::microsec_clock::local_time();
		string persistKey = to_iso_extended_string(time) + "-" + data.GetProduct().GetProductId();
		this->dataMap.emplace(persistKey, data);
		this->PersistData(persistKey, data);
	};

	void PersistData(string persistKey, const PV01<Bond>& data)
	{
		string message = persistKey + "," + data.GetProduct().GetProductId() + "," + std::to_string(data.GetProduct().GetTenor()) + "," +
			std::to_string(data.GetPV01()) + "," + std::to_string(data.GetQuantity());
		this->connector->Publish(message);

		// bucketed pv01 logic here
	}
};

#endif
