#ifndef MATCHING_LOGIC_H
#define MATCHING_LOGIC_H

#include <vector>
#include <optional>
#include "Order.h"

std::optional<std::pair<Order, int>> matchOrder(Order& newOrder, std::vector<Order>& pendingOrders, float prevTransactionPrice);

#endif

