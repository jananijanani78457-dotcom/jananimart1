#pragma once

#include "../model/OrderItem.h"
#include "../repository/OrderItemRepository.h"
#include <vector>

class OrderItemService
{
public:
    OrderItemService(OrderItemRepository& repository);

    bool createOrderItem(const OrderItem& item);

    std::vector<OrderItem> getItemsByOrder(int orderId);

private:
    OrderItemRepository& repository;
};