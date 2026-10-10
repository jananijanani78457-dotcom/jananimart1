#pragma once

struct OrderItem
{
    int id;
    int order_id;
    int product_id;
    int quantity;
    long long unit_price_cents;
};