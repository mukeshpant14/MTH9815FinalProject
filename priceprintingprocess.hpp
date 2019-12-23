#ifndef PRICEPRINTINGPROCESS_HPP
#define PRICEPRINTINGPROCESS_HPP

#include "products.hpp"
#include "pricestream.hpp"
#include "SocketConnector.hpp"

using namespace std;

class PricePrintingProcess
{
public:
	void print(PriceStream<Bond> priceStream)
	{
		if (++printCount <= MAX_PRINT_COUNT)
			std::cout << "[PricePrintingProcess] : " << priceStream.toString() << std::endl;
	}
private:
	int printCount = 0; // 
	int MAX_PRINT_COUNT = 10;
};

class PriceStreamSubscribeSocketConnector : public SubscribeSocketConnector<PriceStream<Bond>>
{
public:
	PriceStreamSubscribeSocketConnector(int _port, PricePrintingProcess _process)
		: SubscribeSocketConnector<PriceStream<Bond>>(_port) 
	{
		this->process = _process;
	}

	void onData(PriceStream<Bond>& priceStream)
	{
		process.print(priceStream);
	}

private:
	PricePrintingProcess process;
};

#endif // !PRICEPRINTINGPROCESS_HPP
