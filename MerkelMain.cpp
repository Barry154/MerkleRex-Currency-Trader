#include "MerkelMain.h"
#include "CSVReader.h"
#include <iostream>

MerkelMain::MerkelMain()
{

}

void MerkelMain::init()
{
    int input;
    currentTime = orderBook.getEarliestTime();

    wallet.insertCurrency("BTC", 10);

    while (true)
    {
        printMenu();
        input = getUserOption();
        processUserOption(input);
        std::cout << "" << std::endl;
    }
}

//Prints the menu of avilable inputs and functions
void MerkelMain::printMenu()
{
    // 1. print help
    std::cout << "1: Print Help" << std::endl;

    // 2. print exchange stats
    std::cout << "2: Print Exchange Stats" << std::endl;

    // 3. make an offer
    std::cout << "3: Make an Offer" << std::endl;

    // 4. make a bid
    std::cout << "4: Make a Bid" << std::endl;

    // 5. print wallet
    std::cout << "5: Print Wallet" << std::endl;

    // 6. next time-step (continue)
    std::cout << "6: Continue" << std::endl;

    std::cout << "=================================" << std::endl;

    std::cout << "Current time is: " << currentTime << std::endl;
}

//Different menu options
void MerkelMain::printHelp()
{
    std::cout << "Help - Your aim is to make money. Analyse the market and make bids and offers." << std::endl;
}

void MerkelMain::printMarketStats()
{
    for (std::string const& p : orderBook.getKnownProducts())
    {
        std::cout << "Product: " << p << std::endl;
        std::vector<OrderBookEntry> entries = orderBook.getOrders(OrderBookType::ask, p, currentTime);
        std::cout << "Asks for product: " << entries.size() << std::endl;
        std::cout << "Max ask: " << OrderBook::getHighPrice(entries) << std::endl;
        std::cout << "Min ask: " << OrderBook::getLowPrice(entries) << std::endl;
        std::cout << "" << std::endl;
    }
}

void MerkelMain::makeOffer()
{
    std::cout << "Make an Offer - enter the amount: product,price,amount (eg. ETH/BTC,200,0.5)" << std::endl;
    
    //user input
    std::string input;

    //tells the console to clear the input buffer from the previous user input (selecting to make an offer), which is storing an end of line character
    //not needed anymore because getUserOption() now uses getline instead of std::cin to retrieve the menu option from the user
    //std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    
    //retrieves user input
    std::getline(std::cin, input); 

    //tokenise user input
    std::vector<std::string> tokens = CSVReader::tokenise(input, ',');
    if (tokens.size() != 3)
    {
        std::cout << "MerkelMain::makeOffer - Incorrect input!" << input << std::endl;
    }
    else
    {
        try
        {
            //create an orderbook entry from user input
            OrderBookEntry obe = CSVReader::stringsToOBE(tokens[1], tokens[2], currentTime, tokens[0], OrderBookType::ask);
            //set username field to the user (default = dataset)
            obe.username = "simUser";
            //checks if wallet has sufficient funds
            if (wallet.canFulfillOrder(obe))
            {
                std::cout << "Wallet looks good!" << std::endl;
                //inserts user's order into the CSV file
                orderBook.insertOrder(obe);
            }
            else std::cout << "Insufficient funds!" << std::endl; 
        }
        catch(const std::exception& e)
        {
            //catch the thrown exception from CSVReader::stringsToOBE
            std::cout << "MerkelMain::makeOffer Bad input" << std::endl;
        }
    }
}

void MerkelMain::makeBid()
{
    std::cout << "Make a Bid - enter the amount: product,price,amount (eg. ETH/BTC,200,0.5)" << std::endl;

    //user input
    std::string input;

    //tells the console to clear the input buffer from the previous user input (selecting to make an offer), which is storing an end of line character
    //not needed anymore because getUserOption() now uses getline instead of std::cin to retrieve the menu option from the user
    //std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    //retrieves user input
    std::getline(std::cin, input);

    //tokenise user input
    std::vector<std::string> tokens = CSVReader::tokenise(input, ',');
    if (tokens.size() != 3)
    {
        std::cout << "MerkelMain::makeBid - Incorrect input!" << std::endl;
    }
    else
    {
        try
        {
            //create an orderbook entry from user input
            OrderBookEntry obe = CSVReader::stringsToOBE(tokens[1], tokens[2], currentTime, tokens[0], OrderBookType::bid);
            //set username field to the user (default = dataset)
            obe.username = "simUser";
            //checks if wallet has sufficient funds
            if (wallet.canFulfillOrder(obe))
            {
                std::cout << "Wallet looks good!" << std::endl;
                //inserts user's order into the orderbook
                orderBook.insertOrder(obe);
            }
            else std::cout << "Insufficient funds!" << std::endl;
        }
        catch (const std::exception& e)
        {
            //catch the thrown exception from CSVReader::stringsToOBE
            std::cout << "MerkelMain::makeBid - Bad input" << std::endl;
        }
    }
}

void MerkelMain::printWallet()
{
    //std::cout << "Your wallet is empty." << std::endl;
    std::cout << wallet.toString() << std::endl;
}

void MerkelMain::nextTimeFrame()
{
    std::cout << "Going to next time-frame" << std::endl;

    for (std::string p : orderBook.getKnownProducts())
    {
        std::cout << "Matching " << p << std::endl;
        std::vector<OrderBookEntry> sales = orderBook.matchAsksToBids(p, currentTime);
        std::cout << "Sales: " << sales.size() << std::endl;

        for (OrderBookEntry& sale : sales)
        {
            std::cout << "Sale price: " << sale.price << " amount " << sale.amount << std::endl;
            if (sale.username == "simUser")
            {
                //update wallet
                wallet.processSale(sale);
            }
        }
    }

    currentTime = orderBook.getNextTime(currentTime);
}

//Stores the user input for processing
int MerkelMain::getUserOption()
{
    int userOption = 0;
    std::string line;

    std::cout << "Select an Option (1-6)" << std::endl;
    std::getline(std::cin, line);

    try
    {
        userOption = std::stoi(line);
    }
    catch (const std::exception& e)
    {
        //can leave an empty block since the 'crash' will be caught, but the userOption will remain 0
        //processUserOption() will then deal with the incorrect input of 0
    }
    std::cout << "You chose: " << userOption << std::endl;

    return userOption;
}

//Checks the user input to determine correct output
void MerkelMain::processUserOption(int userOption)
{
    //input checking
    if (userOption == 0) {
        std::cout << "Invalid choice. Please select a number from 1-6." << std::endl;
    }
    if (userOption == 1) {
        printHelp();
    }
    if (userOption == 2) {
        printMarketStats();
    }
    if (userOption == 3) {
        makeOffer();
    }
    if (userOption == 4) {
        makeBid();
    }
    if (userOption == 5) {
        printWallet();
    }
    if (userOption == 6) {
        nextTimeFrame();
    }
}

