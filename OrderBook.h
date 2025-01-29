#pragma once
#include "OrderBookEntry.h"
#include "CSVReader.h"
#include <string>
#include <vector>


class OrderBook
{
	public:
		//construct, reading a csv data file
		OrderBook(std::string filename);
		//vector of all main products in the dataset
		std::vector<std::string> getKnownProducts();
		//return vector of orders according to the sent filters
		std::vector<OrderBookEntry> getOrders(OrderBookType type, 
												std::string product,
												std::string timestamp);

		//retruns the earliest time in the orderbook
		std::string getEarliestTime();
		//returns the next time after the sent time in the orderbook
		//if there is no timestamp, wraps around back to the start
		std::string getNextTime(std::string timestamp);

		//function which takes the user's obe from console input and inserts it into the list of order (but not the CSV file)
		void insertOrder(OrderBookEntry& order);

		//matching engine function
		std::vector<OrderBookEntry> matchAsksToBids(std::string product, std::string timestamp);

		static double getHighPrice(std::vector<OrderBookEntry>& orders);
		static double getLowPrice(std::vector<OrderBookEntry>& orders);


	private:
		std::vector<OrderBookEntry> orders;

};

