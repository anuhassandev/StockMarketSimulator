// StockMarketSimulator.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <iomanip>
#include "Order.h"
#include "Match.h"
#include "Display.h"

int main(int argc, char* argv[])
{
    // Ensure command-line argument for input file
    if (argc < 2)
    {
        std::cerr << "Error: Missing input file argument.\n";
        return 1;
    }

    std::ifstream inputFile(argv[1]);
    if (!inputFile)
    {
        std::cerr << "Error: Unable to open file: " << argv[1] << "\n";
        return 1;
    }

    std::string lineOfText;
    float prevTransactionPrice;
    std::vector<Order> pendingOrders;   // Holds all orders not yet executed
    std::vector<Order> previousOrders;  // Keeps track of the previous state of orders for incremental updates
    std::vector<Trade> executedTrades;


    // Step 1: Read the first line for the previous transaction price
    if (getline(inputFile, lineOfText))
    {
        std::istringstream iss(lineOfText);
        iss >> prevTransactionPrice;
    }

    // Step 2: Process each order line in the file
    while (getline(inputFile, lineOfText))
    {
        std::istringstream iss(lineOfText);
        std::string orderID;
        char orderType;  // B or S
        int quantity;
        float limitPrice = 0.0f;
        bool isMarketOrder = false;

        // Parse order fields
        if (iss >> orderID >> orderType >> quantity)
        {
            if (!(iss >> limitPrice))
                isMarketOrder = true; // It's a market order if no limit price exists
        }

        // Step 3: Create a new order
        Order newOrder(orderID, orderType, quantity, limitPrice, isMarketOrder, std::chrono::system_clock::now());

        // Step 4: Try to match the new order with existing pending orders
        auto matchResult = matchOrder(newOrder, pendingOrders, prevTransactionPrice);

        // Step 5: Handle trade execution and update of lists

        while (matchResult.has_value()) // If we have found a match
        {
            auto [matchedOrder, quantityToTrade] = matchResult.value(); 

            // Set the OrderState to matched
            matchedOrder.setState(OrderState::Matched);
            newOrder.setState(OrderState::Matched);

            // Update the last trading price
            if (!matchedOrder.isMarketOrder() && !newOrder.isMarketOrder())
            {
                prevTransactionPrice = (matchedOrder.getTimeOfArrival() < newOrder.getTimeOfArrival()) ? matchedOrder.getLimitPrice() : newOrder.getLimitPrice();
            }

            else if (!matchedOrder.isMarketOrder())
            {
                prevTransactionPrice = matchedOrder.getLimitPrice();
            }

            else if (!newOrder.isMarketOrder())
            {
                prevTransactionPrice = newOrder.getLimitPrice();
            }

            // Set matched orders to executed and remove them from the pending list
            matchedOrder.setState(OrderState::Executed);
            newOrder.setState(OrderState::Executed);
            pendingOrders.erase(std::remove(pendingOrders.begin(), pendingOrders.end(), matchedOrder), pendingOrders.end());
            pendingOrders.erase(std::remove(pendingOrders.begin(), pendingOrders.end(), newOrder), pendingOrders.end());


            // Update the remaining quantity for partially executed orders
            if (newOrder.getQuantity() > quantityToTrade)
            {
                newOrder.setQuantity(newOrder.getQuantity() - quantityToTrade);
                newOrder.setState(OrderState::Pending);
                pendingOrders.push_back(newOrder);
            }

            if (matchedOrder.getQuantity() > quantityToTrade)
            {
                matchedOrder.setQuantity(matchedOrder.getQuantity() - quantityToTrade);
                matchedOrder.setState(OrderState::Pending);
                pendingOrders.push_back(matchedOrder);
            }

            // Try to match the order again until there is no longer a match
            matchResult = matchOrder(newOrder, pendingOrders, prevTransactionPrice);

            // Deal with trade execution
            Trade trade = (newOrder.getType() == 'B') ? Trade(newOrder, matchedOrder, quantityToTrade, prevTransactionPrice) : Trade(matchedOrder, newOrder, quantityToTrade, prevTransactionPrice);
            executedTrades.push_back(trade);

        }

        // Step 6: Add the unmatched new order to pending orders
        pendingOrders.push_back(newOrder);

        // Output the unmatched order to the file
        outputPartiallyExecutedOrders(newOrder);


        // Step 7: Display incremental updates
        displayIncrementalUpdates(pendingOrders, prevTransactionPrice, previousOrders, executedTrades);

        // Step 8: Update previousOrders with the current state
        previousOrders = pendingOrders;
    }

    inputFile.close();
    return 0;
}


// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
