#ifndef STREAMINGSERVICE_HPP
#define STREAMINGSERVICE_HPP

#include "soa.hpp"
#include <map>
#include "baseservice.hpp"
#include "pricestream.hpp"
#include "products.hpp"
#include "SocketConnector.hpp"
#include "algostream.hpp"

/**
* Streaming service to publish two-way prices.
* Keyed on product identifier.
* Type T is the product type.
*/
template<typename T>
class StreamingService : public BaseService<PriceStream <T> >
{

public:
	// Get data on our service given a key
	PriceStream<T>& GetData(string key) { return priceStreamMap[key]; };

	// The callback that a Connector should invoke for any new or updated data
	void OnMessage(PriceStream<T> &data) { /* no op */ };

	// Publish two-way prices
	virtual void PublishPrice(PriceStream<T>& priceStream) = 0;

private:
	map<string, PriceStream<T>> priceStreamMap;
};

class BondStreamingService : public StreamingService<Bond>
{
public:
	void addPublishConnector(Connector<PriceStream<Bond>>* _socketPublisher)
	{
		this->socketPublisher = _socketPublisher;
	}

	void PublishPrice(PriceStream<Bond>& priceStream)
	{
		if (NULL == this->socketPublisher)
			throw "No publisher set";

		socketPublisher->Publish(priceStream);
		this->callListeners(priceStream, Action::ADD);
	}

	static BondStreamingService* Instance()
	{
		if (!m_pInstance)   // Only allow one instance of class to be generated.
			m_pInstance = new BondStreamingService();
		return m_pInstance;
	}
private:
	Connector<PriceStream<Bond>>* socketPublisher = NULL;
	static BondStreamingService* m_pInstance;
};

BondStreamingService* BondStreamingService::m_pInstance = NULL;

class BondSteamingServiceListener : public ServiceListener<AlgoStream<Bond>>
{
public:
	BondSteamingServiceListener(BondStreamingService * _service) : service(_service) {}

	// Listener callback to process an add event to the Service
	void ProcessAdd(AlgoStream<Bond> &data)
	{
		PriceStream<Bond> priceStream = data.getPriceStream();
		this->service->PublishPrice(priceStream);
	};

	// Listener callback to process a remove event to the Service
	void ProcessRemove(AlgoStream<Bond> &data) {};

	// Listener callback to process an update event to the Service
	void ProcessUpdate(AlgoStream<Bond> &data) {};

private:
	BondStreamingService* service;
};
#endif
