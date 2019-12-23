#ifndef TRADESGENERATOR_HPP
#define TRADESGENERATOR_HPP

#include "generator.hpp"
#include "quote.hpp"

class TradeGenerator : public FileGenerator
{
public:
	TradeGenerator() : FileGenerator("trades.txt") {}

	void generate()
	{
		vector<string> securities;
		securities.push_back("912828YT1");
		securities.push_back("912828TY6");
		securities.push_back("912828YV6");
		securities.push_back("912828YU8");
		securities.push_back("912828YS3");
		securities.push_back("912810SK5");

		this->generateTrades(securities, 10);
	}

	/**
	* productId id of the product
	* startQuote start price in the range
	* endQuote end price in the range
	* bidAskSpreadIncrementInTick how many ticks by which bid-ask spread is update/moved
	*/
	void generateTrades(vector<string> productIds, int size)
	{
		addHeader("productId,tradeId,price,book,quantity,side");

		Quote buyQuote(99, 0, 0);
		Quote sellQuote(101, 0, 0);

		vector<string> books{ "TRSY1", "TRSY2", "TRSY3" };

		int index = 0;
		for (auto it = productIds.cbegin(); it != productIds.cend(); ++it)
		{
			for (int i = 0; i < size; i++) {
				string side = 0 == (i % 2) ? "BUY" : "SELL";
				int quantity = 1000000 * ((i % 5) + 1);
				Quote price = 0 == (i % 2) ? buyQuote : sellQuote;
				string book = books[(i % 3)];

				string tradeId = "T" + std::to_string((++index));

				addRow(*it + "," + tradeId + "," + price.toString() + "," + book + "," + std::to_string(quantity) + "," + side);
			}
		};

		std::cout << "Generated trade data. File " << this->getFileName() << std::endl;
	}
};

#endif // TRADESGENERATOR_HPP