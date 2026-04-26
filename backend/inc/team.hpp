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
    Team(std::string name);

    uuids::uuid getId() const;
    void addPlayer(const std::string& name);
    void rmvPlayer(const std::string& name);
};
