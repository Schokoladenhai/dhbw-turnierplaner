#include "../../inc/classes/team.hpp"

Team::Team(std::string teamName, uuids::uuid teamId) : name(teamName), id(teamId) {}

uuids::uuid Team::getId() const {
    return id;
}

std::string Team::getName() const {
    return name;
}

const std::vector<std::string>& Team::getPlayers() const {
    return players;
}

void Team::addPlayer(const std::string& playerName) {
    if (std::find(players.begin(), players.end(), playerName) == players.end()) {
        players.push_back(playerName);
    }
}

void Team::rmvPlayer(const std::string& playerName) {
    auto it = std::find(players.begin(), players.end(), playerName);
    if (it != players.end()) {
        players.erase(it);
    }
}


