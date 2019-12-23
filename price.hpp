#ifndef PRICE_HPP
#define PRICE_HPP

#include <string>
#include "quote.hpp"
#include "products.hpp"

using namespace std;

/**
* A price object consisting of mid and bid/offer spread.
* Type T is the product type.
*/
template<typename T>
class Price
{
public:
	// ctor for a price
	Price(T &_product, Quote _bid, Quote _ask) : product(_product), bid(_bid), ask(_ask) {}
	Price() {}

	// assignment
	const Price<T>& operator=(const Price<T>& other)
	{
		this->product = other.product;
		this->bid = other.bid;
		this->ask = other.ask;
		return *this;
	}

	// Get the product
	T GetProduct() const { return product; }

	// Get the mid price
	Quote GetBid() const { return bid; }

	// Get the bid/offer spread around the mid
	Quote GetAsk() const { return ask; }

private:
	T product;
	Quote bid;
	Quote ask;
};

#endif