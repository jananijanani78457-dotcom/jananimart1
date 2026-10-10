#include "ProductService.h"

ProductService::ProductService(ProductRepository& repository)
    : repository(repository)
{
}

bool ProductService::addProduct(const Product& product)
{
    return repository.addProduct(product);
}

bool ProductService::updateProduct(const Product& product)
{
    return repository.updateProduct(product);
}

bool ProductService::deleteProduct(int id)
{
    return repository.deleteProduct(id);
}

std::vector<Product> ProductService::getAllProducts()
{
    return repository.getAllProducts();
}