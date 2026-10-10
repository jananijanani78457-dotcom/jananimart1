#pragma once

#include "../model/Order.h"
#include "../repository/OrderRepository.h"
#include <vector>

class OrderService
{
public:
    OrderService(OrderRepository& repository);

    bool createOrder(const Order& order);

    std::vector<Order> getOrdersByBuyer(int buyerId);

private:
    OrderRepository& repository;
};