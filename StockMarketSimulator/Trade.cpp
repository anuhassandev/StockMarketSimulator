#include "Trade.h"

Trade::Trade(const Order buyingOrder, const Order sellingOrder, int tradedQuantity, float executionPrice)
    : buyingOrder(buyingOrder), sellingOrder(sellingOrder), tradedQuantity(tradedQuantity), executionPrice(executionPrice) {
}

Trade::Trade()
    : buyingOrder(), sellingOrder(), tradedQuantity(0), executionPrice(0.0) {
}

// Getter for execution price
float Trade::getExecutionPrice() const { return executionPrice; }
