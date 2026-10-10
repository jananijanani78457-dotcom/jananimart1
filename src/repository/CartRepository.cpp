#include "CartRepository.h"
#include <iostream>
#include <string>

CartRepository::CartRepository(PGconn* connection)
{
    conn = connection;
}

bool CartRepository::addToCart(const CartItem& item)
{
    if (conn == nullptr)
    {
        std::cout << "Database connection is not available."
                  << std::endl;
        return false;
    }

    const char* query =
        "INSERT INTO cart_items "
        "(user_id, product_id, quantity) "
        "VALUES ($1, $2, $3);";

    std::string userId =
        std::to_string(item.user_id);

    std::string productId =
        std::to_string(item.product_id);

    std::string quantity =
        std::to_string(item.quantity);

    const char* values[] =
    {
        userId.c_str(),
        productId.c_str(),
        quantity.c_str()
    };

    PGresult* result = PQexecParams(
        conn,
        query,
        3,
        nullptr,
        values,
        nullptr,
        nullptr,
        0
    );

    if (PQresultStatus(result) != PGRES_COMMAND_OK)
    {
        std::cout << "Failed to add item to cart: "
                  << PQerrorMessage(conn)
                  << std::endl;

        PQclear(result);
        return false;
    }

    PQclear(result);

    std::cout << "Item added to cart successfully!"
              << std::endl;

    return true;
}

std::vector<CartItem>
CartRepository::getCartItems(int userId)
{
    std::vector<CartItem> items;

    if (conn == nullptr)
    {
        return items;
    }

    const char* query =
        "SELECT id, user_id, product_id, quantity "
        "FROM cart_items "
        "WHERE user_id = $1 "
        "ORDER BY id ASC;";

    std::string userIdValue =
        std::to_string(userId);

    const char* values[] =
    {
        userIdValue.c_str()
    };

    PGresult* result = PQexecParams(
        conn,
        query,
        1,
        nullptr,
        values,
        nullptr,
        nullptr,
        0
    );

    if (PQresultStatus(result) != PGRES_TUPLES_OK)
    {
        std::cout << "Failed to fetch cart items: "
                  << PQerrorMessage(conn)
                  << std::endl;

        PQclear(result);
        return items;
    }

    int rows = PQntuples(result);

    for (int i = 0; i < rows; i++)
    {
        CartItem item;

        item.id =
            std::stoi(PQgetvalue(result, i, 0));

        item.user_id =
            std::stoi(PQgetvalue(result, i, 1));

        item.product_id =
            std::stoi(PQgetvalue(result, i, 2));

        item.quantity =
            std::stoi(PQgetvalue(result, i, 3));

        items.push_back(item);
    }

    PQclear(result);

    return items;
}

bool CartRepository::removeFromCart(int id)
{
    if (conn == nullptr)
    {
        return false;
    }

    const char* query =
        "DELETE FROM cart_items "
        "WHERE id = $1;";

    std::string cartId =
        std::to_string(id);

    const char* values[] =
    {
        cartId.c_str()
    };

    PGresult* result = PQexecParams(
        conn,
        query,
        1,
        nullptr,
        values,
        nullptr,
        nullptr,
        0
    );

    if (PQresultStatus(result) != PGRES_COMMAND_OK)
    {
        std::cout << "Failed to remove cart item: "
                  << PQerrorMessage(conn)
                  << std::endl;

        PQclear(result);
        return false;
    }

    PQclear(result);

    std::cout << "Item removed from cart successfully!"
              << std::endl;

    return true;
}