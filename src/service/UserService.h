#pragma once

#include "../model/User.h"
#include "../repository/UserRepository.h"

class UserService
{
public:
    UserService(UserRepository& repository);

    bool registerUser(const User& user);

    User loginUser(const std::string& email);

private:
    UserRepository& repository;
};