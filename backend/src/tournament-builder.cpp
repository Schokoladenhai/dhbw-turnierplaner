#include "tournament-builder.hpp"
#include "tournament.hpp"
#include "stage.hpp"
#include "uuid.h"
#include "json.hpp"
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
using json = nlohmann::json;
TournamentBuilder& TournamentBuilder::loadJson(json tournamentJson){

    if(tournamentJson.contains("name")){
        this->setName(tournamentJson["name"]);
    }

    if(tournamentJson.contains("teams") && tournamentJson["teams"].is_array()){
        for(auto& teamJson : tournamentJson["teams"]){
            std::string name = teamJson["name"].get<std::string>();
            auto id = uuids::uuid::from_string(teamJson["id"].get<std::string>());
            auto team = std::make_unique<Team>(name, id.value());

            this->addTeam(std::move(team));
        }
    }

    return *this;
}

std::unique_ptr<Tournament> TournamentBuilder::build(){
    return std::make_unique<Tournament>(std::move(name), std::move(teams), std::move(stages));
}
