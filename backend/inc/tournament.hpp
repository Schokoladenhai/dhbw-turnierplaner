#pragma once

#include "stage.hpp"
#include "uuid.h"
#include <memory>
#include <string>
#include <vector>
#include <memory.h>

enum TournamentStatus{
    TOURNAMENT_SETUP,
    TOURNAMENT_RUNNING,
    TOURNAMENT_FINISHED
};

class Tournament {
    private:
    TournamentStatus status = TOURNAMENT_SETUP;
    uint currentStageIndex = 0;
    std::string name;
    std::unordered_map<uuids::uuid, std::unique_ptr<Team>> teams;
    std::vector<std::unique_ptr<Stage>> stages;

    public:
    Tournament();
    // Übergibt auch [this](std::vector<Team*> teams) {this->runNextStage(teams)} als setOnFinished() und verschiebt den owner mit std::move zum Tournament
    void addStage(std::unique_ptr<Stage> stage);
    void rmStage(Stage*);
    void addTeam(std::unique_ptr<Team> team);
    void rmStage(uuids::uuid teamId);
    // Aufgerufen durch [this](std::vector<Team*> teams) {this->runNextStage(teams)}
    void runNextStage(std::vector<Team*> teams);
    void reload();

    void start();
    void end();

    void saveToJson();
    void loadFromJson();
};
