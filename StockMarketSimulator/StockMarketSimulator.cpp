// StockMarketSimulator.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include "Order.h"
#include <vector>
#include "Trade.h"

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

        // Try and match the new order to a pending order:
        Order matchedOrder;
        Trade trade;

        if (pendingOrders.size() > 0)
        {
            if (anOrder.getType() == 'B') // Buying order
            {
                // Traverse the pending orders to try and find a seller
                for (auto o : pendingOrders)
                {
                    if (o.getType() == 'S')
                    {
                        // Limit price of buyer must be >= limit price of seller OR one is a market order to be matched
                        if (anOrder.getLimitPrice() >= o.getLimitPrice() || (o.isMarketOrder() || anOrder.isMarketOrder()))
                        {
                            if (matchedOrder.getType() == 'U') // Undefined, not a real order
                            {
                                // We have found a match!
                                matchedOrder = o;
                            }

                            else
                            {
                                // We already have a match, need to let them fight for the title
                                Order winner;

                                if (o.getLimitPrice() < matchedOrder.getLimitPrice())
                                {
                                    winner = o; // o wins here due to having the lower limit price
                                }

                                else if (o.getLimitPrice() > matchedOrder.getLimitPrice())
                                {
                                    winner = matchedOrder; // o wins here due to having the lower limit price
                                }

                                else
                                {
                                    // logically they must be equal, so its fist come first served
                                    winner = (o.getTimeOfArrival() > matchedOrder.getTimeOfArrival()) ? o : matchedOrder;
                                }

                                if (o.isMarketOrder() && !matchedOrder.isMarketOrder())
                                {
                                    winner = o; // o wins here due to being a market order
                                }

                                else if (!o.isMarketOrder() && matchedOrder.isMarketOrder())
                                {
                                    winner = matchedOrder; // matchedOrder wins here due to being a market order
                                }

                                else if (o.isMarketOrder() && matchedOrder.isMarketOrder())
                                {
                                    // both market orders, give priority to earlier one
                                    winner = (o.getTimeOfArrival() > matchedOrder.getTimeOfArrival()) ? o : matchedOrder;
                                }

                                matchedOrder = winner;

                            }
                        }
                    }
                }
            }



            else if (anOrder.getType() == 'S') // Selling order
            {
                // Traverse the pending orders to try and find a buyer
                for (auto o : pendingOrders)
                {
                    if (o.getType() == 'B')
                    {
                        // Limit price of seller must be <= limit price of buyer OR one is a market order to be matched
                        if (anOrder.getLimitPrice() <= o.getLimitPrice() || (o.isMarketOrder() || anOrder.isMarketOrder()))
                        {
                            if (matchedOrder.getType() == 'U') // Undefined, not a real order
                            {
                                // We have found a match!
                                matchedOrder = o;
                            }

                            else
                            {
                                // We already have a match, need to let them fight for the title
                                Order winner;

                                if (o.getLimitPrice() > matchedOrder.getLimitPrice())
                                {
                                    winner = o; // o wins here due to having the higher limit price
                                }

                                else if (o.getLimitPrice() < matchedOrder.getLimitPrice())
                                {
                                    winner = matchedOrder; // matchedOrder wins here due to having the higher limit price
                                }

                                else
                                {
                                    // logically they must be equal, so its fist come first served
                                    winner = (o.getTimeOfArrival() > matchedOrder.getTimeOfArrival()) ? o : matchedOrder;
                                }

                                if (o.isMarketOrder() && !matchedOrder.isMarketOrder())
                                {
                                    winner = o; // o wins here due to being a market order
                                }

                                else if (!o.isMarketOrder() && matchedOrder.isMarketOrder())
                                {
                                    winner = matchedOrder; // matchedOrder wins here due to being a market order
                                }

                                else if (o.isMarketOrder() && matchedOrder.isMarketOrder())
                                {
                                    // both market orders, give priority to earlier one
                                    winner = (o.getTimeOfArrival() > matchedOrder.getTimeOfArrival()) ? o : matchedOrder;
                                }

                                matchedOrder = winner;

                            }
                        }
                    }
                }
            }
        }

        // If we have found a winning order, let's make a trade
        if (matchedOrder.getType() != 'U')
        {
            float executionPrice;

            // Calculate execution price
            if (!anOrder.isMarketOrder() && !matchedOrder.isMarketOrder())
            {
                executionPrice = (anOrder.getTimeOfArrival() > matchedOrder.getTimeOfArrival()) ? anOrder.getLimitPrice() : matchedOrder.getLimitPrice();
            }
            
            else if (!anOrder.isMarketOrder() && matchedOrder.isMarketOrder())
            {
                executionPrice = matchedOrder.getLimitPrice();
            }

            else if (anOrder.isMarketOrder() && !matchedOrder.isMarketOrder())
            {
                executionPrice = anOrder.getLimitPrice();
            }

            else
            {
                // both must be market orders
                executionPrice = prevTransactionPrice;
            }

            // Now we calculate the quantity
            int quantityToTrade; 

            if (anOrder.getQuantity() == matchedOrder.getQuantity())
            {
                quantityToTrade = anOrder.getQuantity(); // full trade!
            }
            
            else if (anOrder.getQuantity() < matchedOrder.getQuantity())
            {
                quantityToTrade = anOrder.getQuantity();
                int remainingQuantity = matchedOrder.getQuantity() - quantityToTrade;
                Order remainingOrder(matchedOrder.getOrderID(), matchedOrder.getType(), remainingQuantity, matchedOrder.getLimitPrice(), matchedOrder.isMarketOrder(), matchedOrder.getTimeOfArrival());
                //pendingOrders.push_back(remainingOrder);

                // Let's try and recursively get this matched until it is in its smallest form
                for (auto potential : pendingOrders)
                {

                }

            }

            else if (anOrder.getQuantity() > matchedOrder.getQuantity())
            {
                quantityToTrade = matchedOrder.getQuantity();
                int remainingQuantity = anOrder.getQuantity() - quantityToTrade;
                Order remainingOrder(anOrder.getOrderID(), anOrder.getType(), remainingQuantity, anOrder.getLimitPrice(), anOrder.isMarketOrder(), anOrder.getTimeOfArrival());
                //pendingOrders.push_back(remainingOrder);
            }


        }
        


        
        pendingOrders.push_back(anOrder);
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
