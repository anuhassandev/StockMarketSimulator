#ifndef DISPLAY_H
#define DISPLAY_H
#include <vector>
#include "Order.h"
#include "Trade.h"

void displayIncrementalUpdates(const std::vector<Order>& pendingOrders, float& lastTradedPrice, std::vector<Order>& previousOrders, std::vector<Trade> executedTrades);
void outputPartiallyExecutedOrders(Order order);

#endif