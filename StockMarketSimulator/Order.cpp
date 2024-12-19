#include "Order.h"
#include <iostream>

// Constructors
Order::Order(std::string id, char type, int quantity, float limitPrice, bool isMarketOrder, std::chrono::system_clock::time_point toa)
    : orderID(id), type(type), quantity(quantity), limitPrice(limitPrice), _isMarketOrder(isMarketOrder), timeOfArrival(toa), state(OrderState::Pending), priority(10000) {
}

Order::Order() 
    : orderID(""), type('U'), quantity(0), limitPrice(0.0), _isMarketOrder(false), timeOfArrival(), state(OrderState::Pending), priority(0)  {

}

// Getters
std::string Order::getOrderID() const { return orderID; }
char Order::getType() const { return type; }
int Order::getQuantity() const { return quantity; }
float Order::getLimitPrice() const { return limitPrice; }
bool Order::isMarketOrder() const { return _isMarketOrder; }
std::chrono::system_clock::time_point Order::getTimeOfArrival() const { return timeOfArrival; }
OrderState Order::getState() const { return state; }
int Order::getPriority() const { return priority;}

// Setters
void Order::setState(OrderState newState) { state = newState; }
void Order::setQuantity(int newQuantity) { quantity = newQuantity; }


bool Order::operator == (const Order& other) const
{
    // Define what makes two Orders "equal"
    return this->getOrderID() == other.getOrderID() &&
        this->getType() == other.getType() &&
        this->getQuantity() == other.getQuantity() &&
        this->getLimitPrice() == other.getLimitPrice() &&
        this->isMarketOrder() == other.isMarketOrder();
}
