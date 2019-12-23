//#ifndef ALGOEXECUTIONSERVICELISTENER_HPP
//#define ALGOEXECUTIONSERVICELISTENER_HPP
//
//#include "soa.hpp"
//#include "AlgoExecution.hpp"
//#include "executionservice.hpp"
//#include "algostreamingservice.hpp"
//
//class AlgoExecutionServiceListener : public ServiceListener<AlgoExecution<Bond>>
//{
//public:
//	AlgoExecutionServiceListener(BondExecutionService * _bondExecutionService)
//	{
//		this->bondExecutionService = _bondExecutionService;
//	}
//
//	// Listener callback to process an add event to the Service
//	void ProcessAdd(AlgoExecution<Bond> &data) { this->bondExecutionService->ExecuteOrder(data.getExecutionOrder(), data.getMarket()); };
//
//	// Listener callback to process a remove event to the Service
//	void ProcessRemove(AlgoExecution<Bond> &data) {};
//
//	// Listener callback to process an update event to the Service
//	void ProcessUpdate(AlgoExecution<Bond> &data) {};
//
//private:
//	BondExecutionService* bondExecutionService;
//};
//
//class AlgoStreamingServiceListener : public ServiceListener<AlgoExecution<Bond>>
//{
//public:
//	AlgoStreamingServiceListener(BondAlgoStreamingService * _bondAlgoStreamingService)
//	{
//		this->bondAlgoStreamingService = _bondAlgoStreamingService;
//	}
//
//	// Listener callback to process an add event to the Service
//	void ProcessAdd(AlgoExecution<Bond> &data) 
//	{ 
//		//this->bondAlgoStreamingService->ExecuteOrder(data.getExecutionOrder(), data.getMarket()); 
//	};
//
//	// Listener callback to process a remove event to the Service
//	void ProcessRemove(AlgoExecution<Bond> &data) {};
//
//	// Listener callback to process an update event to the Service
//	void ProcessUpdate(AlgoExecution<Bond> &data) {};
//
//private:
//	BondAlgoStreamingService* bondAlgoStreamingService;
//};
//#endif // !ALGOEXECUTIONSERVICELISTENER_HPP
//
