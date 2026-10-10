#pragma once

#include "../model/user.h"
#include <libpq-fe.h>

class UserRepository
{
public:
    UserRepository(PGconn* connection);

    bool createUser(const User& user);

    User getUserByEmail(const std::string& email);

private:
    PGconn* conn;
};