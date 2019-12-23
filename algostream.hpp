#ifndef ALGOSTREAM_HPP
#define ALGOSTREAM_HPP

#include "pricestream.hpp"

using namespace std;

template <typename T>
class AlgoStream
{
public:
	AlgoStream(PriceStream<T> _priceStream)
	{
		this->priceStream = _priceStream;
	}

	AlgoStream() {}

	// assignment
	const AlgoStream<T>& operator=(const AlgoStream<T>& other)
	{
		this->priceStream = other.priceStream;
		return *this;
	}

	PriceStream<T> getPriceStream() { return priceStream; }

private:
	PriceStream<T> priceStream;

};
#endif // !ALGOSTREAM_HPP
