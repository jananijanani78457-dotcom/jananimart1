#include "ProductRepository.h"
#include <iostream>

ProductRepository::ProductRepository(PGconn* connection)
{
    conn = connection;
}

std::vector<Product> ProductRepository::getAllProducts()
{
    std::vector<Product> products;

    if (conn == nullptr)
    {
        std::cout << "Database connection is not available."
                  << std::endl;
        return products;
    }

    const char* query =
        "SELECT id, seller_id, name, description, "
        "price_cents, stock_qty, category, image_url "
        "FROM products ORDER BY id ASC;";

    PGresult* result = PQexec(conn, query);

    if (PQresultStatus(result) != PGRES_TUPLES_OK)
    {
        std::cout << "Failed to fetch products: "
                  << PQerrorMessage(conn) << std::endl;

        PQclear(result);
        return products;
    }

    int rows = PQntuples(result);

    for (int i = 0; i < rows; i++)
    {
        Product product;

        product.id = std::stoi(PQgetvalue(result, i, 0));
        product.seller_id = std::stoi(PQgetvalue(result, i, 1));
        product.name = PQgetvalue(result, i, 2);
        product.description = PQgetvalue(result, i, 3);
        product.price_cents = std::stoi(PQgetvalue(result, i, 4));
        product.stock_qty = std::stoi(PQgetvalue(result, i, 5));
        product.category = PQgetvalue(result, i, 6);
        product.image_url = PQgetvalue(result, i, 7);

        products.push_back(product);
    }

    PQclear(result);

    return products;
}

// ==================== CREATE PRODUCT ====================

bool ProductRepository::addProduct(const Product& product)
{
    if (conn == nullptr)
    {
        std::cout << "Database connection is not available."
                  << std::endl;
        return false;
    }

    const char* query =
        "INSERT INTO products "
        "(seller_id, name, description, price_cents, stock_qty, category, image_url) "
        "VALUES ($1, $2, $3, $4, $5, $6, $7);";

    std::string sellerId = std::to_string(product.seller_id);
    std::string price = std::to_string(product.price_cents);
    std::string stock = std::to_string(product.stock_qty);

    const char* values[] =
    {
        sellerId.c_str(),
        product.name.c_str(),
        product.description.c_str(),
        price.c_str(),
        stock.c_str(),
        product.category.c_str(),
        product.image_url.c_str()
    };

    PGresult* result = PQexecParams(
        conn,
        query,
        7,
        nullptr,
        values,
        nullptr,
        nullptr,
        0
    );

    if (PQresultStatus(result) != PGRES_COMMAND_OK)
    {
        std::cout << "Failed to add product: "
                  << PQerrorMessage(conn) << std::endl;

        PQclear(result);
        return false;
    }

    PQclear(result);

    std::cout << "Product added successfully!"
              << std::endl;

    return true;
}

// ==================== UPDATE PRODUCT ====================

bool ProductRepository::updateProduct(const Product& product)
{
    if (conn == nullptr)
    {
        std::cout << "Database connection is not available."
                  << std::endl;
        return false;
    }

    const char* query =
        "UPDATE products SET "
        "seller_id = $1, "
        "name = $2, "
        "description = $3, "
        "price_cents = $4, "
        "stock_qty = $5, "
        "category = $6, "
        "image_url = $7 "
        "WHERE id = $8;";

    std::string sellerId = std::to_string(product.seller_id);
    std::string price = std::to_string(product.price_cents);
    std::string stock = std::to_string(product.stock_qty);
    std::string id = std::to_string(product.id);

    const char* values[] =
    {
        sellerId.c_str(),
        product.name.c_str(),
        product.description.c_str(),
        price.c_str(),
        stock.c_str(),
        product.category.c_str(),
        product.image_url.c_str(),
        id.c_str()
    };

    PGresult* result = PQexecParams(
        conn,
        query,
        8,
        nullptr,
        values,
        nullptr,
        nullptr,
        0
    );

    if (PQresultStatus(result) != PGRES_COMMAND_OK)
    {
        std::cout << "Failed to update product: "
                  << PQerrorMessage(conn) << std::endl;

        PQclear(result);
        return false;
    }

    PQclear(result);

    std::cout << "Product updated successfully!"
              << std::endl;

    return true;
}

// ==================== DELETE PRODUCT ====================

bool ProductRepository::deleteProduct(int id)
{
    if (conn == nullptr)
    {
        std::cout << "Database connection is not available."
                  << std::endl;
        return false;
    }

    const char* query =
        "DELETE FROM products WHERE id = $1;";

    std::string productId = std::to_string(id);

    const char* values[] =
    {
        productId.c_str()
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
        std::cout << "Failed to delete product: "
                  << PQerrorMessage(conn) << std::endl;

        PQclear(result);
        return false;
    }

    PQclear(result);

    std::cout << "Product deleted successfully!"
              << std::endl;

    return true;
}