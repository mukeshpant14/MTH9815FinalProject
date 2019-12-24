#ifndef PRICEGENERATOR_HPP
#define PRICEGENERATOR_HPP

#include "generator.hpp"
#include "quote.hpp"

using namespace std;

class PriceGenerator : public FileGenerator
{
public:
	PriceGenerator() : FileGenerator("prices.txt") {}

	void generate()
	{
		Quote start(99, 0, 0);
		Quote end(101, 0, 0);
		vector<string> securities;
		securities.push_back("912828YT1");
		securities.push_back("912828TY6");
		securities.push_back("912828YV6");
		securities.push_back("912828YU8");
		securities.push_back("912828YS3");
		securities.push_back("912810SK5");

		this->generatePriceData(securities, start, end, 2, 1000000);
	}
	/**
	* productId id of the product
	* startQuote start price in the range
	* endQuote end price in the range
	* bidAskSpreadIncrementInTick how many ticks by which bid-ask spread is update/moved
	*/
	void generatePriceData(vector<string> productIds, Quote startQuote, Quote endQuote, int bidAskSpreadIncrementInTick,  int size)
	{
		addHeader("productId,bid,ask");

		int noOfTicksInRange = endQuote.noOfTicks() - startQuote.noOfTicks();
		
		for (auto it = productIds.cbegin(); it != productIds.cend(); ++it)
		{
			bool direction = true; // true == increasing, false == descresing
			int tickSize = 1;
			for (int i = 0; i < size; i++) {
				//int tickSize = rand() % noOfTicksInRange;
				Quote bid = startQuote.addTick(tickSize);

				// bid size spread tick
				int bidAskSpreadTick = (rand() % 4 + 1) * bidAskSpreadIncrementInTick;
				Quote ask = bid.addTick(bidAskSpreadTick);

				addRow(*it + "," + bid.toString() + "," + ask.toString());

				if (tickSize == noOfTicksInRange)
					direction = false; 
				else if (tickSize == 1)
					direction = true;

				direction ? tickSize++ : tickSize--;
			};
		};

		std::cout << "Generated price data. File " << this->getFileName() << std::endl;
	}
};

#endif