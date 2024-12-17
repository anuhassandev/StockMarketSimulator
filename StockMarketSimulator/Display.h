#ifndef DISPLAY_H
#define DISPLAY_H
#include <vector>
#include "Order.h"

void displayIncrementalUpdates(const std::vector<Order>& pendingOrders, float& lastTradedPrice, std::vector<Order>& previousOrders);


#endif