#include "UserService.h"

UserService::UserService(UserRepository& repository)
    : repository(repository)
{
}

bool UserService::registerUser(const User& user)
{
    return repository.createUser(user);
}

User UserService::loginUser(const std::string& email)
{
    return repository.getUserByEmail(email);
}