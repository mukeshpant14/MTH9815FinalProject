#ifndef POSITION_SERVICE_HPP
#define POSITION_SERVICE_HPP

#include <string>
#include <map>
#include "soa.hpp"
#include "tradebookingservice.hpp"
#include "position.hpp"

using namespace std;

/**
* Position Service to manage positions across multiple books and secruties.
* Keyed on product identifier.
* Type T is the product type.
*/
template<typename T>
class PositionService : public BaseService<Position <T> >
{

public:
	PositionService() { positionMap = map<string, Position<T>>(); }

	// Get data on our service given a key
	Position<T>& GetData(string key) { return positionMap[key]; }

	// The callback that a Connector should invoke for any new or updated data
	void OnMessage(Position<T> &data) { /* no op */ };
	
	// Add a trade to the service
	virtual void AddTrade(const Trade<T> &trade) = 0;

protected:
	map<string, Position<T>> positionMap;
};

class BondPositionService : public PositionService<Bond>
{
public:
	void AddTrade(const Trade<Bond>& trade)
	{
		string book = trade.GetBook();
		long newPosition = trade.GetQuantity(); //  *trade.GetPrice(); is it quantity or value?
		newPosition = trade.GetSide() == Side::BUY ? newPosition : (-1 * newPosition);

		string productId = trade.GetProduct().GetProductId();
		Position<Bond> position;
		if (positionMap.find(productId) == positionMap.end())
		{
			Bond p = trade.GetProduct();
			position = Position<Bond>(p);
			position.AppendPosition(book, newPosition);
			positionMap.emplace(productId, position);
		}
		else 
		{
			position = positionMap[productId];
			position.AppendPosition(book, newPosition);
			positionMap[productId] = position;
		}

		this->printMessage("Added trade in BondPositionService --> " + trade.GetTradeId());
		this->callListeners(position, Action::ADD);
	}

	static BondPositionService* Instance()
	{
		if (!m_pInstance)   // Only allow one instance of class to be generated.
			m_pInstance = new BondPositionService();
		return m_pInstance;
	}

private:
	static BondPositionService* m_pInstance;
};

BondPositionService* BondPositionService::m_pInstance = NULL;

class BondPositionServiceListener : public ServiceListener<Trade<Bond>>
{
public:
	BondPositionServiceListener(BondPositionService* _bondPositionService)
	{
		this->bondPositionService = _bondPositionService;
	}

	// Listener callback to process an add event to the Service
	void ProcessAdd(Trade<Bond> &data) { bondPositionService->AddTrade(data); };

	// Listener callback to process a remove event to the Service
	void ProcessRemove(Trade<Bond> &data) {};

	// Listener callback to process an update event to the Service
	void ProcessUpdate(Trade<Bond> &data) {};

private:
	BondPositionService* bondPositionService;
};
#endif
