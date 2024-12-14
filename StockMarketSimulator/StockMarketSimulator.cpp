// StockMarketSimulator.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include "Order.h"
#include <vector>
#include "Trade.h"
#include "Match.h"

int main(int argc, char* argv[])
{
	std::ifstream inputFile(argv[1]);
	std::string lineOfText;
	float prevTransactionPrice;
    std::vector<Order> pendingOrders;

    // Read the first line to get the previous transaction price
    if (getline(inputFile, lineOfText)) 
    {
        std::istringstream iss(lineOfText);
        iss >> prevTransactionPrice;
    }

    // Process the rest of the lines for orders
    while (getline(inputFile, lineOfText)) 
    {
        std::istringstream iss(lineOfText);

        // Declare Variables
        std::string orderID;
        char orderType;  // B or S
        int quantity;
        float limitPrice = 0.0f;
        bool isMarketOrder = false;

        // Parse the fields in the line
        if (iss >> orderID >> orderType >> quantity) 
        {
            if (!(iss >> limitPrice)) 
            {
                // If no limit price is provided, it's a market order
                isMarketOrder = true;
            }
        }

        Order anOrder(orderID, orderType, quantity, limitPrice, isMarketOrder, std::chrono::system_clock::now());
        pendingOrders.push_back(anOrder);

        auto matchResult = matchOrder(anOrder, pendingOrders, prevTransactionPrice);

        if (matchResult.has_value()) {
            auto [matchedOrder, quantityToTrade] = matchResult.value();

            // Update order quantities and handle residuals
            if (anOrder.getQuantity() > quantityToTrade) {
                anOrder.setQuantity(anOrder.getQuantity() - quantityToTrade);
                pendingOrders.push_back(anOrder); // Add remaining part back to pending orders
            }

            if (matchedOrder.getQuantity() > quantityToTrade) {
                matchedOrder.setQuantity(matchedOrder.getQuantity() - quantityToTrade);
                pendingOrders.push_back(matchedOrder); // Add remaining part back to pending orders
            }

            // Remove fully executed matched order
            pendingOrders.erase(std::remove_if(pendingOrders.begin(), pendingOrders.end(),
                [&matchedOrder](const Order& o) { return o.getOrderID() == matchedOrder.getOrderID(); }),
                pendingOrders.end());
        }

        // Recursively try and match the residual order
        while (anOrder.getQuantity() > 0) {
            auto matchResult = matchOrder(anOrder, pendingOrders, prevTransactionPrice);

            if (!matchResult.has_value()) {
                break; // No further matches
            }

            auto [matchedOrder, quantityToTrade] = matchResult.value();

            // Handle residuals as before
            if (anOrder.getQuantity() > quantityToTrade) {
                anOrder.setQuantity(anOrder.getQuantity() - quantityToTrade);
            }

            if (matchedOrder.getQuantity() > quantityToTrade) {
                matchedOrder.setQuantity(matchedOrder.getQuantity() - quantityToTrade);
                pendingOrders.push_back(matchedOrder); // Add remaining part back to pending orders
            }

            // Remove fully executed matched order
            pendingOrders.erase(std::remove_if(pendingOrders.begin(), pendingOrders.end(),
                [&matchedOrder](const Order& o) { return o.getOrderID() == matchedOrder.getOrderID(); }),
                pendingOrders.end());
        }
        
    }

    inputFile.close();
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
