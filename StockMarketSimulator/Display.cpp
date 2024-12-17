#include "Display.h"
#include <iostream>
#include <iomanip>
#include <vector>
#include <algorithm>
#include "Order.h" 
#include <sstream>

void displayIncrementalUpdates(const std::vector<Order>& pendingOrders, float& lastTradedPrice, std::vector<Order>& previousOrders)
{
    static bool firstCall = true;
    std::vector<Order> buyOrders;
    std::vector<Order> sellOrders;
    std::vector<Order> newOrders;
    std::vector<Order> updatedOrders;
    std::vector<Order> removedOrders = previousOrders;


    // Separate pendingOrders into Buy and Sell orders
    for (const auto& order : pendingOrders)
    {
        if (order.getType() == 'B')
            buyOrders.push_back(order);
        else if (order.getType() == 'S')
            sellOrders.push_back(order);
    }

    // Compare current pendingOrders with previousOrders to identify changes
    for (const auto& order : pendingOrders)
    {
        auto it = std::find(previousOrders.begin(), previousOrders.end(), order);
        if (it == previousOrders.end())
        {
            // New order
            newOrders.push_back(order);
        }
        else
        {
            // Check for updates to the order
            if (it->getQuantity() != order.getQuantity())
                updatedOrders.push_back(order);

            // Remove from removedOrders list, because it's still active
            removedOrders.erase(std::remove(removedOrders.begin(), removedOrders.end(), order), removedOrders.end());
        }
    }

    // First call: print the headers and correct formatting
    if (firstCall)
    {
        std::cout << "Last trading price: " << std::fixed << std::setprecision(2) << lastTradedPrice << "\n\n";
        std::cout << std::left << std::setw(40) << "Buy" << "Sell\n";
        std::cout << "---------------------------------------------------------\n";

        // Print all pending orders
        size_t maxRows = std::max(buyOrders.size(), sellOrders.size());
        for (size_t i = 0; i < maxRows; ++i)
        {
            // Print Buy order
            if (i < buyOrders.size())
            {
                const auto& order = buyOrders[i];
                if (order.isMarketOrder())
                    std::cout << std::left << std::setw(10) << order.getOrderID() << "M     " << std::setw(6) << order.getQuantity();
                else
                    std::cout << std::left << std::setw(10) << order.getOrderID() << std::setw(6) << order.getLimitPrice() << std::setw(6) << order.getQuantity();
            }
            else
            {
                std::cout << std::setw(25) << ""; // Empty space if no buy order
            }

            // Print Sell order
            if (i < sellOrders.size())
            {
                const auto& order = sellOrders[i];
                if (order.isMarketOrder())
                    std::cout << std::right << std::setw(10) << order.getOrderID() << " M     " << std::setw(6) << order.getQuantity();
                else
                    std::cout << std::right << std::setw(10) << order.getOrderID() << std::setw(6) << order.getLimitPrice() << std::setw(6) << order.getQuantity();
                
                std::cout << std::endl;
            }

        }

        // Mark as no longer the first call
        firstCall = false;
    }

    // Incremental updates: only display the new orders or changes
    else
    {
        for (const auto& order : newOrders)
        {
            if (order.getType() == 'B') // Buy orders
            {
                std::stringstream limprice;
                limprice << std::fixed << std::setprecision(2) << order.getLimitPrice();
                std::string limpricestring = limprice.str();
                std::cout << std::left << std::setw(10) << order.getOrderID()
                    << std::setw(6) << (order.isMarketOrder() ? "M" : limpricestring)
                    << std::setw(6) << order.getQuantity();
                std::cout << std::right << std::setw(25) << "\n"; // Align with Buy column
            }
            else if (order.getType() == 'S') // Sell orders
            {
                std::stringstream limprice;
                limprice << std::fixed << std::setprecision(2) << order.getLimitPrice();
                std::string limpricestring = limprice.str();
                std::cout << std::setw(10) << "" // Blank space for Buy side
                    << std::right << std::setw(10) << order.getOrderID()
                    << std::setw(6) << (order.isMarketOrder() ? "M" : limpricestring)
                    << std::setw(6) << order.getQuantity() << "\n";
            }
        }
    }

    // Update previousOrders for future comparison
    previousOrders = pendingOrders;
}



