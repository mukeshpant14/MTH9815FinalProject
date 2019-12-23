//#ifndef MARKETDATASERVICELISTENER_HPP
//#define MARKETDATASERVICELISTENER_HPP
//
//#include "soa.hpp"
//#include "marketdataservice.hpp"
//#include "algoexecutionservice.hpp"
//
//class MarketDataServiceListener : public ServiceListener<OrderBook<Bond>>
//{
//public:
//	MarketDataServiceListener(BondAlgoExecutionService * _bondAlgoExecutionService)
//	{
//		this->bondAlgoExecutionService = _bondAlgoExecutionService;
//	}
//
//	// Listener callback to process an add event to the Service
//	void ProcessAdd(OrderBook<Bond> &data) { bondAlgoExecutionService->ExecuteAlgo(data); };
//
//	// Listener callback to process a remove event to the Service
//	void ProcessRemove(OrderBook<Bond> &data) {};
//
//	// Listener callback to process an update event to the Service
//	void ProcessUpdate(OrderBook<Bond> &data) {};
//
//private:
//	BondAlgoExecutionService* bondAlgoExecutionService;
//};
//#endif // !MARKETDATASERVICELISTENER_HPP
