#pragma once

#include <string>

struct Order
{
    int id;
    int buyer_id;
    std::string status;
    long long total_amount_cents;
};