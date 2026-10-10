#include "ProductController.h"

#include "../database.h"
#include "../repository/ProductRepository.h"
#include "../service/ProductService.h"
#include "../model/Product.h"

#include <functional>
#include <vector>
#include <string>
#include <exception>

namespace
{
    drogon::HttpResponsePtr jsonResponse(
        const Json::Value& body,
        drogon::HttpStatusCode status)
    {
        auto response =
            drogon::HttpResponse::newHttpJsonResponse(body);

        response->setStatusCode(status);
        return response;
    }

    Json::Value errorResponse(const std::string& message)
    {
        Json::Value response;
        response["success"] = false;
        response["data"] = Json::nullValue;
        response["error"] = message;
        return response;
    }

    bool readProduct(
        const Json::Value& body,
        Product& product,
        bool includeId)
    {
        if (!body.isObject() ||
            !body.isMember("name") ||
            !body["name"].isString() ||
            !body.isMember("seller_id") ||
            !body["seller_id"].isInt() ||
            !body.isMember("price_cents") ||
            !body["price_cents"].isInt() ||
            !body.isMember("stock_qty") ||
            !body["stock_qty"].isInt())
        {
            return false;
        }

        if (body["name"].asString().empty() ||
            body["seller_id"].asInt() <= 0 ||
            body["price_cents"].asInt() < 0 ||
            body["stock_qty"].asInt() < 0)
        {
            return false;
        }

        if (includeId)
        {
            if (!body.isMember("id") ||
                !body["id"].isInt() ||
                body["id"].asInt() <= 0)
            {
                return false;
            }

            product.id = body["id"].asInt();
        }

        product.seller_id = body["seller_id"].asInt();
        product.name = body["name"].asString();
        product.price_cents = body["price_cents"].asInt();
        product.stock_qty = body["stock_qty"].asInt();

        product.description =
            body.isMember("description") &&
            body["description"].isString()
            ? body["description"].asString() : "";

        product.category =
            body.isMember("category") &&
            body["category"].isString()
            ? body["category"].asString() : "";

        product.image_url =
            body.isMember("image_url") &&
            body["image_url"].isString()
            ? body["image_url"].asString() : "";

        return true;
    }

    Json::Value productToJson(const Product& product)
    {
        Json::Value item;
        item["id"] = product.id;
        item["seller_id"] = product.seller_id;
        item["name"] = product.name;
        item["description"] = product.description;
        item["price_cents"] = product.price_cents;
        item["stock_qty"] = product.stock_qty;
        item["category"] = product.category;
        item["image_url"] = product.image_url;
        return item;
    }
}

void ProductController::getProducts(
    const drogon::HttpRequestPtr& request,
    std::function<void(const drogon::HttpResponsePtr&)>&& callback)
{
    Database database;

    if (database.getConnection() == nullptr)
    {
        callback(jsonResponse(
            errorResponse("Database connection failed"),
            drogon::k500InternalServerError));
        return;
    }

    ProductRepository repository(database.getConnection());
    ProductService service(repository);

    Json::Value response;
    Json::Value productList(Json::arrayValue);

    try
    {
        std::vector<Product> products = service.getAllProducts();

        for (const auto& product : products)
        {
            productList.append(productToJson(product));
        }

        response["success"] = true;
        response["data"] = productList;
        response["error"] = Json::nullValue;

        callback(jsonResponse(response, drogon::k200OK));
    }
    catch (const std::exception& e)
    {
        callback(jsonResponse(
            errorResponse(e.what()),
            drogon::k500InternalServerError));
    }
}

void ProductController::createProduct(
    const drogon::HttpRequestPtr& request,
    std::function<void(const drogon::HttpResponsePtr&)>&& callback)
{
    auto body = request->getJsonObject();

    if (!body)
    {
        callback(jsonResponse(
            errorResponse("A valid JSON body is required"),
            drogon::k400BadRequest));
        return;
    }

    Product product{};

    if (!readProduct(*body, product, false))
    {
        callback(jsonResponse(
            errorResponse(
                "Required fields: name, seller_id, price_cents, stock_qty. "
                "Values must be valid and non-negative where applicable."),
            drogon::k400BadRequest));
        return;
    }

    Database database;

    if (database.getConnection() == nullptr)
    {
        callback(jsonResponse(
            errorResponse("Database connection failed"),
            drogon::k500InternalServerError));
        return;
    }

    ProductRepository repository(database.getConnection());
    ProductService service(repository);

    try
    {
        if (!service.addProduct(product))
        {
            callback(jsonResponse(
                errorResponse("Failed to create product"),
                drogon::k500InternalServerError));
            return;
        }

        Json::Value response;
        response["success"] = true;
        response["data"] = productToJson(product);
        response["error"] = Json::nullValue;

        callback(jsonResponse(response, drogon::k201Created));
    }
    catch (const std::exception& e)
    {
        callback(jsonResponse(
            errorResponse(e.what()),
            drogon::k500InternalServerError));
    }
}

void ProductController::updateProduct(
    const drogon::HttpRequestPtr& request,
    std::function<void(const drogon::HttpResponsePtr&)>&& callback,
    int id)
{
    if (id <= 0)
    {
        callback(jsonResponse(
            errorResponse("Invalid product ID"),
            drogon::k400BadRequest));
        return;
    }

    auto body = request->getJsonObject();

    if (!body)
    {
        callback(jsonResponse(
            errorResponse("A valid JSON body is required"),
            drogon::k400BadRequest));
        return;
    }

    Product product{};

    if (!readProduct(*body, product, false))
    {
        callback(jsonResponse(
            errorResponse(
                "Required fields: name, seller_id, price_cents, stock_qty"),
            drogon::k400BadRequest));
        return;
    }

    product.id = id;

    Database database;

    if (database.getConnection() == nullptr)
    {
        callback(jsonResponse(
            errorResponse("Database connection failed"),
            drogon::k500InternalServerError));
        return;
    }

    ProductRepository repository(database.getConnection());
    ProductService service(repository);

    try
    {
        if (!service.updateProduct(product))
        {
            callback(jsonResponse(
                errorResponse("Product not found or update failed"),
                drogon::k404NotFound));
            return;
        }

        Json::Value response;
        response["success"] = true;
        response["data"] = productToJson(product);
        response["error"] = Json::nullValue;

        callback(jsonResponse(response, drogon::k200OK));
    }
    catch (const std::exception& e)
    {
        callback(jsonResponse(
            errorResponse(e.what()),
            drogon::k500InternalServerError));
    }
}

void ProductController::deleteProduct(
    const drogon::HttpRequestPtr& request,
    std::function<void(const drogon::HttpResponsePtr&)>&& callback,
    int id)
{
    if (id <= 0)
    {
        callback(jsonResponse(
            errorResponse("Invalid product ID"),
            drogon::k400BadRequest));
        return;
    }

    Database database;

    if (database.getConnection() == nullptr)
    {
        callback(jsonResponse(
            errorResponse("Database connection failed"),
            drogon::k500InternalServerError));
        return;
    }

    ProductRepository repository(database.getConnection());
    ProductService service(repository);

    try
    {
        if (!service.deleteProduct(id))
        {
            callback(jsonResponse(
                errorResponse("Product not found or delete failed"),
                drogon::k404NotFound));
            return;
        }

        Json::Value response;
        response["success"] = true;
        response["data"]["id"] = id;
        response["error"] = Json::nullValue;

        callback(jsonResponse(response, drogon::k200OK));
    }
    catch (const std::exception& e)
    {
        callback(jsonResponse(
            errorResponse(e.what()),
            drogon::k500InternalServerError));
    }
}

