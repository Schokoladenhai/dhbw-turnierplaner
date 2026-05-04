#include "team.hpp"
#include "uuid-gen.hpp"
#include "uuid.h"
#include <string>
#include "json.hpp"

Team::Team(std::string name) : name(name), id(generateUuid()){}

uuids::uuid Team::getId() const {
    return id;
}

void Team::addPlayer(const std::string& name){
    players.push_back(name);
}

void Team::rmvPlayer(const std::string& name){
    std::erase(players, name);
}

using json = nlohmann::json;
json Team::toJson() const{
    json j;
    j["id"] = uuids::to_string(id);
    j["name"] = name;
    j["players"] = players;

    return j;
}
