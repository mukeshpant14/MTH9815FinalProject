/**
* tradebookingservice.hpp
* Defines the data types and Service for trade booking.
*
* @author Breman Thuraisingham
*/
#ifndef TRADE_BOOKING_SERVICE_HPP
#define TRADE_BOOKING_SERVICE_HPP

#include <string>
#include <vector>
#include "soa.hpp"
#include "trade.hpp"
#include "products.hpp"
#include "baseservice.hpp"

/**
* Trade Booking Service to book trades to a particular book.
* Keyed on trade id.
* Type T is the product type.
*/
template<typename T>
class TradeBookingService : public BaseService<Trade <T> >
{

public:
	TradeBookingService()
	{
		tradeMap = map<string, Trade<T>>();
	}

	// Get data on our service given a key
	Trade<T>& GetData(string key)
	{
		return tradeMap[key];
	}

	// The callback that a Connector should invoke for any new or updated data
	void OnMessage(Trade<T>& data)
	{
		this->BookTrade(data);

		this->callListeners(data, Action::ADD);
	}

	// Book the trade
	void BookTrade(const Trade<T> &trade)
	{
		tradeMap.emplace(trade.GetTradeId(), trade);
		this->printMessage("Booked trade in TradeBookingService --> " + trade.GetTradeId());
	}

private:
	map<string, Trade<T>> tradeMap;
};

class BondTradeBookingService : public TradeBookingService<Bond> 
{
public:
	static BondTradeBookingService* Instance()
	{
		if (!m_pInstance)   // Only allow one instance of class to be generated.
			m_pInstance = new BondTradeBookingService();
		return m_pInstance;
	}

private:
	static BondTradeBookingService* m_pInstance;
};

BondTradeBookingService* BondTradeBookingService::m_pInstance = NULL;

#endif
