#ifndef POSITION_HPP
#define POSITION_HPP

#include <string>
#include "products.hpp"
#include "price.hpp"
#include "trade.hpp"

using namespace std;

/**
* Position class in a particular book.
* Type T is the product type.
*/
template<typename T>
class Position
{

public:

	// ctor for a position
	Position(const T &_product)
	{
		this->product = _product;
		this->positions = map<string, long>();
	}

	// ctor for a position
	Position() 
	{
		this->product = T();
		this->positions = map<string, long>();
	}

	// assignment
	const Position<T>& operator=(const Position<T>& other)
	{
		this->product = other.product;
		this->positions = other.positions;
		return *this;
	}

	// Get the product
	const T& GetProduct() const
	{
		return product;
	}

	// Get the position quantity
	long GetPosition(string &book)
	{
		return positions[book];
	}

	// Get the aggregate position
	long GetAggregatePosition()
	{
		long sum = 0;
		for (auto it = positions.begin(); it != positions.end(); ++it)
			sum += (*it).second;

		return sum;
	}

	void AppendPosition(string book, long position)
	{
		if (positions.find(book) == positions.end())
			positions.emplace(book, position);
		else
		{
			long aggPosition = positions[book] + position;
			positions.emplace(book, aggPosition);
		}
	}

private:
	T product;

	// position sum by book
	map<string, long> positions;
};

#endif // !POSITION_HPP