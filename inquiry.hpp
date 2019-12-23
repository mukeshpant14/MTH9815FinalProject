#ifndef INQUIRY_HPP
#define INQUIRY_HPP

#include <string>
#include "trade.hpp"

using namespace std;

// Various inqyury states
enum InquiryState { RECEIVED, QUOTED, DONE, REJECTED, CUSTOMER_REJECTED };

/**
* Inquiry object modeling a customer inquiry from a client.
* Type T is the product type.
*/
template<typename T>
class Inquiry
{
public:
	// ctor for an inquiry
	Inquiry(string _inquiryId, const T &_product, Side _side, long _quantity, double _price, InquiryState _state)
		: product(_product)
	{
		inquiryId = _inquiryId;
		side = _side;
		quantity = _quantity;
		price = _price;
		state = _state;
	}

	Inquiry() : product(T()) {}

	// assignment
	const Inquiry<T>& operator=(const Inquiry<T>& other)
	{
		this->inquiryId = other.inquiryId;
		this->product = other.product;
		this->side = other.side;
		this->quantity = other.quantity;
		this->price = other.price;
		this->state = other.state;
		return *this;
	}

	// Get the inquiry ID
	const string& GetInquiryId() const { return this->inquiryId; }

	// Get the product
	const T& GetProduct() const { return this->product; }

	// Get the side on the inquiry
	Side GetSide() const { return this->side; }

	// Get the quantity that the client is inquiring for
	long GetQuantity() const { return this->quantity; }

	// Get the price that we have responded back with
	double GetPrice() const { return this->price; }
	void setPrice(double _price) { this->price = _price; }

	// Get the current state on the inquiry
	InquiryState GetState() const { return this->state; }
	void setState(InquiryState _state) { this->state = _state; }

	static InquiryState getStateEnum(string stateStr)
	{
		//RECEIVED, QUOTED, DONE, REJECTED, CUSTOMER_REJECTED
		if (stateStr == "RECEIVED")
			return InquiryState::RECEIVED;
		else if (stateStr == "QUOTED")
			return InquiryState::QUOTED;
		else if (stateStr == "DONE")
			return InquiryState::DONE;
		else if (stateStr == "REJECTED")
			return InquiryState::RECEIVED;
		else if (stateStr == "CUSTOMER_REJECTED")
			return InquiryState::CUSTOMER_REJECTED;
		else
			throw "incorrect state: " + stateStr;
	}

private:
	string inquiryId;
	T product;
	Side side;
	long quantity;
	double price;
	InquiryState state;

};


#endif // !INQUIRY_HPP
