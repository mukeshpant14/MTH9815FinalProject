#ifndef MARKETDATASOCKETCONNECTOR_HPP
#define MARKETDATASOCKETCONNECTOR_HPP

#include "products.hpp"
#include "quote.hpp"
#include "orderbook.hpp"
#include "SocketConnector.hpp"

class MarketDataSocketConnector : public ServiceSubscribeSocketConnector<string, OrderBook<Bond>>
{
public:
	MarketDataSocketConnector(int _port, Service<string, OrderBook<Bond>>* _service, Service<string, Bond>* _bondService) :
		ServiceSubscribeSocketConnector(_port, _service), bondService(_bondService) {}

	OrderBook<Bond>* convertData(string& record)
	{
		/*
		productId,bid,ask,quantity|
		912828YT1,99-141,99-151,10|
		*/
		std::vector<std::string> vec;
		std::stringstream data(record);
		std::string line;
		while (std::getline(data, line, ','))
			vec.push_back(line);

		if (vec[0] == "productId") // if header line
			return NULL;

		Quote bid = Quote::fromString(vec[1]);
		Quote ask = Quote::fromString(vec[2]);
		long quantity = stol(vec[3]);
		Bond bond = this->bondService->GetData(vec[0]);
		
		Order bidOrder(bid.getPrice(), quantity, PricingSide::BID);
		vector<Order> bidStack{ bidOrder };

		Order offerOrder(ask.getPrice(), quantity, PricingSide::OFFER);
		vector<Order> offerStack{ offerOrder };

		return new OrderBook<Bond>(bond, bidStack, offerStack);
	};

private:
	Service<string, Bond>* bondService;
};
#endif // !MARKETDATASOCKETCONNECTOR_HPP
