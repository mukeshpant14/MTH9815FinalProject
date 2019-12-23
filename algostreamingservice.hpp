#ifndef ALGOSTREAMINGSERVICE_HPP
#define ALGOSTREAMINGSERVICE_HPP

#include <string>
#include <map>
#include "baseservice.hpp"
#include "algostream.hpp"
#include "products.hpp"
#include "executionorder.hpp"
#include "AlgoExecution.hpp"

using namespace std;

template <typename T>
class AlgoStreamingService : public BaseService<AlgoStream<T>>
{
public:
	// The callback that a Connector should invoke for any new or updated data
	void OnMessage(AlgoStream<T> &data)
	{ 
		this->callListeners(data, Action::ADD);
	}

	// Get data on our service given a key
	AlgoStream<T>& GetData(string key) { return algoStreamMap[key]; };

	void ProcessAlgoExecution(AlgoExecution<T>& data) 
	{ 
		ExecutionOrder<T> executionOrder = data.getExecutionOrder();
		PriceStreamOrder bidOrder(executionOrder.GetPrice(), executionOrder.GetVisibleQuantity(),
			executionOrder.GetHiddenQuantity(), executionOrder.GetSide());
		// TODO
		PriceStreamOrder offerOrder(executionOrder.GetPrice(), executionOrder.GetVisibleQuantity(),
			executionOrder.GetHiddenQuantity(), executionOrder.GetSide());

		PriceStream<Bond> priceStream(data.getExecutionOrder().GetProduct(), bidOrder, offerOrder);
		AlgoStream<Bond> algoStream(priceStream);
		this->OnMessage(algoStream);
	}

private:
	map<string, AlgoStream<T>> algoStreamMap;
};

class BondAlgoStreamingService : public AlgoStreamingService<Bond>
{
public:
	static BondAlgoStreamingService* Instance()
	{
		if (!m_pInstance)   // Only allow one instance of class to be generated.
			m_pInstance = new BondAlgoStreamingService();
		return m_pInstance;
	}
private:
	static BondAlgoStreamingService* m_pInstance;
};

BondAlgoStreamingService* BondAlgoStreamingService::m_pInstance = NULL;

/*
------------- listeners ----------------------
*/
class BondAlgoStreamingServiceListener : public ServiceListener<AlgoExecution<Bond>>
{
public:
	BondAlgoStreamingServiceListener(BondAlgoStreamingService * _bondAlgoStreamingService) 
		: bondAlgoStreamingService(_bondAlgoStreamingService) {}

	// Listener callback to process an add event to the Service
	void ProcessAdd(AlgoExecution<Bond> &data)
	{
		this->bondAlgoStreamingService->ProcessAlgoExecution(data);
	};

	// Listener callback to process a remove event to the Service
	void ProcessRemove(AlgoExecution<Bond> &data) {};

	// Listener callback to process an update event to the Service
	void ProcessUpdate(AlgoExecution<Bond> &data) {};

private:
	BondAlgoStreamingService* bondAlgoStreamingService;
};
#endif // !ALGOSTREAMINGSERVICE_HPP
