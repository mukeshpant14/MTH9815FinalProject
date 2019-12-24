#ifndef QUOTE_HPP
#define QUOTE_HPP

#include <iostream>
#include <string>
#include <sstream>

using namespace std;

class Quote
{
public:
	Quote(int _base, int _first, int _second)
	{
		base = _base;
		if (_first > 31 || _second > 7) throw "unexpected quote";

		first = _first;
		second = _second;
	}
	Quote() {}

	// assignment
	const Quote& operator=(const Quote& other)
	{
		this->base = other.base;
		this->first = other.first;
		this->second = other.second;
		return *this;
	}

	bool operator==(const Quote &rhs) {
		return (this->base == rhs.base && this->first == rhs.first && this->second == rhs.second);
	}

	bool operator!=(const Quote &rhs) {
		return this->base != rhs.base || this->first != rhs.first || this->second != rhs.second;
	}

	string toString()
	{
		return to_string(base) + "-" +
			(first < 10 ? "0" + to_string(first) : to_string(first)) +
			(second == 4 ? "+" : to_string(second));
	}

	int noOfTicks()
	{
		return base * 256 + first * 8 + second;
	}

	static Quote fromString(string s)
	{
		int i = s.find('-');
		int base = stoi(s.substr(0, i));
		int first = stoi(s.substr(i + 1, 2));
		int second = s.back() == '+' ? 4 : (s.back() - 48);
		return Quote(base, first, second);
	}

	Quote addTick(int noOfTicks)
	{
		int second = (this->second + noOfTicks) % 8;
		int first = this->first + (int)(this->second + noOfTicks) / 8;
		int base = this->base;

		if (first > 31)
		{
			int temp = first;
			first = first % 32;
			base += (int)temp / 32;
		};

		return Quote(base, first, second);
	}

	double getPrice()
	{
		return base + ((first * 8.0) + second) / 256.0;
	}

	int base;
	int first;
	int second;
};

#endif