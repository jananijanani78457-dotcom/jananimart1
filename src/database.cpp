#include "database.h"
#include <iostream>

Database::Database()
{
    conn = PQconnectdb(
        "host=localhost "
        "port=5432 "
        "dbname=janani_mart "
        "user=postgres "
        "password=janani@2006"
    );

    if (PQstatus(conn) == CONNECTION_OK)
    {
        std::cout << "PostgreSQL connected successfully!" << std::endl;
    }
    else
    {
        std::cout << "PostgreSQL connection failed: "
                  << PQerrorMessage(conn) << std::endl;
    }
}

Database::~Database()
{
    if (conn != nullptr)
    {
        PQfinish(conn);
    }
}

PGconn* Database::getConnection()
{
    return conn;
}