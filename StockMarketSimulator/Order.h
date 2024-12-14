#ifndef ORDER_H
#define ORDER_H

#include <string>
#include <ctime>
#include "OrderState.h"

class Order
{
private:
	std::string orderID;
	char type; // 'B' for buy, 'S' for sell
	int quantity;
	float limitPrice;
	bool _isMarketOrder;
	std::time_t timeOfArrival;
	OrderState state;
	int priority;

public:
	Order(std::string id, char type, int quantity, float limitPrice, bool isMarketOrder, std::time_t toa);

	// Getters & Setters
	std::string getOrderID() const;
	char getType() const;
	int getQuantity() const;
	float getLimitPrice() const;
	bool isMarketOrder() const;
	std::time_t getTimeOfArrival() const;
	OrderState getState() const;

	void setState(OrderState newState);
	void setQuantity(int newQuantity);
	void calculatePriority(float lastTradePrice);
};

#endif