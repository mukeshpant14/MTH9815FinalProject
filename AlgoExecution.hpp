#ifndef ALGOEXECUTION_HPP
#define ALGOEXECUTION_HPP

#include "executionorder.hpp"

template <typename T>
class AlgoExecution
{
public:
	AlgoExecution(ExecutionOrder<T>& _order, Market _market)
	{
		this->order = _order;
		this->market = _market;
	}

	AlgoExecution() : order(ExecutionOrder<T>()) {}

	ExecutionOrder<T> getExecutionOrder() { return this->order; }
	Market getMarket() { return this->market; }

private:
	ExecutionOrder<T> order;

	Market market;
};

#endif // !ALGOEXECUTION_HPP
