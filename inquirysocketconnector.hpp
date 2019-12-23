#ifndef INQUIRYSOCKETCONNECTOR_HPP
#define INQUIRYSOCKETCONNECTOR_HPP

#include "SocketConnector.hpp"
#include "products.hpp"
#include "inquiry.hpp"
#include "quote.hpp"
#include "trade.hpp"

class InquirySocketConnector : public ServiceSubscribeSocketConnector<string, Inquiry<Bond>>
{
public:
	InquirySocketConnector(int _port, Service<string, Inquiry<Bond>>* _service, Service<string, Bond>* _bondService) :
		ServiceSubscribeSocketConnector(_port, _service), bondService(_bondService) {}

	Inquiry<Bond>* convertData(string& record)
	{
		std::vector<std::string> vec;
		std::stringstream data(record);
		std::string line;
		while (std::getline(data, line, ','))
			vec.push_back(line);

		if (vec[0] == "inquiryId") // if header line
			return NULL;

		Quote price = Quote::fromString(vec[4]);
		Side side = "BUY" == vec[2] ? Side::BUY : Side::SELL;

		Bond bond = this->bondService->GetData(vec[1]);
		InquiryState state = Inquiry<Bond>::getStateEnum(vec[5]);
		return new Inquiry<Bond>(vec[0], bond, side, stol(vec[3]), price.getPrice(), state);
	};

private:
	Service<string, Bond>* bondService;
};
#endif // !INQUIRYSOCKETCONNECTOR.HPP
