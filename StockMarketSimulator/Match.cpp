#include "Match.h"

// Function to match orders
std::optional<std::pair<Order, int>> matchOrder(Order& newOrder, std::vector<Order>& pendingOrders, float prevTransactionPrice) {
    Order matchedOrder; // Placeholder for matched order
    bool matchFound = false;
    float executionPrice = 0.0f;
    int quantityToTrade = 0;
    Order sellingOrder;
    Order buyingOrder;

    // Determine the matching criteria based on order type
    for (auto& pendingOrder : pendingOrders) {
        if (newOrder.getType() == 'B' && pendingOrder.getType() == 'S') { // Match buy order to sell order

            if (newOrder.getLimitPrice() >= pendingOrder.getLimitPrice() || (newOrder.isMarketOrder() || pendingOrder.isMarketOrder())) {

                if (!matchFound || // First match
                    pendingOrder.getLimitPrice() < matchedOrder.getLimitPrice() || // Higher priority match
                    (pendingOrder.getLimitPrice() == matchedOrder.getLimitPrice() && pendingOrder.getTimeOfArrival() < matchedOrder.getTimeOfArrival())) { // Earlier arrival
                    matchedOrder = pendingOrder;
                    matchFound = true;
                    buyingOrder = newOrder;
                    sellingOrder = matchedOrder;
                }
            }
        }
        else if (newOrder.getType() == 'S' && pendingOrder.getType() == 'B') { // Match sell order to buy order
       
            if (newOrder.getLimitPrice() <= pendingOrder.getLimitPrice() || (newOrder.isMarketOrder() || pendingOrder.isMarketOrder())) {
                if (!matchFound || // First match
                    pendingOrder.getLimitPrice() > matchedOrder.getLimitPrice() || // Higher priority match
                    (pendingOrder.getLimitPrice() == matchedOrder.getLimitPrice() && pendingOrder.getTimeOfArrival() < matchedOrder.getTimeOfArrival())) { // Earlier arrival
                    matchedOrder = pendingOrder;
                    matchFound = true;
                    buyingOrder = matchedOrder;
                    sellingOrder = newOrder;
                }
            }
        }
    }

    if (!matchFound) {
        return std::nullopt; // No match found
    }

    // Determine execution price
    if (!newOrder.isMarketOrder() && !matchedOrder.isMarketOrder()) {
        executionPrice = (newOrder.getTimeOfArrival() < matchedOrder.getTimeOfArrival()) ? newOrder.getLimitPrice() : matchedOrder.getLimitPrice();
    }
    else if (!newOrder.isMarketOrder() && matchedOrder.isMarketOrder()) {
        executionPrice = newOrder.getLimitPrice();
    }
    else if (newOrder.isMarketOrder() && !matchedOrder.isMarketOrder()) {
        executionPrice = matchedOrder.getLimitPrice();
    }
    else {
        executionPrice = prevTransactionPrice;
    }

    // Determine quantity to trade
    quantityToTrade = std::min(newOrder.getQuantity(), matchedOrder.getQuantity());

    // Make the trade
    Trade trade(buyingOrder, sellingOrder, quantityToTrade, executionPrice);
    trades.push_back(trade);

    return std::make_pair(matchedOrder, quantityToTrade);
}
