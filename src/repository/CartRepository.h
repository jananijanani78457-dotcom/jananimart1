#pragma once

#include "../model/CartItem.h"
#include <libpq-fe.h>
#include <vector>

class CartRepository
{
public:
    CartRepository(PGconn* connection);

    bool addToCart(const CartItem& item);

    std::vector<CartItem> getCartItems(int userId);

    bool removeFromCart(int id);

private:
    PGconn* conn;
};