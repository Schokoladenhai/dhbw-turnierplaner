#include "tournament.hpp"
#include "stage.hpp"
#include "team.hpp"
#include "uuid.h"
#include "json.hpp"
#include <string>
#include <memory>
#include <vector>
#include <utility>
#include <unordered_map>

Tournament::Tournament(std::string name, std::unordered_map<uuids::uuid, std::unique_ptr<Team>> teams, std::vector<std::unique_ptr<Stage>> stages)
    : name(std::move(name)), teams(std::move(teams)), stages(std::move(stages)), status(TOURNAMENT_READY), currentStageIndex(0)
{
    for(auto& stage : this->stages){
        if(stage){
            stage->setOnFinished([this](std::vector<uuids::uuid> teamsList){
                this->runNextStage(teamsList);
            });
        }
    }
}

TournamentStatus Tournament::getStatus(){
    return status;
}

Stage* Tournament::getCurrentStage() const{
    if(currentStageIndex >= stages.size()){
        return nullptr;
    }
    return stages[currentStageIndex].get();
}

bool Tournament::start(){
    if(status != TOURNAMENT_READY){
        return false;
    }
    std::vector<uuids::uuid> teamIds;
    teamIds.reserve(teams.size());

    for(auto const& [uuid, teamPtr] : teams){
        teamIds.push_back(uuid);
    }

    Stage* nextStage = getCurrentStage();
    nextStage->populateMatches(teamIds);

    status = TOURNAMENT_RUNNING;

    return true;
}
void Tournament::runNextStage(std::vector<uuids::uuid> teamIds){
    if(currentStageIndex < stages.size() - 1){
        ++currentStageIndex;
        Stage* nextStage = getCurrentStage();
        nextStage->populateMatches(teamIds);
    }else {
        end(teamIds);
    }
}
void Tournament::end(std::vector<uuids::uuid> teamIds){
    status = TOURNAMENT_FINISHED;
}

using json = nlohmann::json;
json Tournament::toJson()const{
    json j;
    j["name"] = name;
    j["status"] = status;
    j["stageindex"] = currentStageIndex;

    j["stages"] = json::array();
    size_t index = 0;
    for(auto const& stageUPtr : stages){
        Stage* stage = stageUPtr.get();
        if(stage != nullptr){
            json stageJson = stage->toJson();
            stageJson["index"] = index;
            j["stages"].push_back(stageJson);
        }
        ++index;
    }

    j["teams"] = json::array();
    for(auto const& [uuid, teamUPtr] : teams){
        Team* team = teamUPtr.get();
        if(team != nullptr){
            json teamJson = team->toJson();
            j["teams"].push_back(teamJson);
        }
    }

    return j;
}
