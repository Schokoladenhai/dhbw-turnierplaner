#include "team.hpp"
#include "uuid-gen.hpp"
#include "uuid.h"
#include <string>

Team::Team(std::string name){
    this->name = name;
    this->id = generateUuid();
}

uuids::uuid Team::getId() const {
    return id;
}

void Team::addPlayer(const std::string& name){
    players.push_back(name);
}

void Team::rmvPlayer(const std::string& name){
    std::erase(players, name);
}
