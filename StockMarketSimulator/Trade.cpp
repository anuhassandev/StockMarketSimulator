#include "Trade.h"

Trade::Trade(const Order buyingOrder, const Order sellingOrder, int tradedQuantity, float executionPrice)
    : buyingOrder(buyingOrder), sellingOrder(sellingOrder), tradedQuantity(tradedQuantity), executionPrice(executionPrice) {
}

// Getter for execution price
float Trade::getExecutionPrice() const { return executionPrice; }
