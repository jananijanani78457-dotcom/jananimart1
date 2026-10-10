#pragma once

#include <libpq-fe.h>

class Database
{
public:
    Database();
    ~Database();

    PGconn* getConnection();

private:
    PGconn* conn;
};