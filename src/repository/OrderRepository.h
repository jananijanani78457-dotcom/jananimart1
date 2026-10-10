#pragma once

#include "../model/Order.h"
#include <libpq-fe.h>
#include <vector>

class OrderRepository
{
public:
    OrderRepository(PGconn* connection);

    bool createOrder(const Order& order);

    std::vector<Order> getOrdersByBuyer(int buyerId);

private:
    PGconn* conn;
};