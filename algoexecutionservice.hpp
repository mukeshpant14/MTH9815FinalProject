#ifndef ALGOEXECUTIONSERVICE_HPP
#define ALGOEXECUTIONSERVICE_HPP

#include <map>
#include <string>

#include "baseservice.hpp"
#include "AlgoExecution.hpp"
#include "products.hpp"
#include "executionorder.hpp"

using namespace std;

template <typename T>
class AlgoExecutionService : public BaseService<AlgoExecution<T>>
{
public:
	AlgoExecutionService() { algoExecutionMap = map<string, AlgoExecution<T>>(); }

	// Get data on our service given a key
	AlgoExecution<T>& GetData(string key) { return algoExecutionMap[key]; };

	// The callback that a Connector should invoke for any new or updated data
	void OnMessage(AlgoExecution<T> &data) { /* no op */ };

	virtual void ExecuteAlgo(OrderBook<T> orderBook) = 0;

private:
	map<string, AlgoExecution<T>> algoExecutionMap;
};

class BondAlgoExecutionService : public AlgoExecutionService<Bond>
{
public:
	void ExecuteAlgo(OrderBook<Bond> orderBook)
	{
		// create a object of algo execution based on the algo using order book
		Bond const bond = orderBook.GetProduct();

		// go over each bid and ask order and check if spread if 1/128
		vector<AlgoExecution<Bond>> algoExecutionOrders;
		for (unsigned int i = 0; i < orderBook.GetBidStack().size(); i++)
		{
			Order bid = orderBook.GetBidStack().at(i);
			Order ask = orderBook.GetOfferStack().at(i);

			if ((ask.GetPrice() - bid.GetPrice()) == 1.0 / 128)
			{
				PricingSide side = i % 2 == 0 ? PricingSide::BID : PricingSide::OFFER;
				ExecutionOrder<Bond> order(bond, side, "orderId_" + bond.GetProductId() + "_" + std::to_string(i), OrderType::LIMIT,
					(side == PricingSide::BID ? ask.GetPrice() : bid.GetPrice()), bid.GetQuantity(), bid.GetQuantity() * 2, "", false);

				AlgoExecution<Bond> executionOrder(order, Market::CME);
				algoExecutionOrders.push_back(executionOrder);
			}
		}

		for(auto it = algoExecutionOrders.begin(); it != algoExecutionOrders.end(); ++it)
			this->callListeners(*it, Action::ADD);
	}

	static BondAlgoExecutionService* Instance()
	{
		if (!m_pInstance)   // Only allow one instance of class to be generated.
			m_pInstance = new BondAlgoExecutionService();
		return m_pInstance;
	}
private:
	static BondAlgoExecutionService* m_pInstance;
};

BondAlgoExecutionService* BondAlgoExecutionService::m_pInstance = NULL;

/*
--------------- Listeners --------------------
*/
class BondAlgoExecutionServiceListener : public ServiceListener<OrderBook<Bond>>
{
public:
	BondAlgoExecutionServiceListener(BondAlgoExecutionService * _bondAlgoExecutionService) 
		: bondAlgoExecutionService(_bondAlgoExecutionService) {}
	
	// Listener callback to process an add event to the Service
	void ProcessAdd(OrderBook<Bond> &data) { bondAlgoExecutionService->ExecuteAlgo(data); };

	// Listener callback to process a remove event to the Service
	void ProcessRemove(OrderBook<Bond> &data) {};

	// Listener callback to process an update event to the Service
	void ProcessUpdate(OrderBook<Bond> &data) {};

private:
	BondAlgoExecutionService* bondAlgoExecutionService;
};
#endif // !ALGOEXECUTIONSERVICE_HPP
