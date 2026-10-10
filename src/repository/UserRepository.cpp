#include "UserRepository.h"
#include <iostream>

UserRepository::UserRepository(PGconn* connection)
{
    conn = connection;
}

bool UserRepository::createUser(const User& user)
{
    if (conn == nullptr)
    {
        std::cout << "Database connection is not available."
                  << std::endl;
        return false;
    }

    const char* query =
        "INSERT INTO users "
        "(name, email, password_hash, role) "
        "VALUES ($1, $2, $3, $4);";

    const char* values[] =
    {
        user.name.c_str(),
        user.email.c_str(),
        user.password_hash.c_str(),
        user.role.c_str()
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
        std::cout << "Failed to create user: "
                  << PQerrorMessage(conn)
                  << std::endl;

        PQclear(result);
        return false;
    }

    PQclear(result);

    std::cout << "User created successfully!"
              << std::endl;

    return true;
}

User UserRepository::getUserByEmail(
    const std::string& email
)
{
    User user{};

    if (conn == nullptr)
    {
        return user;
    }

    const char* query =
        "SELECT id, name, email, password_hash, role "
        "FROM users "
        "WHERE email = $1;";

    const char* values[] =
    {
        email.c_str()
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
        std::cout << "Failed to find user: "
                  << PQerrorMessage(conn)
                  << std::endl;

        PQclear(result);
        return user;
    }

    if (PQntuples(result) > 0)
    {
        user.id = std::stoi(PQgetvalue(result, 0, 0));
        user.name = PQgetvalue(result, 0, 1);
        user.email = PQgetvalue(result, 0, 2);
        user.password_hash = PQgetvalue(result, 0, 3);
        user.role = PQgetvalue(result, 0, 4);
    }

    PQclear(result);

    return user;
}