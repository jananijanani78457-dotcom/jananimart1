#pragma once

#include "../model/OrderItem.h"
#include <libpq-fe.h>
#include <vector>

class OrderItemRepository
{
public:
    OrderItemRepository(PGconn* connection);

    bool createOrderItem(const OrderItem& item);

    std::vector<OrderItem> getItemsByOrder(int orderId);

private:
    PGconn* conn;
};