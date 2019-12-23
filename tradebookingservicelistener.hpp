//#ifndef TRADEBOOKINGSERVICELISTENER_HPP
//#define TRADEBOOKINGSERVICELISTENER_HPP
//
//#include "soa.hpp"
//#include "trade.hpp"
//#include "positionservice.hpp"
//
//class BondTradeBookingServiceListener : public ServiceListener<Trade<Bond>>
//{
//public:
//	BondTradeBookingServiceListener(BondPositionService* _bondPositionService)
//	{
//		this->bondPositionService = _bondPositionService;
//	}
//
//	// Listener callback to process an add event to the Service
//	void ProcessAdd(Trade<Bond> &data) { bondPositionService->AddTrade(data); };
//
//	// Listener callback to process a remove event to the Service
//	void ProcessRemove(Trade<Bond> &data) {};
//
//	// Listener callback to process an update event to the Service
//	void ProcessUpdate(Trade<Bond> &data) {};
//
//private:
//	BondPositionService* bondPositionService;
//};
//
//#endif // !TRADEBOOKINGSERVICELISTENER_HPP
