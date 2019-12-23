#ifndef ORDERBOOK_HPP
#define ORDERBOOK_HPP

#include <string>
#include <vector>
#include "products.hpp"
#include "price.hpp"
#include "trade.hpp"

using namespace std;

// Side for market data
enum PricingSide { BID, OFFER };

/**
* A market data order with price, quantity, and side.
*/
class Order
{

public:

	// ctor for an order
	Order(double _price, long _quantity, PricingSide _side)
	{
		this->price = _price;
		this->quantity = _quantity;
		this->side = _side;
	}

	Order() {}

	// assignment
	const Order& operator=(const Order& other)
	{
		this->price = other.price;
		this->quantity = other.quantity;
		this->side = other.side;
		return *this;
	}

	// Get the price on the order
	double GetPrice() const { return this->price;  }

	// Get the quantity on the order
	long GetQuantity() const { return this->quantity; }

	// Get the side on the order
	PricingSide GetSide() const { return this->side; }

private:
	double price;
	long quantity;
	PricingSide side;

};

/**
* Class representing a bid and offer order
*/
class BidOffer
{

public:

	// ctor for bid/offer
	BidOffer(const Order &_bidOrder, const Order &_offerOrder)
	{
		this->bidOrder = _bidOrder;
		this->offerOrder = _offerOrder;
	}

	// Get the bid order
	const Order& GetBidOrder() const { return this->bidOrder; }

	// Get the offer order
	const Order& GetOfferOrder() const { return this->offerOrder; }

private:
	Order bidOrder;
	Order offerOrder;

};

/**
* Order book with a bid and offer stack.
* Type T is the product type.
*/
template<typename T>
class OrderBook
{

public:

	// ctor for the order book
	OrderBook(const T &_product, const vector<Order> &_bidStack, const vector<Order> &_offerStack)
	{
		this->product = _product;
		this->bidStack = _bidStack;
		this->offerStack = _offerStack;
	}

	OrderBook()
	{
		this->product = T();
		this->bidStack = vector<Order>();
		this->offerStack = vector<Order>();
	}

	// assignment
	const OrderBook& operator=(const OrderBook& other)
	{
		this->product = other.product;
		this->bidStack = other.bidStack;
		this->offerStack = other.offerStack;
		return *this;
	}

	// Get the product
	const T& GetProduct() const { return this->product; }

	// Get the bid stack
	const vector<Order>& GetBidStack() const { return this->bidStack; }

	// Get the offer stack
	const vector<Order>& GetOfferStack() const { return this->offerStack; }

private:
	T product;
	vector<Order> bidStack;
	vector<Order> offerStack;

};

#endif // !ORDERBOOK_HPP
