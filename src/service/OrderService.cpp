#include "OrderService.h"

OrderService::OrderService(OrderRepository& repository)
    : repository(repository)
{
}

bool OrderService::createOrder(const Order& order)
{
    return repository.createOrder(order);
}

std::vector<Order>
OrderService::getOrdersByBuyer(int buyerId)
{
    return repository.getOrdersByBuyer(buyerId);
}