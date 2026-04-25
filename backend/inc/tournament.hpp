#pragma once

#include "stage.hpp"
#include "uuid.h"
#include "team.hpp"
#include <memory>
#include <unordered_map>
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
    unsigned int currentStageIndex = 0;
    std::string name = "Tournament";
    std::unordered_map<uuids::uuid, std::unique_ptr<Team>> teams;
    std::vector<std::unique_ptr<Stage>> stages;

    // Aufgerufen durch [this](std::vector<Team*> teams) {this->runNextStage(teams)}
    void runNextStage(std::vector<Team*> teams);

    public:
    Tournament();
    void updateName(std::string newName);
    // Übergibt auch [this](std::vector<Team*> teams) {this->runNextStage(teams)} als setOnFinished() und verschiebt den owner mit std::move zum Tournament
    void addStage(std::unique_ptr<Stage> stage);
    void rmvStage(Stage*);
    void addTeam(std::unique_ptr<Team> team);
    void rmvTeam(uuids::uuid teamId);

    void start();
    void end();

    void saveToJson();
    void loadFromJson();
};
