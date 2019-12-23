//#ifndef POSITIONSERVICELISTENER_HPP
//#define POSITIONSERVICELISTENER_HPP
//
//#include "soa.hpp"
//#include "trade.hpp"
//#include "riskservice.hpp"
//
//class BondPositionServiceListener : public ServiceListener<Position<Bond>>
//{
//public:
//	BondPositionServiceListener(BondRiskService* _bondRiskService)
//	{
//		this->bondRiskService = _bondRiskService;
//	}
//
//	// Listener callback to process an add event to the Service
//	void ProcessAdd(Position<Bond> &data) { bondRiskService->AddPosition(data); };
//
//	// Listener callback to process a remove event to the Service
//	void ProcessRemove(Position<Bond> &data) {};
//
//	// Listener callback to process an update event to the Service
//	void ProcessUpdate(Position<Bond> &data) {};
//
//private:
//	BondRiskService* bondRiskService;
//};
//
//#endif // !TRADEBOOKINGSERVICELISTENER_HPP
