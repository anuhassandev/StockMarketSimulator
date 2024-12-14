#include "Order.h"
#include <iostream>

// Constructor
Order::Order(std::string id, char type, int quantity, float limitPrice, bool isMarketOrder, std::time_t toa)
    : orderID(id), type(type), quantity(quantity), limitPrice(limitPrice), _isMarketOrder(isMarketOrder), timeOfArrival(toa), state(OrderState::Pending), priority(10000) {
}

// Getters
std::string Order::getOrderID() const { return orderID; }
char Order::getType() const { return type; }
int Order::getQuantity() const { return quantity; }
float Order::getLimitPrice() const { return limitPrice; }
bool Order::isMarketOrder() const { return _isMarketOrder; }
std::time_t Order::getTimeOfArrival() const { return timeOfArrival; }
OrderState Order::getState() const { return state; }

// Setters
void Order::setState(OrderState newState) { state = newState; }
void Order::setQuantity(int newQuantity) { quantity = newQuantity; }


void Order::calculatePriority(float lastTradePrice) 
{
    if (type == 'B')
    {
        // larger the limit price, larger the priority
        int intLimitPrice = static_cast<int>(limitPrice * 100.0);
        priority = priority + limitPrice;
    }
    else
    {
        // lower limit price = higher priority
        int intLimitPrice = static_cast<int>(limitPrice * 100.0);
        priority = priority - limitPrice; 
    }
}