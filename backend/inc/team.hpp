#pragma once

#include <string>
#include <vector>
#include "uuid.h"
#include "uuid-gen.hpp"
#include "json.hpp"

class Team{
    private:
    const uuids::uuid id;
    std::string name;
    std::vector<std::string> players;

    public:
    Team(std::string name, uuids::uuid uuid = generateUuid());

    uuids::uuid getId() const;
    void addPlayer(const std::string& name);
    void rmvPlayer(const std::string& name);

    using json = nlohmann::json;
    json toJson() const;
};
