#ifndef TRADE_HPP
#define TRADE_HPP

#include <string>
#include "products.hpp"

using namespace std;

// Trade sides
enum Side { BUY, SELL };

/**
* Trade object with a price, side, and quantity on a particular book.
* Type T is the product type.
*/
template<typename T>
class Trade
{
public:
	// ctor for a trade
	Trade(const T &_product, string _tradeId, double _price, string _book, long _quantity, Side _side)
	{
		product = _product;
		tradeId = _tradeId;
		price = _price;
		book = _book;
		quantity = _quantity;
		side = _side;
	}

	Trade() : product(T()) {}

	// assignment
	const Trade<T>& operator=(const Trade<T>& other)
	{
		this->product = other.GetProduct();
		this->tradeId = other.GetTradeId();
		this->price = other.GetPrice();
		this->book = other.GetBook();
		this->quantity = other.GetQuantity();
		this->side = other.GetSide();
		return *this;
	}

	// Get the product
	const T& GetProduct() const { return product; }

	// Get the trade ID
	const string& GetTradeId() const { return tradeId; }

	// Get the mid price
	double GetPrice() const { return price; }

	// Get the book
	const string& GetBook() const { return book; }

	// Get the quantity
	long GetQuantity() const { return quantity; }

	// Get the side
	Side GetSide() const { return side; }

private:
	T product;
	string tradeId;
	double price;
	string book;
	long quantity;
	Side side;
};

#endif // !TRADE_HPP