#include "OrderRepository.h"
#include <iostream>

OrderRepository::OrderRepository(PGconn* connection)
{
    conn = connection;
}

bool OrderRepository::createOrder(const Order& order)
{
    if (conn == nullptr)
    {
        std::cout << "Database connection is not available."
                  << std::endl;
        return false;
    }

    const char* query =
        "INSERT INTO orders "
        "(buyer_id, status, total_amount_cents) "
        "VALUES ($1, $2, $3);";

    std::string buyerId =
        std::to_string(order.buyer_id);

    std::string totalAmount =
        std::to_string(order.total_amount_cents);

    const char* values[] =
    {
        buyerId.c_str(),
        order.status.c_str(),
        totalAmount.c_str()
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
        std::cout << "Failed to create order: "
                  << PQerrorMessage(conn)
                  << std::endl;

        PQclear(result);
        return false;
    }

    PQclear(result);

    std::cout << "Order created successfully!"
              << std::endl;

    return true;
}

std::vector<Order>
OrderRepository::getOrdersByBuyer(int buyerId)
{
    std::vector<Order> orders;

    if (conn == nullptr)
    {
        return orders;
    }

    const char* query =
        "SELECT id, buyer_id, status, "
        "total_amount_cents "
        "FROM orders "
        "WHERE buyer_id = $1 "
        "ORDER BY id ASC;";

    std::string buyerIdValue =
        std::to_string(buyerId);

    const char* values[] =
    {
        buyerIdValue.c_str()
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
        std::cout << "Failed to fetch orders: "
                  << PQerrorMessage(conn)
                  << std::endl;

        PQclear(result);
        return orders;
    }

    int rows = PQntuples(result);

    for (int i = 0; i < rows; i++)
    {
        Order order;

        order.id =
            std::stoi(PQgetvalue(result, i, 0));

        order.buyer_id =
            std::stoi(PQgetvalue(result, i, 1));

        order.status =
            PQgetvalue(result, i, 2);

        order.total_amount_cents =
            std::stoll(PQgetvalue(result, i, 3));

        orders.push_back(order);
    }

    PQclear(result);

    return orders;
}