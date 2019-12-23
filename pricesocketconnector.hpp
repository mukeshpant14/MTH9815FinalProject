#ifndef PRICEDATASOCKETCONNECTOR_HPP
#define PRICEDATASOCKETCONNECTOR_HPP

#include "soa.hpp"
#include "price.hpp"
#include "SocketConnector.hpp"
#include "products.hpp"

class PriceDataSocketConnector : public ServiceSubscribeSocketConnector<string, Price<Bond>>
{
public:
	PriceDataSocketConnector(int _port, Service<string, Price<Bond>>* _service, Service<string, Bond>* _bondService) 
		: ServiceSubscribeSocketConnector(_port, _service)
	{ this->bondService = _bondService; }

	Price<Bond>* convertData(string& record)
	{
		std::vector<std::string> vec;
		std::stringstream data(record);
		std::string line;
		while (std::getline(data, line, ','))
			vec.push_back(line);

		if (vec[0] == "productId") // if header line
			return NULL;

		Quote bid = Quote::fromString(vec[1]);
		Quote ask = Quote::fromString(vec[2]);
		Bond bond = bondService->GetData(vec[0]);

		return new Price<Bond>(bond, bid, ask);
	};

private:
	Service<string, Bond>* bondService;
};

#endif