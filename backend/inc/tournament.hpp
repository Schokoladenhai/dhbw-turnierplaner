#pragma once

#include "stage.hpp"
#include "uuid.h"
#include "team.hpp"
#include "json.hpp"
#include <memory>
#include <unordered_map>
#include <string>
#include <vector>

enum TournamentStatus{
    TOURNAMENT_READY,
    TOURNAMENT_RUNNING,
    TOURNAMENT_FINISHED
};

NLOHMANN_JSON_SERIALIZE_ENUM(TournamentStatus, {
    {TOURNAMENT_READY, "READY"},
    {TOURNAMENT_RUNNING, "RUNNING"},
    {TOURNAMENT_FINISHED, "FINISHED"}
})

class Tournament {
    private:
    TournamentStatus status = TOURNAMENT_READY;
    unsigned int currentStageIndex = 0;
    std::string name;
    std::unordered_map<uuids::uuid, std::unique_ptr<Team>> teams;
    std::vector<std::unique_ptr<Stage>> stages;

    public:
    Tournament(std::string name, std::unordered_map<uuids::uuid, std::unique_ptr<Team>> teams, std::vector<std::unique_ptr<Stage>> stages);

    TournamentStatus getStatus();
    Stage* getCurrentStage() const;
    bool start();
    // Aufgerufen durch [this](std::vector<uuids::uuid> teams) {this->runNextStage(teams)}
    void runNextStage(std::vector<uuids::uuid> teamIds);
    void end(std::vector<uuids::uuid> teamIds);

    using json = nlohmann::json;
    json toJson() const;
};
