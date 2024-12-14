#ifndef MATCHING_LOGIC_H
#define MATCHING_LOGIC_H

#include <vector>
#include <optional>
#include "Order.h"
#include "Trade.h"

std::optional<std::pair<Order, int>> matchOrder(Order& newOrder, std::vector<Order>& pendingOrders, float prevTransactionPrice);
std::vector<Trade> trades;

#endif

