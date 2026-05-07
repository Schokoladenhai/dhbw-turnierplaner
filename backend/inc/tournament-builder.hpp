#pragma once

#include "tournament.hpp"
#include "stage.hpp"
#include "uuid.h"
#include "team.hpp"
#include <memory>
#include <unordered_map>
#include <string>
#include <vector>
#include <memory>

class TournamentBuilder {
    private:
    std::string name = "Tournament";
    std::unordered_map<uuids::uuid, std::unique_ptr<Team>> teams{};
    std::vector<std::unique_ptr<Stage>> stages{};

    public:
    TournamentBuilder& setName(std::string newName);
    TournamentBuilder& addTeam(std::unique_ptr<Team> newTeam);
    TournamentBuilder& addTeams(std::vector<std::unique_ptr<Team>> newTeams);
    TournamentBuilder& addStage(std::unique_ptr<Stage> newStage);
    TournamentBuilder& addStages(std::vector<std::unique_ptr<Stage>> newStages);
    TournamentBuilder& loadJson();

    std::unique_ptr<Tournament> build();

};
