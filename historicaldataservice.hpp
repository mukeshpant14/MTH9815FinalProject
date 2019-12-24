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
	void ProcessUpdate(T &data) { this->service->OnMessage(data); };

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
		: HistoricalDataService(new HistoricalDataFileConnector("hist_risk.txt", "persistKey,productId,pv01,quantity")) {}

	void OnMessage(PV01<Bond> &data)
	{
		auto time = boost::posix_time::microsec_clock::local_time();
		string persistKey = to_iso_extended_string(time) + "-" + data.GetProduct().GetProductId();
		this->dataMap.emplace(persistKey, data);
		this->PersistData(persistKey, data);
	};

	void PersistData(string persistKey, const PV01<Bond>& data)
	{
		string message = persistKey + "," + data.GetProduct().GetProductId() + "," +
			std::to_string(data.GetPV01()) + "," + std::to_string(data.GetQuantity());
		this->connector->Publish(message);

		// When persisting risk, we should persist risk for each security as well as for the 
		// following bucket sectors : FrontEnd(2Y, 3Y), Belly(5Y, 7Y, 10Y), and LongEnd(30Y)
		auto bondService = BondProductService::Instance();
		Bond bond2y = bondService->findProductByTenor(2);
		Bond bond3y = bondService->findProductByTenor(3); 
		Bond bond5y = bondService->findProductByTenor(5);
		Bond bond7y = bondService->findProductByTenor(7);
		Bond bond10y = bondService->findProductByTenor(10); 
		Bond bond30y = bondService->findProductByTenor(30);

		vector<Bond> frontEndBonds{ bond2y, bond3y };
		vector<Bond> bellyBonds{ bond5y, bond7y, bond10y };
		vector<Bond> longEndBonds{ bond30y };

		BucketedSector<Bond> frontEndBucket(frontEndBonds, "FrontEndBucket");
		BucketedSector<Bond> bellyBucket(bellyBonds, "BellyBucket");
		BucketedSector<Bond> longEndBucket(longEndBonds, "LongEndBucket");

		auto bondRiskService = BondRiskService::Instance();
		PV01<BucketedSector<Bond>> pv01Bucket1 = bondRiskService->GetBucketedRisk(frontEndBucket);
		PV01<BucketedSector<Bond>> pv01Bucket2 = bondRiskService->GetBucketedRisk(bellyBucket);
		PV01<BucketedSector<Bond>> pv01Bucket3 = bondRiskService->GetBucketedRisk(longEndBucket);

		string message1 = persistKey + "," + getBucketSectorMessage(pv01Bucket1);
		string message2 = persistKey + "," + getBucketSectorMessage(pv01Bucket2);
		string message3 = persistKey + "," + getBucketSectorMessage(pv01Bucket3);

		this->connector->Publish(message1);
		this->connector->Publish(message2);
		this->connector->Publish(message3);
	}

	string getBucketSectorMessage(PV01<BucketedSector<Bond>> pv01S)
	{
		return pv01S.GetProduct().GetName() + "," 
			+ std::to_string(pv01S.GetPV01()) + ","
			+ std::to_string(pv01S.GetQuantity());
	}

};

class BondExecutionHistoricalDataService : public HistoricalDataService<ExecutionOrder<Bond>>
{
public:
	BondExecutionHistoricalDataService()
		: HistoricalDataService(new HistoricalDataFileConnector("hist_executions.txt", 
			"persistKey,productId,side,orderId,orderType,price,visibleQuantity,hiddenQuantity,parentOrderId,isChildOrder" )) {}

	void OnMessage(ExecutionOrder<Bond> &data)
	{
		auto time = boost::posix_time::microsec_clock::local_time();
		string persistKey = to_iso_extended_string(time) + "-" + data.GetProduct().GetProductId();
		this->dataMap.emplace(persistKey, data);
		this->PersistData(persistKey, data);
	};

	void PersistData(string persistKey, const ExecutionOrder<Bond>& data)
	{
		string message = persistKey + "," + data.GetProduct().GetProductId() + "," + std::to_string(data.GetSide()) + "," +
			data.GetOrderId() + "," + data.getOrderTypeStr() + "," + std::to_string(data.GetPrice()) + "," +
			std::to_string(data.GetVisibleQuantity()) + "," + std::to_string(data.GetHiddenQuantity()) + "," +
			data.GetParentOrderId() + "," + std::to_string(data.IsChildOrder());
		
		this->connector->Publish(message);
	}
};

class BondStreamingHistoricalDataService : public HistoricalDataService<PriceStream<Bond>>
{
public:
	BondStreamingHistoricalDataService()
		: HistoricalDataService(new HistoricalDataFileConnector("hist_streaming.txt",
			"persistKey,productId,bidPrice,bidVisibleQuantity,bidHiddenQuantity,bidSide,askPrice,askVisibleQuantity,askHiddenQuantity,askSide")) {}

	void OnMessage(PriceStream<Bond> &data)
	{
		auto time = boost::posix_time::microsec_clock::local_time();
		string persistKey = to_iso_extended_string(time) + "-" + data.GetProduct().GetProductId();
		this->dataMap.emplace(persistKey, data);
		this->PersistData(persistKey, data);
	};

	void PersistData(string persistKey, const PriceStream<Bond>& data)
	{
		string message = persistKey + "," + data.GetProduct().GetProductId() + "," +
			data.GetBidOrder().toString() + "," + data.GetOfferOrder().toString();
		this->connector->Publish(message);
	}
};

class BondInquiryHistoricalDataService : public HistoricalDataService<Inquiry<Bond>>
{
public:
	BondInquiryHistoricalDataService()
		: HistoricalDataService(new HistoricalDataFileConnector("hist_inquiry.txt",
			"persistKey,inquiryId,productId,side,quantity,price,state")) {}

	void OnMessage(Inquiry<Bond> &data)
	{
		auto time = boost::posix_time::microsec_clock::local_time();
		string persistKey = to_iso_extended_string(time) + "-" + data.GetInquiryId();
		this->dataMap.emplace(persistKey, data);
		this->PersistData(persistKey, data);
	};

	void PersistData(string persistKey, const Inquiry<Bond>& data)
	{
		//string _inquiryId, const T &_product, Side _side, long _quantity, double _price, InquiryState _state
		string message = persistKey + "," + data.GetInquiryId() + "," + data.GetProduct().GetProductId() + "," + data.getStateStr();
		this->connector->Publish(message);
	}
};

#endif
