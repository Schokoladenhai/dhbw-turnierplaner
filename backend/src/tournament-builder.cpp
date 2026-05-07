#include "tournament-builder.hpp"
#include "tournament.hpp"
#include "stage.hpp"
#include "uuid.h"
#include "team.hpp"
#include <algorithm>
#include <memory>
#include <unordered_map>
#include <string>
#include <vector>
#include <utility>
#include <iterator>

TournamentBuilder& TournamentBuilder::setName(std::string newName){
    name = std::move(newName);
    return *this;
}
TournamentBuilder& TournamentBuilder::addTeam(std::unique_ptr<Team> newTeam){
    uuids::uuid id = newTeam->getId();
    teams.emplace(id, std::move(newTeam));
    return *this;
}
TournamentBuilder& TournamentBuilder::addTeams(std::vector<std::unique_ptr<Team>> newTeams){
    for(auto& team : newTeams){
        addTeam(std::move(team));
    }
    return *this;
}
TournamentBuilder& TournamentBuilder::addStage(std::unique_ptr<Stage> newStage){
    stages.push_back(std::move(newStage));
    return *this;
}
TournamentBuilder& TournamentBuilder::addStages(std::vector<std::unique_ptr<Stage>> newStages){
    std::move(newStages.begin(), newStages.end(), std::back_inserter(stages));
    return *this;
}
TournamentBuilder& TournamentBuilder::loadJson(){
    return *this;
}

std::unique_ptr<Tournament> TournamentBuilder::build(){
    return std::make_unique<Tournament>(std::move(name), std::move(teams), std::move(stages));
}
