#include "CSVReader.h"
#include <iostream>
#include <fstream>

CSVReader::CSVReader()
{

}

std::vector<OrderBookEntry> CSVReader::readCSV(std::string csvFileName)
{
	std::vector<OrderBookEntry> entries;
    //Open and read data from an external file
    //Declare file as a variable and present filename to open it
    std::ifstream csvFile{csvFileName};
    std::string line; //Stores the current line in the file when iterating

    if (csvFile.is_open())
    {
        while (std::getline(csvFile, line))
        {
            try
            {
                OrderBookEntry obe = stringsToOBE(tokenise(line, ','));
                entries.push_back(obe);
            }catch(const std::exception& e)
            {
                std::cout << "CSVReader::readCSV Bad Data" << entries.size() << std::endl;
                std::cout << "" << std::endl;
                //std::cout << "" << std::endl;
            }
        }
    }

    std::cout << "Entries read: " << entries.size() << std::endl;
 	return entries;
}

std::vector<std::string> CSVReader::tokenise(std::string csvLine, char seperator)
{
    //Stores the tokens
    std::vector<std::string> tokens;

    //Declare variables
    std::string token;
    signed int start, end;
    //Initialise start to be the position of the first character that is not the seperator
    start = csvLine.find_first_not_of(seperator, 0);

    do
    {
        //Initilaise end to be the first instance of the seperator character after the start point
        end = csvLine.find_first_of(seperator, start);

        //Break out of the loop if the start position reaches the end of the CSV line
        if (start == csvLine.length() || start == end)
        {
            break;
        }

        //Seperate the tokens from the CSV line
        if (end >= 0)
        {
            token = csvLine.substr(start, end - start);
        }
        else
        {
            token = csvLine.substr(start, csvLine.length() - start);
        }

        //Store the tokens in the vector and reset to new start value
        tokens.push_back(token);
        start = end + 1;

    } while (end > 0);

    //Retuns the vector storing the tokens
    return tokens;
}

OrderBookEntry CSVReader::stringsToOBE(std::vector<std::string> tokens)
{
    double price, amount;

    //Check if there are 5 tokens stored in the vector
    if (tokens.size() != 5)
    {
        std::cout << "CSVReader::stringsToOBE Bad line data" << std::endl;
        throw std::exception{};

        std::cout << "" << std::endl;
    }

    //If we have 5 tokens - convert tokens to required data-type
    //Exception handling - if trying to convert the data-type fails, catch the error and continue
    try
    {
        price = std::stod(tokens[3]);
        amount = std::stod(tokens[4]);
    }
    catch (const std::exception& e) {
        std::cout << "CSVReader::stringsToOBE Bad float! " << tokens[3] << std::endl;
        std::cout << "CSVReader::stringsToOBE Bad float! " << tokens[4] << std::endl;
        throw;

        std::cout << "" << std::endl;
    }


	OrderBookEntry obe{ price,
						amount,
						tokens[0],
						tokens[1],
						OrderBookEntry::stringToOBT(tokens[2])};

	return obe;
}

OrderBookEntry CSVReader::stringsToOBE( std::string priceString,
                                        std::string amountString,
                                        std::string timestamp,
                                        std::string product,
                                        OrderBookType orderType)
{
    double price, amount;

    try
    {
        price = std::stod(priceString);
        amount = std::stod(amountString);
    }
    catch (const std::exception& e) {
        std::cout << "CSVReader::stringsToOBE Bad float! " << priceString << std::endl;
        std::cout << "CSVReader::stringsToOBE Bad float! " << amountString << std::endl;
        throw;

        std::cout << "" << std::endl;
    }

    OrderBookEntry obe{ price,
                        amount,
                        timestamp,
                        product,
                        orderType};

    return obe;
}