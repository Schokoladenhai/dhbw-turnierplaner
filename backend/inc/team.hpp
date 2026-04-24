#pragma once

#include <string>
#include <vector>
#include "uuid.h"

class Team{
    private:
    uuids::uuid id;
    std::string name;
    std::vector<std::string> players;

    public:
    Team();

    uuids::uuid getId();
    void addPlayer(std::string name);
    void rmPlayer(std::string);
};
