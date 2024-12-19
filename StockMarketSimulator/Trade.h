#ifndef TRADE_H
#define TRADE_H

#include "Order.h"

class Trade {
private:
    Order buyingOrder;
    Order sellingOrder;
    int tradedQuantity;
    float executionPrice;

public:
    Trade(const Order buyingOrder, const Order sellingOrder, int tradedQuantity, float executionPrice);
    Trade();

    Order getBuyingOrder() const;
    Order getSellingOrder() const;
    int getTradedQuantity() const;
    float getExecutionPrice() const;
};

#endif
