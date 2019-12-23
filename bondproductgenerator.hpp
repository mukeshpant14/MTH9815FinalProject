#ifndef BONDPRODUCTGENERATOR_HPP
#define BONDPRODUCTGENERATOR_HPP

#include "generator.hpp"

using namespace std;

class BondGenerator : public FileGenerator
{
public:
	BondGenerator() : FileGenerator("bonds.txt") {}

	void generate()
	{
		addHeader("CUSIP,coupon,issueDate,maturityDate,ticker,tenor");
		addRow("912828YT1,1.500,20190212,20211130,T,2");
		addRow("912828TY6,1.625,20190511,20221115,T,3"); 
		addRow("912828YV6,1.500,20190212,20241130,T,5"); 
		addRow("912828YU8,1.625,20190212,20261130,T,7"); 
		addRow("912828YS3,1.750,20191115,20291115,T,10"); 
		addRow("912810SK5,2.375,20191115,20491115,T,30"); 

		std::cout << "Generated bond data. File " << this->getFileName() << std::endl;
	}
};

#endif