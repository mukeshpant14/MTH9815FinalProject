#ifndef BONDPRODUCTSERVICE_HPP
#define BONDPRODUCTSERVICE_HPP

/**
* productservice.hpp defines Bond and IRSwap ProductServices
*/

#include <iostream>
#include <map>
#include "soa.hpp"
#include "baseservice.hpp"
#include "products.hpp"

/**
* Bond Product Service to own reference data over a set of bond securities.
* Key is the productId string, value is a Bond.
*/
class BondProductService : public BaseService<Bond>
{

public:
	// BondProductService ctor
	BondProductService()
	{
		bondMap = map<string, Bond>();
	}

	// Return the bond data for a particular bond product identifier
	Bond& GetData(string productId)
	{
		return bondMap[productId];
	}

	// Add a bond to the service (convenience method)
	void Add(Bond &bond)
	{
		bondMap.insert(pair<string, Bond>(bond.GetProductId(), bond));
	}

	// The callback that a Connector should invoke for any new or updated data
	void OnMessage(Bond &data)
	{
		bondMap.insert(pair<string, Bond>(data.GetProductId(), data));
	}

	Bond findProductByTenor(int tenor)
	{
		for (auto it = bondMap.begin(); it != bondMap.end(); ++it)
			if ((*it).second.GetTenor() == tenor)
				return (*it).second;

		return Bond(); // may be throw
	}

	/** This function is called to create an instance of the class.
	Calling the constructor publicly is not allowed. The constructor
	is private and is only called by this Instance function.
	*/
	static BondProductService* Instance()
	{
		if (!m_pInstance)   // Only allow one instance of class to be generated.
			m_pInstance = new BondProductService();
		return m_pInstance;
	}

private:
	map<string, Bond> bondMap; // cache of bond products

	// Global static pointer used to ensure a single instance of the class.
	static BondProductService* m_pInstance;
};

BondProductService* BondProductService::m_pInstance = NULL;

#endif