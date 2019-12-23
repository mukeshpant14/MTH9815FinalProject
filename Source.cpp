#include <iostream>
#include <thread>
#include <boost/thread.hpp>
#include <boost/log/trivial.hpp>

#include "PriceGenerator.hpp"
#include "bondproductgenerator.hpp"
#include "FileReaderProcess.hpp"
#include "pricingservice.hpp"
#include "bondproductservice.hpp"
#include "pricesocketconnector.hpp"
#include "productsocketconnector.hpp"
#include "tradesgenerator.hpp"
#include "tradesocketconnector.hpp"
#include "tradebookingservice.hpp"
#include "tradebookingservicelistener.hpp"
#include "riskservice.hpp"
#include "positionservicelistener.hpp"
#include "marketdatagenerator.hpp"
#include "marketdataservice.hpp"
#include "marketdatasocketconnector.hpp"
#include "AlgoExecution.hpp"
#include "algoexecutionservice.hpp"
#include "executionorder.hpp"
#include "executionservice.hpp"
#include "marketdataservicelistener.hpp"
#include "algoexecutionservicelistener.hpp"
#include "streamingservice.hpp"
#include "algostreamingservice.hpp"
#include "priceprintingprocess.hpp"
#include "inquirydatagenerator.hpp"
#include "inquirysocketconnector.hpp"
#include "inquiryservice.hpp"
#include "guiservice.hpp"

void initializeDataProcess()
{
	FileReaderProcess<string> p1(new PublishSocketConnector<string>(5100));
	FileReaderProcess<string> p2(new PublishSocketConnector<string>(5000));
	FileReaderProcess<string> p3(new PublishSocketConnector<string>(5200));
	FileReaderProcess<string> p4(new PublishSocketConnector<string>(5300));
	FileReaderProcess<string> p5(new PublishSocketConnector<string>(5400));
	
	p1.readData("bonds.txt");
	p2.readData("prices.txt");
	p3.readData("trades.txt");
	p4.readData("marketdata.txt");
	p5.readData("inquiry.txt");
}

void initializeOutputProcess()
{
	// print process
	PricePrintingProcess pricePrintingProcess;
	PriceStreamSubscribeSocketConnector priceStreamConnector(6000, pricePrintingProcess);
	priceStreamConnector.Subscribe();

	// close connector
	priceStreamConnector.close();
}

void initializeServices()
{
	// ------------ Bond Data ------------------
	Service<string, Bond>* bondProductService = BondProductService::Instance();
	// data connector
	BondProductSocketConnector bondDataConnector(5100, bondProductService);

	// ------------ Price Data ---------------- 
	PricingService* pricingService = PricingService::Instance();
	// gui service and listener
	GuiService* guiService = GuiService::Instance();
	GuiServiceListener guiServiceListener(300, guiService);
	pricingService->AddListener(&guiServiceListener);
	// data connector
	PriceDataSocketConnector priceDataConnector(5000, pricingService, bondProductService);
	
	// ------------ Trade Data ----------------
	// bond trade booking service and connector
	BondTradeBookingService* tradeBookingService = BondTradeBookingService::Instance();
	// bond position service and listener
	BondPositionService* bondPositionService = BondPositionService::Instance();
	BondPositionServiceListener bondTradeBookingListener(bondPositionService);
	tradeBookingService->AddListener(&bondTradeBookingListener);
	// bond risk service, and listener
	BondRiskService* bondRiskService = BondRiskService::Instance();
	BondRiskServiceListener bondRiskServiceListener(bondRiskService);
	bondPositionService->AddListener(&bondRiskServiceListener);
	// data connector
	TradeSocketConnector tradeDataConnector(5200, tradeBookingService, BondProductService::Instance());

	// ------------ Market Data ----------------
	// bond market data and connector
	BondMarketDataService* marketDataService = BondMarketDataService::Instance();
	// bond algo execution service, listener and add listener
	BondAlgoExecutionService* bondAlgoExecutionService = BondAlgoExecutionService::Instance();
	BondAlgoExecutionServiceListener bondAlgoExecutionServiceListener(bondAlgoExecutionService);
	marketDataService->AddListener(&bondAlgoExecutionServiceListener);
	// bond execution service, listener and add listener
	BondExecutionService* bondExecutionService = BondExecutionService::Instance();
	BondExecutionServiceListener bondExecutionServiceListener(bondExecutionService);
	bondAlgoExecutionService->AddListener(&bondExecutionServiceListener);
	// algo steaming service, listener and add listener 
	BondAlgoStreamingService* bondAlgoStreamingService = BondAlgoStreamingService::Instance();
	BondAlgoStreamingServiceListener bondAlgoStreamingServiceListener(bondAlgoStreamingService);
	bondAlgoExecutionService->AddListener(&bondAlgoStreamingServiceListener);
	// bond steaming service, listener and add listener
	BondStreamingService* bondStreamingService = BondStreamingService::Instance();
	BondSteamingServiceListener bondStreamingServiceListener(bondStreamingService);
	bondAlgoStreamingService->AddListener(&bondStreamingServiceListener);
	Connector<PriceStream<Bond>>* socketPublisher = new PublishSocketConnector<PriceStream<Bond>>(6000);
	bondStreamingService->addPublishConnector(socketPublisher);	
	// data connector
	MarketDataSocketConnector marketDataConnector(5300, marketDataService, BondProductService::Instance());

	// ------------ Inquiry Data ----------------
	// bond trade booking service and connector
	BondInquiryService* bondInquiryService = BondInquiryService::Instance();
	InquirySocketConnector inquiryDataConnector(5400, bondInquiryService, BondProductService::Instance());
	InquiryConnector inquiryConnector(bondInquiryService);
	BondInquiryServiceListener listener(&inquiryConnector);
	bondInquiryService->AddListener(&listener);

	// invoke subscribe on all connectors 
	bondDataConnector.Subscribe();
	priceDataConnector.Subscribe();
	tradeDataConnector.Subscribe();
	marketDataConnector.Subscribe();
	inquiryDataConnector.Subscribe();

	// close all connectors
	bondDataConnector.close();
	priceDataConnector.close();
	tradeDataConnector.close();
	marketDataConnector.close();
	socketPublisher->close();
	inquiryDataConnector.close();
}

void generateData()
{
	BondGenerator bondGenerator;
	bondGenerator.generate();

	PriceGenerator generator;
	generator.generate();

	TradeGenerator tradeGenerator;
	tradeGenerator.generate();

	MarketDataGenerator marketDataGenerator;
	marketDataGenerator.generate();

	InquiryDataGenerator inquiryDataGenerator;
	inquiryDataGenerator.generate();

	BOOST_LOG_TRIVIAL(info) << "data generation completed";
}

void tradingSystem()
{
	BOOST_LOG_TRIVIAL(info) << "............ trading system is starting ...........";

	generateData();

	boost::thread_group tg;
	tg.create_thread(initializeOutputProcess);
	tg.create_thread(initializeServices);
	tg.create_thread(initializeDataProcess);
	tg.join_all();

	BOOST_LOG_TRIVIAL(info) << "............ trading system is shutting down ...........";
}

int main()
{
	tradingSystem();
	
	std::cout << "\n----------- Press Any key to quit! -------------\n" << std::endl;
	std::cin.get();
	return 0;
}