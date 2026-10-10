#pragma once

#include "../model/CartItem.h"
#include "../repository/CartRepository.h"
#include <vector>

class CartService
{
public:
    CartService(CartRepository& repository);

    bool addToCart(const CartItem& item);

    std::vector<CartItem> getCartItems(int userId);

    bool removeFromCart(int id);

private:
    CartRepository& repository;
};