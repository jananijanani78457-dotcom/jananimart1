#include "CartService.h"

CartService::CartService(CartRepository& repository)
    : repository(repository)
{
}

bool CartService::addToCart(const CartItem& item)
{
    return repository.addToCart(item);
}

std::vector<CartItem>
CartService::getCartItems(int userId)
{
    return repository.getCartItems(userId);
}

bool CartService::removeFromCart(int id)
{
    return repository.removeFromCart(id);
}