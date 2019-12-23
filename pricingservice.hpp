#ifndef PRICINGSERVICE_HPP
#define PRICINGSERVICE_HPP

#include "soa.hpp"
#include "price.hpp"
#include "products.hpp"
#include "baseservice.hpp"

class PricingService : public BaseService<Price<Bond>>
{
public:
	PricingService()
	{
		priceMap = map<string, Price<Bond>>();
	}

	// Get data on our service given a key
	Price<Bond>& GetData(string key)
	{
		return priceMap[key];
	}

	// The callback that a Connector should invoke for any new or updated data
	void OnMessage(Price<Bond>& data)
	{
		Bond& product = dynamic_cast<Bond&>(data.GetProduct());
		string productId = product.GetProductId();
		priceMap.emplace(productId, data);
		this->callListeners(data, Action::ADD);
	}

	/** This function is called to create an instance of the class.
	Calling the constructor publicly is not allowed. The constructor
	is private and is only called by this Instance function.
	*/
	static PricingService* Instance()
	{
		if (!m_pInstance)   // Only allow one instance of class to be generated.
			m_pInstance = new PricingService();
		return m_pInstance;
	}

private:
	map<string, Price<Bond>> priceMap;

	// Global static pointer used to ensure a single instance of the class.
	static PricingService* m_pInstance;
};

PricingService* PricingService::m_pInstance = NULL;

#endif