#ifndef ORDER_H
#define ORDER_H

#include <string>
#include "OrderState.h"
#include <chrono>

class Order
{
private:
	std::string orderID;
	char type; // 'B' for buy, 'S' for sell
	unsigned int quantity;
	float limitPrice;
	bool _isMarketOrder;
	std::chrono::system_clock::time_point timeOfArrival;
	OrderState state;
	int priority;

public:
	Order(std::string id, char type, int quantity, float limitPrice, bool isMarketOrder, std::chrono::system_clock::time_point toa);
	Order();

	// Getters & Setters
	std::string getOrderID() const;
	char getType() const;
	int getQuantity() const;
	float getLimitPrice() const;
	bool isMarketOrder() const;
	std::chrono::system_clock::time_point getTimeOfArrival() const;
	OrderState getState() const;
	int getPriority() const;

	void setState(OrderState newState);
	void setQuantity(int newQuantity);

	bool operator == (const Order& other) const;

};

#endif