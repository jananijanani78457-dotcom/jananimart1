#pragma once

#include "../model/Product.h"
#include "../repository/ProductRepository.h"
#include <vector>

class ProductService
{
public:
    ProductService(ProductRepository& repository);

    bool addProduct(const Product& product);

    bool updateProduct(const Product& product);

    bool deleteProduct(int id);

    std::vector<Product> getAllProducts();

private:
    ProductRepository& repository;
};