#ifndef PRICESTREAM_HPP
#define PRICESTREAM_HPP

#include "orderbook.hpp"

/**
* A price stream order with price and quantity (visible and hidden)
*/
class PriceStreamOrder
{

public:

	// ctor for an order
	PriceStreamOrder(double _price, long _visibleQuantity, long _hiddenQuantity, PricingSide _side)
		: price(_price), visibleQuantity(_visibleQuantity), hiddenQuantity(_hiddenQuantity), side(_side) {}

	PriceStreamOrder() {}

	// assignment
	const PriceStreamOrder& operator=(const PriceStreamOrder& other)
	{
		this->price = other.price;
		this->visibleQuantity = other.visibleQuantity;
		this->hiddenQuantity = other.hiddenQuantity;
		this->side = other.side;
		return *this;
	}

	// The side on this order
	PricingSide GetSide() const { return this->side; }
	string getSideStr() const
	{
		switch (this->side)
		{
		case PricingSide::BID: return "BID";
		case PricingSide::OFFER: return "OFFER";
		default: throw "unrecognized side";
		}
	}

	// Get the price on this order
	double GetPrice() const { return this->price; }

	// Get the visible quantity on this order
	long GetVisibleQuantity() const { return this->visibleQuantity; }

	// Get the hidden quantity on this order
	long GetHiddenQuantity() const { return this->hiddenQuantity; }

	string toString() const
	{
		return std::to_string(price) + "," + std::to_string(visibleQuantity) + "," + std::to_string(hiddenQuantity)
			+ "," + this->getSideStr();
	}
private:
	friend class boost::serialization::access;

	double price;
	long visibleQuantity;
	long hiddenQuantity;
	PricingSide side;

	template <typename Archive>
	void serialize(Archive &ar, const unsigned int version)
	{
		ar & price;
		ar & visibleQuantity;
		ar & hiddenQuantity;
		ar & side;
	}
};

/**
* Price Stream with a two-way market.
* Type T is the product type.
*/
template<typename T>
class PriceStream
{

public:

	// ctor
	PriceStream(const T &_product, const PriceStreamOrder &_bidOrder, const PriceStreamOrder &_offerOrder)
		: product(_product), bidOrder(_bidOrder), offerOrder(_offerOrder) {}

	PriceStream() : product(T()), bidOrder(PriceStreamOrder()), offerOrder(PriceStreamOrder()) {}

	// assignment
	const PriceStream<T>& operator=(const PriceStream<T>& other)
	{
		this->product = other.product;
		this->bidOrder = other.bidOrder;
		this->offerOrder = other.offerOrder;
		return *this;
	}

	// Get the product
	const T& GetProduct() const { return this->product; }

	// Get the bid order
	const PriceStreamOrder& GetBidOrder() const { return this->bidOrder; }

	// Get the offer order
	const PriceStreamOrder& GetOfferOrder() const { return this->offerOrder; }

	string toString()
	{
		return product.toString() + "," + bidOrder.toString() + "," + offerOrder.toString();
	}

private:
	friend class boost::serialization::access;

	T product;
	PriceStreamOrder bidOrder;
	PriceStreamOrder offerOrder;

	template <typename Archive>
	void serialize(Archive &ar, const unsigned int version)
	{
		ar & product;
		ar & bidOrder;
		ar & offerOrder;
	}
};

#endif // !PRICESTREAM_HPP
