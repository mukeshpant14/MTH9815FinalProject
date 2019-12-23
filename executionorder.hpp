#ifndef EXECUTIONORDER_HPP
#define EXECUTIONORDER_HPP

#include <string>
#include "orderbook.hpp"

#include "orderbook.hpp"

using namespace std;

enum OrderType { FOK, IOC, MARKET, LIMIT, STOP };

enum Market { BROKERTEC, ESPEED, CME };

/**
* An execution order that can be placed on an exchange.
* Type T is the product type.
*/
template<typename T>
class ExecutionOrder
{
public:
	// ctor for an order
	ExecutionOrder(const T &_product, PricingSide _side, string _orderId, OrderType _orderType, double _price,
		double _visibleQuantity, double _hiddenQuantity, string _parentOrderId, bool _isChildOrder) : product(_product)
	{
		side = _side;
		orderId = _orderId;
		orderType = _orderType;
		price = _price;
		visibleQuantity = _visibleQuantity;
		hiddenQuantity = _hiddenQuantity;
		parentOrderId = _parentOrderId;
		isChildOrder = _isChildOrder;
	}

	// default
	ExecutionOrder() : product(T()) {}

	const ExecutionOrder& operator=(const ExecutionOrder& other) // copy assignment
	{
		this->product = other.product;
		this->side = other.side;
		this->orderId = other.orderId;
		this->orderType = other.orderType;
		this->price = other.price;
		this->visibleQuantity = other.visibleQuantity;
		this->hiddenQuantity = other.hiddenQuantity;
		this->parentOrderId = other.parentOrderId;
		this->isChildOrder = other.isChildOrder;
		return *this;
	}

	// Get the product
	const T& GetProduct() const { return this->product; }

	const PricingSide GetSide() const { return this->side; }

	// Get the order ID
	const string& GetOrderId() const { return this->orderId; }

	// Get the order type on this order
	OrderType GetOrderType() const { return this->orderType; }

	// Get the price on this order
	double GetPrice() const { return this->price; }

	// Get the visible quantity on this order
	long GetVisibleQuantity() const { return this->visibleQuantity; }

	// Get the hidden quantity
	long GetHiddenQuantity() const { return this->hiddenQuantity; }

	// Get the parent order ID
	const string& GetParentOrderId() const { return this->parentOrderId; }

	// Is child order?
	bool IsChildOrder() const { return this->isChildOrder; }

	const string toString() const
	{
		//return "";
		return "[order id: " + orderId + " ] --> at " + std::to_string(price);
	}

private:
	T product;
	PricingSide side;
	string orderId;
	OrderType orderType;
	double price;
	double visibleQuantity;
	double hiddenQuantity;
	string parentOrderId;
	bool isChildOrder;

};

#endif // !EXECUTIONORDER_HPP
