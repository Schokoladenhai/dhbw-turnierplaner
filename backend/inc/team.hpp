#pragma once

#include <string>
#include <vector>
#include "uuid.h"

class Team{
    public:
    uuids::uuid id;
    std::string name;
    std::vector<std::string> players;
};
