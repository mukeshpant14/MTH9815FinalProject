#ifndef BONDPRODUCTSOCKETCONNECTOR_HPP
#define BONDPRODUCTSOCKETCONNECTOR_HPP

#include "soa.hpp"
#include "products.hpp"
#include "SocketConnector.hpp"

class BondProductSocketConnector : public ServiceSubscribeSocketConnector<string, Bond>
{
public:
	BondProductSocketConnector(int _port, Service<string, Bond>* _service) : ServiceSubscribeSocketConnector(_port, _service) {}

	Bond* convertData(string& record)
	{
		std::vector<std::string> vec;
		std::stringstream data(record);
		std::string line;
		while (std::getline(data, line, ','))
			vec.push_back(line);

		if (vec[0] == "CUSIP") // if header line
			return NULL;

		date issueDate = date_from_iso_string(vec[2]);
		date maturityDate = date_from_iso_string(vec[3]);
		float coupon = stof(vec[1]);

		return new Bond(vec[0], BondIdType::CUSIP, vec[4], coupon, maturityDate, stoi(vec[5]));
	}
};

#endif // !BONDPRODUCTSOCKETCONNECTOR_HPP