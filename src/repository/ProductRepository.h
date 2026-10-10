#pragma once

#include "../model/Product.h"
#include <libpq-fe.h>
#include <vector>

class ProductRepository
{
public:
    ProductRepository(PGconn* connection);

    std::vector<Product> getAllProducts();

    bool addProduct(const Product& product);

    bool updateProduct(const Product& product);

    bool deleteProduct(int id);

private:
    PGconn* conn;
};