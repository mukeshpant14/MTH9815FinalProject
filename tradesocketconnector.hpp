#ifndef TRADESOCKETCONNECTOR_HPP
#define TRADESOCKETCONNECTOR_HPP

#include "SocketConnector.hpp"
#include "trade.hpp"

class TradeSocketConnector : public ServiceSubscribeSocketConnector<string, Trade<Bond>>
{
public:
	TradeSocketConnector(int _port, Service<string, Trade<Bond>>* _service, Service<string, Bond>* _bondService) :
		ServiceSubscribeSocketConnector(_port, _service), bondService(_bondService) {}

	Trade<Bond>* convertData(string& record)
	{
		std::vector<std::string> vec;
		std::stringstream data(record);
		std::string line;
		while (std::getline(data, line, ','))
			vec.push_back(line);

		if (vec[0] == "productId") // if header line
			return NULL;

		Quote price = Quote::fromString(vec[2]);
		Side side = "BUY" == vec[5] ? Side::BUY : Side::SELL;

		Bond bond = this->bondService->GetData(vec[0]);
		return new Trade<Bond>(bond, vec[1], price.getPrice(), vec[3], stoi(vec[4]), side);
	};

private:
	Service<string, Bond>* bondService;
};
#endif // !TRADESOCKETCONNECTOR_HPP
