#include "OrderBook.h"
#include "CSVReader.h"
#include <map>
#include <algorithm>
#include <iostream>


OrderBook::OrderBook(std::string filename)
{
	orders = CSVReader::readCSV(filename);
}

std::vector<std::string> OrderBook::getKnownProducts()
{
	std::vector<std::string> products;
	std::map<std::string, bool> prodMap;

	for (OrderBookEntry& e : orders)
	{
		prodMap[e.product] = true;
	}

	for (auto const& e : prodMap)
	{
		products.push_back(e.first);
	}

	return products;
}

std::vector<OrderBookEntry> OrderBook::getOrders(OrderBookType type,
													std::string product,
													std::string timestamp)
{
	std::vector<OrderBookEntry> orders_sub;
	for (OrderBookEntry& e : orders)
	{
		if (e.orderType == type && e.product == product && e.timestamp == timestamp)
		{
			orders_sub.push_back(e);
		}
	}
	return orders_sub;
}

double OrderBook::getHighPrice(std::vector<OrderBookEntry>& orders)
{
	double max = orders[0].price;
	for (OrderBookEntry& e : orders)
	{
		if (e.price > max)
		{
			max = e.price;
		}
	}

	return max;
}

double OrderBook::getLowPrice(std::vector<OrderBookEntry>& orders)
{
	double min = orders[0].price;
	for (OrderBookEntry& e : orders)
	{
		if (e.price < min)
		{
			min = e.price;
		}
	}

	return min;
}

std::string OrderBook::getEarliestTime()
{
	return orders[0].timestamp;
}

std::string OrderBook::getNextTime(std::string timestamp)
{
	std::string next_timestamp = "";
	for (OrderBookEntry& e : orders)
	{
		if (e.timestamp > timestamp)
		{
			next_timestamp = e.timestamp;
			break;
		}
	}
	if (next_timestamp == "")
	{
		next_timestamp = orders[0].timestamp;
	}

	return next_timestamp;
}

void OrderBook::insertOrder(OrderBookEntry& order)
{
	orders.push_back(order);

	//built in sorting algorithm which will place the inserted order into the correct position
	std::sort(orders.begin(), orders.end(), OrderBookEntry::compareByTime);
}

//mathcing engine implimentation
std::vector<OrderBookEntry> OrderBook::matchAsksToBids(std::string product, std::string timestamp)
{
	std::vector<OrderBookEntry> asks = getOrders(OrderBookType::ask,
													product,
													timestamp);
	std::vector<OrderBookEntry> bids = getOrders(OrderBookType::bid,
													product,
													timestamp);

	std::vector<OrderBookEntry> sales;

	// I put in a little check to ensure we have bids and asks
	// to process.
	if (asks.size() == 0 || bids.size() == 0)
	{
		std::cout << " OrderBook::matchAsksToBids no bids or asks" << std::endl;
		return sales;
	}

	std::sort(asks.begin(), asks.end(), OrderBookEntry::compareByPriceAsc);
	std::sort(bids.begin(), bids.end(), OrderBookEntry::compareByPriceDesc);
	// for ask in asks:
	std::cout << "max ask " << asks[asks.size() - 1].price << std::endl;
	std::cout << "min ask " << asks[0].price << std::endl;
	std::cout << "max bid " << bids[0].price << std::endl;
	std::cout << "min bid " << bids[bids.size() - 1].price << std::endl;

	for (OrderBookEntry& ask : asks)
	{
		for (OrderBookEntry& bid : bids)
		{
			if (bid.price >= ask.price)
			{
				//checks if the user is interacting with the sim so that their wallet can be updated correctly
				OrderBookEntry sale{ ask.price, 0, timestamp, product, OrderBookType::asksale };
				if (bid.username == "simUser")
				{
					sale.username = "simUser";
					sale.orderType = OrderBookType::bidsale;
				}
				if (ask.username == "simUser")
				{
					sale.username = "simUser";
					sale.orderType = OrderBookType::asksale;
				}

				//sale matching conditions between bids and asks
				if (bid.amount == ask.amount)
				{
					sale.amount = ask.amount;
					sales.push_back(sale);

					bid.amount = 0;
					break;
				}//end condition 1

				if (bid.amount > ask.amount)
				{
					sale.amount = ask.amount;
					sales.push_back(sale);

					bid.amount = bid.amount - ask.amount;
					break;
				}//end condition 2

				if (bid.amount < ask.amount && bid.amount > 0)
				{
					sale.amount = bid.amount;
					sales.push_back(sale);

					ask.amount = ask.amount - bid.amount;
					bid.amount = 0;
					continue;
				}//end condition 3
			}
		}//end nested loop
	}//end outer loop

	return sales;

}

