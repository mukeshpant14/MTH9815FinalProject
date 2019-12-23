#ifndef INQUIRYDATAGENERATOR_HPP
#define INQUIRYDATAGENERATOR_HPP

#include <vector>
#include <string>
#include "generator.hpp"
#include "quote.hpp"

class InquiryDataGenerator : public FileGenerator
{
public:
	InquiryDataGenerator() : FileGenerator("inquiry.txt") {}

	void generate()
	{
		vector<string> securities;
		securities.push_back("912828YT1");
		securities.push_back("912828TY6");
		securities.push_back("912828YV6");
		securities.push_back("912828YU8");
		securities.push_back("912828YS3");
		securities.push_back("912810SK5");

		this->generateInquiry(securities, 10);
	}

	void generateInquiry(vector<string> productIds, int size)
	{
		addHeader("inquiryId,productId,side,quantity,price,state");

		Quote buyQuote(99, 0, 0);
		Quote sellQuote(100, 0, 0);

		int index = 0;
		for (auto it = productIds.cbegin(); it != productIds.cend(); ++it)
		{
			for (int i = 0; i < size; i++) {
				string side = 0 == (i % 2) ? "BUY" : "SELL";
				int quantity = 1000000 * ((i % 5) + 1);
				Quote price = 0 == (i % 2) ? buyQuote.addTick(i*2) : sellQuote.addTick(i*2);
				
				string inquiryId = "Inquiry_" + (*it) + "_" + std::to_string((++index));
				string state = "RECEIVED";
				addRow(inquiryId + "," + (*it) + "," + side + "," + std::to_string(quantity) + "," + price.toString() + "," + state);
			}
		};

		std::cout << "Generated trade data. File " << this->getFileName() << std::endl;
	}
};

#endif // !INQUIRYDATAGENERATOR_HPP
