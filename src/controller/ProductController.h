#pragma once

#include <drogon/HttpController.h>

class ProductController
    : public drogon::HttpController<ProductController>
{
public:
    METHOD_LIST_BEGIN

    ADD_METHOD_TO(
        ProductController::getProducts,
        "/api/v1/products",
        drogon::Get
    );

    ADD_METHOD_TO(
        ProductController::createProduct,
        "/api/v1/products",
        drogon::Post
    );

    ADD_METHOD_TO(
        ProductController::updateProduct,
        "/api/v1/products/{1}",
        drogon::Put
    );

    ADD_METHOD_TO(
        ProductController::deleteProduct,
        "/api/v1/products/{1}",
        drogon::Delete
    );

    METHOD_LIST_END

    void getProducts(
        const drogon::HttpRequestPtr& request,
        std::function<void(const drogon::HttpResponsePtr&)>&& callback
    );

    void createProduct(
        const drogon::HttpRequestPtr& request,
        std::function<void(const drogon::HttpResponsePtr&)>&& callback
    );

    void updateProduct(
        const drogon::HttpRequestPtr& request,
        std::function<void(const drogon::HttpResponsePtr&)>&& callback,
        int id
    );

    void deleteProduct(
        const drogon::HttpRequestPtr& request,
        std::function<void(const drogon::HttpResponsePtr&)>&& callback,
        int id
    );
};

