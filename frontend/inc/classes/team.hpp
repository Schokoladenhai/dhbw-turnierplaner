#ifndef TEAM_H
#define TEAM_H

#include <string>
#include <vector>
#include <algorithm>
#include "uuid.h"

class Team {
private:
    std::string name;
    uuids::uuid id;
    std::vector<std::string> players;

public:
    // Konstruktor
    Team(std::string teamName, uuids::uuid teamId);

    // Getter
    uuids::uuid getId() const;
    std::string getName() const;
    const std::vector<std::string>& getPlayers() const;

    // Methoden gemäß UML
    void addPlayer(const std::string& playerName);
    void rmvPlayer(const std::string& playerName);
};

#endif // TEAM_H