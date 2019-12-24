#ifndef RISK_SERVICE_HPP
#define RISK_SERVICE_HPP

#include "soa.hpp"
#include "baseservice.hpp"
#include "positionservice.hpp"

/**
* PV01 risk.
* Type T is the product type.
*/
template<typename T>
class PV01
{

public:

	// ctor for a PV01 value
	PV01(const T &_product, double _pv01, long _quantity)
	{
		this->product = _product;
		this->pv01 = _pv01;
		this->quantity = _quantity;
	}

	PV01() : product(T()) {}

	// assignment
	const PV01<T>& operator=(const PV01<T>& other)
	{
		this->product = other.GetProduct();
		this->pv01 = other.GetPV01();
		this->quantity = other.GetQuantity();
		return *this;
	}

	// Get the product on this PV01 value
	const T& GetProduct() const {return product;}

	// Get the PV01 value
	double GetPV01() const { return pv01; }

	// Get the quantity that this risk value is associated with
	long GetQuantity() const { return quantity; }

private:
	T product;
	double pv01;
	long quantity;
};

/**
* A bucket sector to bucket a group of securities.
* We can then aggregate bucketed risk to this bucket.
* Type T is the product type.
*/
template<typename T>
class BucketedSector
{

public:

	// ctor for a bucket sector
	BucketedSector(const vector<T> &_products, string _name)
	{
		this->products = _products;
		this->name = _name;
	}

	BucketedSector() 
	{
		this->products = vector<T>();
		this->name = "";
	}

	// assignment
	const BucketedSector<T>& operator=(const BucketedSector<T>& other)
	{
		this->products = other.GetProducts();
		this->name = other.GetName();
		return *this;
	}

	// Get the products associated with this bucket
	const vector<T>& GetProducts() const { return products; }

	// Get the name of the bucket
	const string& GetName() const { return name; }

	string toString() const
	{

	}

private:
	vector<T> products;
	string name;
};

/**
* Risk Service to vend out risk for a particular security and across a risk bucketed sector.
* Keyed on product identifier.
* Type T is the product type.
*/
template<typename T>
class RiskService : public BaseService<PV01 <T> >
{

public:
	// Get data on our service given a key
	PV01<T>& GetData(string key) { return pvMap[key]; };

	// The callback that a Connector should invoke for any new or updated data
	void OnMessage(PV01<T> &data) { /* no op */ };

	// Add a position that the service will risk
	virtual void AddPosition(Position<T> &position) = 0;

	// Get the bucketed risk for the bucket sector
	virtual PV01< BucketedSector<T> >& GetBucketedRisk(const BucketedSector<T> &sector) = 0;

protected:
	// product id to PV01 map
	map<string, PV01<T>> pvMap;
};

class BondRiskService : public RiskService<Bond>
{
public:
	void AddPosition(Position<Bond>& position)
	{
		long quantity = position.GetAggregatePosition();
		double pv01v = calculatePV01(position.GetProduct());
		PV01<Bond> pv01(position.GetProduct(), pv01v, quantity);
		string productId = position.GetProduct().GetProductId();
		pvMap[productId] = pv01;

		this->callListeners(pv01, Action::ADD);
		this->printMessage("Added position in BondRiskService --> " + productId);
	}

	bool dataExists(string productId)
	{
		return this->pvMap.find(productId) != this->pvMap.end();
	}

	PV01<BucketedSector<Bond>>& GetBucketedRisk(const BucketedSector<Bond>& sector) 
	{
		vector<Bond> products = sector.GetProducts();
		
		long quantity = 0;
		double pv01 = 0;
		for (auto it = products.cbegin(); it != products.cend(); ++it)
		{
			string productId = (*it).GetProductId();
			if (this->dataExists(productId))
			{
				auto pv01Bond = this->GetData(productId);
				pv01 += pv01Bond.GetPV01();
				quantity += pv01Bond.GetQuantity();
			}
		}

		auto pvo1_bucketed = new PV01<BucketedSector<Bond>>(sector, pv01, quantity);
		return *pvo1_bucketed;
	}

	static BondRiskService* Instance()
	{
		if (!m_pInstance)   // Only allow one instance of class to be generated.
			m_pInstance = new BondRiskService();
		return m_pInstance;
	}

private:
	static BondRiskService* m_pInstance;

	double calculatePV01(Bond bond)
	{
		if (bond.GetTenor() == 2) return 0.01985;
		else if (bond.GetTenor() == 3) return 0.02930;
		else if (bond.GetTenor() == 5) return 0.04864;
		else if (bond.GetTenor() == 7) return 0.06584;
		else if (bond.GetTenor() == 10) return 0.08793;
		else if (bond.GetTenor() == 30) return 0.18469;
		else throw "unknown tenor";
	}
};

BondRiskService* BondRiskService::m_pInstance = NULL;

/*
---------- Listeners ---------------
*/
class BondRiskServiceListener : public ServiceListener<Position<Bond>>
{
public:
	BondRiskServiceListener(BondRiskService* _bondRiskService)
	{
		this->bondRiskService = _bondRiskService;
	}

	// Listener callback to process an add event to the Service
	void ProcessAdd(Position<Bond> &data) { bondRiskService->AddPosition(data); };

	// Listener callback to process a remove event to the Service
	void ProcessRemove(Position<Bond> &data) {};

	// Listener callback to process an update event to the Service
	void ProcessUpdate(Position<Bond> &data) {};

private:
	BondRiskService* bondRiskService;
};
#endif