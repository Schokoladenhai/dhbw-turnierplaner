#pragma once

#include "stage.hpp"
#include <memory>
#include <string>
#include <vector>
#include <memory.h>

class Tournament {
    public:
    std::string name;
    std::vector<std::unique_ptr<Team>> teams;
    std::vector<std::unique_ptr<Stage>> stages;
    uint currentStageIndex = 0;

    // Übergibt auch [this](std::vector<Team*> teams) {this->runNextStage(teams)} als setOnFinished() und verschiebt den owner mit std::move zum Tournament
    void addStage(std::unique_ptr<Stage> stage);
    void addTeam(std::unique_ptr<Team> team);
    void start();
    // Aufgerufen durch [this](std::vector<Team*> teams) {this->runNextStage(teams)}
    void runNextStage(std::vector<Team*> teams);
    // Wird am ende Aufgerufen
    void end();

    void saveToJson();
    void loadFromJson();
};
