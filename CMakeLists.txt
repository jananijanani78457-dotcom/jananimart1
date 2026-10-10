cmake_minimum_required(VERSION 3.25)

project(JananiMart LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(PostgreSQL REQUIRED)
find_package(Drogon CONFIG REQUIRED)

add_executable(jananimart

    src/main.cpp

    src/database.cpp

    src/controller/ProductController.cpp

    src/repository/ProductRepository.cpp
    src/repository/UserRepository.cpp
    src/repository/OrderRepository.cpp
    src/repository/OrderItemRepository.cpp
    src/repository/CartRepository.cpp

    src/service/ProductService.cpp
    src/service/UserService.cpp
    src/service/OrderService.cpp
    src/service/OrderItemService.cpp
    src/service/CartService.cpp

)

target_link_libraries(jananimart
    PRIVATE
    PostgreSQL::PostgreSQL
    Drogon::Drogon
)