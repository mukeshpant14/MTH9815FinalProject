#ifndef MARKETDATAGENERATOR_HPP
#define MARKETDATAGENERATOR_HPP

#include <boost/log/trivial.hpp>
#include "generator.hpp"
#include "quote.hpp"

class MarketDataGenerator : public FileGenerator
{
public:
	MarketDataGenerator() : FileGenerator("marketdata.txt") {}

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

		this->generateMarketData(securities, start, end, 2, 1000);
	}

	void generateMarketData(vector<string> productIds, Quote startQuote, Quote endQuote, int bidAskSpreadIncrementInTick, int size)
	{
		addHeader("productId,bid,ask,quantity");

		vector<long> quantities;
		for (int i = 0; i < 5; i++)
			quantities.push_back((i + 1) * 10);
		
		int noOfTicksInRange = endQuote.noOfTicks() - startQuote.noOfTicks();
		Quote q = startQuote;
		for (auto it = productIds.cbegin(); it != productIds.cend(); ++it)
		{
			for (auto qit = quantities.cbegin(); qit != quantities.cend(); ++qit)
			{
				for (int i = 0; i < size/5; i++) {
					int tickSize = rand() % noOfTicksInRange;
					Quote bid = startQuote.addTick(tickSize);

					// bid size spread tick
					int bidAskSpreadTick = (rand() % 4 + 1) * bidAskSpreadIncrementInTick;
					Quote ask = bid.addTick(bidAskSpreadTick);


					addRow(*it + "," + bid.toString() + "," + ask.toString() + "," + std::to_string(*qit));
				};
			};
		};

		BOOST_LOG_TRIVIAL(info) << "Generated market data. File " << this->getFileName();
	}
};

#endif // !MARKETDATAGENERATOR_HPP
