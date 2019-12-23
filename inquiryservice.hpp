#ifndef INQUIRY_SERVICE_HPP
#define INQUIRY_SERVICE_HPP

#include "soa.hpp"
#include "tradebookingservice.hpp"
#include "inquiry.hpp"
#include "SocketConnector.hpp"

/**
* Service for customer inquirry objects.
* Keyed on inquiry identifier (NOTE: this is NOT a product identifier since each inquiry must be unique).
* Type T is the product type.
*/
template<typename T>
class InquiryService : public BaseService<Inquiry <T> >
{

public:
	// Get data on our service given a key
	Inquiry<T>& GetData(string key) { return inquiryMap[key]; };

	// Send a quote back to the client
	void SendQuote(const string &inquiryId, double price) {};

	// Reject an inquiry from the client
	void RejectInquiry(const string &inquiryId) {};

protected:
	map<string, Inquiry<T>> inquiryMap;
};

class BondInquiryService : public InquiryService<Bond>
{
public:
	// The callback that a Connector should invoke for any new or updated data
	void OnMessage(Inquiry<Bond> &data)
	{
		// check if inquiry id exists in map
		bool isUpdate = false;
		if (inquiryMap.find(data.GetInquiryId()) != inquiryMap.end())
		{
			isUpdate = true;
			data.setState(InquiryState::DONE);
		};

		inquiryMap.emplace(data.GetInquiryId(), data);
		isUpdate ? this->callListeners(data, Action::UPDATE) : this->callListeners(data, Action::ADD);
	};

	static BondInquiryService* Instance()
	{
		if (!m_pInstance)   // Only allow one instance of class to be generated.
			m_pInstance = new BondInquiryService();
		return m_pInstance;
	}

private:
	static BondInquiryService* m_pInstance;
};

BondInquiryService* BondInquiryService::m_pInstance = NULL;

/*
---------- Connectors ---------------
*/
class InquiryConnector : public Connector<Inquiry<Bond>>
{
public:
	InquiryConnector(BondInquiryService* _service) : service(_service) {}

	// Publish data to the Connector
	void Publish(Inquiry<Bond> &data)
	{
		if (data.GetState() == InquiryState::RECEIVED)
		{
			data.setState(InquiryState::QUOTED);
			service->OnMessage(data);
		}
		else if (data.GetState() == InquiryState::DONE)
		{
			BOOST_LOG_TRIVIAL(info) << "Inquiry " << data.GetInquiryId() << " is DONE";
		}
	};

	void Subscribe() {};

	void close() {};

private:
	BondInquiryService* service;
};

/*
---------- Listeners ---------------
*/
class BondInquiryServiceListener : public ServiceListener<Inquiry<Bond>>
{
public:
	BondInquiryServiceListener(InquiryConnector* _publishConnector) : publishConnector(_publishConnector) {}

	// Listener callback to process an add event to the Service
	void ProcessAdd(Inquiry<Bond> &data) 
	{ 
		if (data.GetState() == InquiryState::RECEIVED)
		{
			data.setPrice(100.0); // set quote of 100.0
			publishConnector->Publish(data);
		};
	};

	// Listener callback to process a remove event to the Service
	void ProcessRemove(Inquiry<Bond> &data) {};

	// Listener callback to process an update event to the Service
	void ProcessUpdate(Inquiry<Bond> &data) 
	{
		if (data.GetState() == InquiryState::DONE)
			publishConnector->Publish(data);
	};

private:
	InquiryConnector* publishConnector;
};

#endif
