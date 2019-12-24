#ifndef MARKET_DATA_SERVICE_HPP
#define MARKET_DATA_SERVICE_HPP

#include <map>
#include "baseservice.hpp"
#include "orderbook.hpp"
#include "products.hpp"

/**
* Market Data Service which distributes market data
* Keyed on product identifier.
* Type T is the product type.
*/
template<typename T>
class MarketDataService : public BaseService<OrderBook<T> >
{
public:
	// Get data on our service given a key
	OrderBook<T>& GetData(string key) { return this->orderBookMap[key]; };

	// Get the best bid/offer order
	virtual const BidOffer& GetBestBidOffer(const string &productId) = 0;

	// Aggregate the order book
	virtual const OrderBook<T>& AggregateDepth(const string &productId) = 0;

protected:
	map<string, OrderBook<T>> orderBookMap;
};

class BondMarketDataService : public MarketDataService<Bond>
{
public:
	// The callback that a Connector should invoke for any new or updated data
	void OnMessage(OrderBook<Bond> &data)
	{
		string productId = data.GetProduct().GetProductId();
		if (this->orderBookMap.find(productId) == this->orderBookMap.end()) // not found
		{
			this->orderBookMap[productId] = data;
		}
		else
		{
			this->orderBookMap[productId].GetBidStack().push_back(data.GetBidStack().at(0));
			this->orderBookMap[productId].GetOfferStack().push_back(data.GetOfferStack().at(0)); 
		}

		this->printMessage("Added order to order book in BondMarketDataService --> " + productId);
		this->callListeners(data, Action::ADD);
	}

	// Get the best bid/offer order
	const BidOffer& GetBestBidOffer(const string &productId)
	{
		OrderBook<Bond> orderBook = this->orderBookMap[productId];

		auto minFunc = [](Order o1, Order o2) -> bool {
			return (o1.GetPrice() < o2.GetPrice());
		};

		auto minBidOrder = std::max_element(orderBook.GetBidStack().begin(), orderBook.GetBidStack().end(), minFunc);
		auto maxOfferOrder = std::min_element(orderBook.GetOfferStack().begin(), orderBook.GetOfferStack().end(), minFunc);

		BidOffer* bidOffer = new BidOffer(*minBidOrder, *maxOfferOrder);
		return *bidOffer;
	}

	const OrderBook<Bond>& AggregateDepth(const string &productId)
	{
		OrderBook<Bond>* b = new OrderBook<Bond>();
		return *b; // TODO
	}

	static BondMarketDataService* Instance()
	{
		if (!m_pInstance)   // Only allow one instance of class to be generated.
			m_pInstance = new BondMarketDataService();
		return m_pInstance;
	}

private:
	static BondMarketDataService* m_pInstance;
};

BondMarketDataService* BondMarketDataService::m_pInstance = NULL;
#endif
