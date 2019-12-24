#ifndef ALGOEXECUTION_HPP
#define ALGOEXECUTION_HPP

#include "executionorder.hpp"

template <typename T>
class AlgoExecution
{
public:
	AlgoExecution(T& _product, ExecutionOrder<T>& _order, Market _market, Order _bid, Order _offer)
	{
		this->product = _product;
		this->order = _order;
		this->market = _market;
		this->bid = _bid;
		this->offer = _offer;
	}

	AlgoExecution() : order(ExecutionOrder<T>()) {}

	// assignment
	const AlgoExecution<T>& operator=(const AlgoExecution<T>& other)
	{
		this->product = other.product;
		this->order = other.order;
		this->market = other.market;
		this->bid = other.bid;
		this->offer = other.offer;
		return *this;
	}

	T getProduct() const { return this->product; }
	ExecutionOrder<T> getExecutionOrder() const { return this->order; }
	Market getMarket() const { return this->market; }
	Order getBid() const { return this->bid; }
	Order getOffer() const { return this->offer; }

private:
	T product;
	ExecutionOrder<T> order;
	Market market;
	Order bid;
	Order offer;
};

#endif // !ALGOEXECUTION_HPP
