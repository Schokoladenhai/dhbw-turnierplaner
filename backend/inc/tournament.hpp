#pragma once

#include "stage.hpp"
#include "uuid.h"
#include "team.hpp"
#include <memory>
#include <unordered_map>
#include <string>
#include <vector>
#include <memory>

enum TournamentStatus{
    TOURNAMENT_SETUP,
    TOURNAMENT_RUNNING,
    TOURNAMENT_FINISHED
};

class Tournament {
    private:
    TournamentStatus status = TOURNAMENT_SETUP;
    unsigned int currentStageIndex = 0;
    std::string name = "Tournament";
    std::unordered_map<uuids::uuid, std::unique_ptr<Team>> teams{};
    std::vector<std::unique_ptr<Stage>> stages{};

    public:
    Tournament();
    void updateName(std::string newName);
    // Übergibt auch [this](std::vector<uuids::uuid> teams) {this->runNextStage(teams)} als setOnFinished() und verschiebt den owner mit std::move zum Tournament
    void pushStage(std::unique_ptr<Stage> stage);
    void popStage();
    void addTeam(std::unique_ptr<Team> team);
    void rmvTeam(uuids::uuid teamId);
    Stage* getCurrentStage() const;

    void start();
    // Aufgerufen durch [this](std::vector<uuids::uuid> teams) {this->runNextStage(teams)}
    void runNextStage(std::vector<uuids::uuid> teamIds);
    void end(std::vector<uuids::uuid> teamIds);

    void saveToJson();
    void loadFromJson();
};
