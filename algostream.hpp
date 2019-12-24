#ifndef ALGOSTREAM_HPP
#define ALGOSTREAM_HPP

#include "pricestream.hpp"

using namespace std;

template <typename T>
class AlgoStream
{
public:
	AlgoStream(T& _product, PriceStream<T> _priceStream)
	{
		this->product = _product;
		this->priceStream = _priceStream;
	}

	AlgoStream() {}

	// assignment
	const AlgoStream<T>& operator=(const AlgoStream<T>& other)
	{
		this->product = other.product;
		this->priceStream = other.priceStream;
		return *this;
	}

	PriceStream<T> getPriceStream() const { return priceStream; }
	T getProduct() const { return this->product; }

private:
	T product;
	PriceStream<T> priceStream;

};
#endif // !ALGOSTREAM_HPP
