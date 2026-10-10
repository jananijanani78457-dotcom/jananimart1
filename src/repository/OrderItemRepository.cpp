#include "OrderItemRepository.h"
#include <iostream>
#include <string>

OrderItemRepository::OrderItemRepository(PGconn* connection)
{
    conn = connection;
}

bool OrderItemRepository::createOrderItem(
    const OrderItem& item
)
{
    if (conn == nullptr)
    {
        std::cout << "Database connection is not available."
                  << std::endl;
        return false;
    }

    const char* query =
        "INSERT INTO order_items "
        "(order_id, product_id, quantity, unit_price_cents) "
        "VALUES ($1, $2, $3, $4);";

    std::string orderId =
        std::to_string(item.order_id);

    std::string productId =
        std::to_string(item.product_id);

    std::string quantity =
        std::to_string(item.quantity);

    std::string unitPrice =
        std::to_string(item.unit_price_cents);

    const char* values[] =
    {
        orderId.c_str(),
        productId.c_str(),
        quantity.c_str(),
        unitPrice.c_str()
    };

    PGresult* result = PQexecParams(
        conn,
        query,
        4,
        nullptr,
        values,
        nullptr,
        nullptr,
        0
    );

    if (PQresultStatus(result) != PGRES_COMMAND_OK)
    {
        std::cout << "Failed to create order item: "
                  << PQerrorMessage(conn)
                  << std::endl;

        PQclear(result);
        return false;
    }

    PQclear(result);

    std::cout << "Order item created successfully!"
              << std::endl;

    return true;
}

std::vector<OrderItem>
OrderItemRepository::getItemsByOrder(int orderId)
{
    std::vector<OrderItem> items;

    if (conn == nullptr)
    {
        return items;
    }

    const char* query =
        "SELECT id, order_id, product_id, "
        "quantity, unit_price_cents "
        "FROM order_items "
        "WHERE order_id = $1 "
        "ORDER BY id ASC;";

    std::string orderIdValue =
        std::to_string(orderId);

    const char* values[] =
    {
        orderIdValue.c_str()
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
        std::cout << "Failed to fetch order items: "
                  << PQerrorMessage(conn)
                  << std::endl;

        PQclear(result);
        return items;
    }

    int rows = PQntuples(result);

    for (int i = 0; i < rows; i++)
    {
        OrderItem item;

        item.id =
            std::stoi(PQgetvalue(result, i, 0));

        item.order_id =
            std::stoi(PQgetvalue(result, i, 1));

        item.product_id =
            std::stoi(PQgetvalue(result, i, 2));

        item.quantity =
            std::stoi(PQgetvalue(result, i, 3));

        item.unit_price_cents =
            std::stoll(PQgetvalue(result, i, 4));

        items.push_back(item);
    }

    PQclear(result);

    return items;
}