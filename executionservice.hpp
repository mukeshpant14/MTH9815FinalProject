#ifndef EXECUTIONSERVICE_HPP
#define EXECUTIONSERVICE_HPP

#include <string>
#include <map>
#include <boost/log/trivial.hpp>

#include "soa.hpp"
#include "marketdataservice.hpp"
#include "executionorder.hpp"
#include "baseservice.hpp"

/**
* Service for executing orders on an exchange.
* Keyed on product identifier.
* Type T is the product type.
*/
template<typename T>
class ExecutionService : public BaseService<ExecutionOrder <T> >
{

public:
	ExecutionService() { exeuctionOrderMap = map<string, ExecutionOrder<T>>(); }

	// Get data on our service given a key
	ExecutionOrder<T>& GetData(string key) { return exeuctionOrderMap[key]; };

	// The callback that a Connector should invoke for any new or updated data
	void OnMessage(ExecutionOrder<T> &data) { /* no op */ };

	// Execute an order on a market
	virtual void ExecuteOrder(const ExecutionOrder<T>& order, Market market) = 0;

protected:
	map<string, ExecutionOrder<T>> exeuctionOrderMap;
};

class BondExecutionService : public ExecutionService<Bond>
{
public:
	// Execute an order on a market
	void ExecuteOrder(const ExecutionOrder<Bond>& order, Market market)
	{
		this->exeuctionOrderMap.emplace(order.GetProduct().GetProductId(), order);
		this->printMessage("Executing order " + order.toString() + " in BondExecutionService --> " + order.GetProduct().GetProductId());
	};

	static BondExecutionService* Instance()
	{
		if (!m_pInstance)   // Only allow one instance of class to be generated.
			m_pInstance = new BondExecutionService();
		return m_pInstance;
	}

private:
	static BondExecutionService* m_pInstance;
};

BondExecutionService* BondExecutionService::m_pInstance = NULL;

/* 
------------- Listeners -----------------
*/
class BondExecutionServiceListener : public ServiceListener<AlgoExecution<Bond>>
{
public:
	BondExecutionServiceListener(BondExecutionService* _bondExecutionService) : bondExecutionService(_bondExecutionService) {}

	// Listener callback to process an add event to the Service
	void ProcessAdd(AlgoExecution<Bond> &data) { this->bondExecutionService->ExecuteOrder(data.getExecutionOrder(), data.getMarket()); };

	// Listener callback to process a remove event to the Service
	void ProcessRemove(AlgoExecution<Bond> &data) {};

	// Listener callback to process an update event to the Service
	void ProcessUpdate(AlgoExecution<Bond> &data) {};

private:
	BondExecutionService* bondExecutionService;
};
#endif
