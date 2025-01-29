#pragma once
#include "OrderBookEntry.h"
#include <vector>
#include <string>

class CSVReader
{
	public:
		CSVReader();
		static std::vector<OrderBookEntry> readCSV(std::string csvFile);
		static std::vector<std::string> tokenise(std::string csvLine, char seperator);

		//creates orderbook entries from user input
		static OrderBookEntry stringsToOBE(	std::string price, 
											std::string amount,
											std::string timestamp, 
											std::string product,
											OrderBookType OrderBookType );

	private:
		//creates orderbook entries from the external CSV file
		static OrderBookEntry stringsToOBE(std::vector<std::string> strings);
};

