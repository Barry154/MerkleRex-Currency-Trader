#pragma once
#include "OrderBookEntry.h"
#include "OrderBook.h"
#include "Wallet.h"
#include <vector>

class MerkelMain
{
	public:
		MerkelMain();
        //Call this to start the sim
        void init();

    private:
        //Prints the menu of avilable inputs and functions
        void printMenu();
        void printHelp();
        void printMarketStats();
        void makeOffer();
        void makeBid();
        void printWallet();
        void nextTimeFrame();
        //Stores the user input for processing
        int getUserOption();
        //Checks the user input to determine correct output
        void processUserOption(int userOption);
        
        std::string currentTime;

        OrderBook orderBook{ "20200317.csv" };

        Wallet wallet;
};

