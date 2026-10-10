#include "OrderItemService.h"
OrderItemService::OrderItemService(
    OrderItemRepository& repository
)
    : repository(repository)
{
}

bool OrderItemService::createOrderItem(
    const OrderItem& item
)
{
    return repository.createOrderItem(item);
}

std::vector<OrderItem>
OrderItemService::getItemsByOrder(int orderId)
{
    return repository.getItemsByOrder(orderId);
}