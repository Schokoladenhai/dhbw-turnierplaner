#include "tournament.hpp"
#include "stage.hpp"
#include "team.hpp"
#include "uuid.h"
#include "json.hpp"
#include <string>
#include <memory>
#include <vector>
#include <utility>

Tournament::Tournament(){}
void Tournament::updateName(std::string newName){
    name = newName;
}
TournamentStatus Tournament::getStatus(){
    return status;
}
void Tournament::pushStage(std::unique_ptr<Stage> stage){
    if(stage){
        stage->setOnFinished([this](std::vector<uuids::uuid> teams){this->runNextStage(teams);});
        stages.push_back(std::move(stage));
    }
}
void Tournament::popStage(){
    stages.pop_back();
}
void Tournament::addTeam(std::unique_ptr<Team> team){
    if(team){
        uuids::uuid id = team->getId();
        teams.emplace(id, std::move(team));
    }
}
void Tournament::rmvTeam(uuids::uuid teamId){
    if(!teamId.is_nil()){
        teams.erase(teamId);
    }
}

Stage* Tournament::getCurrentStage() const{
    if(currentStageIndex >= stages.size()){
        return nullptr;
    }
    return stages[currentStageIndex].get();
}

bool Tournament::start(){
    if(stages.empty() || teams.empty()){
        return false;
    }
    std::vector<uuids::uuid> teamIds;
    teamIds.reserve(teams.size());

    for(auto const& [uuid, teamPtr] : teams){
        teamIds.push_back(uuid);
    }

    Stage* nextStage = getCurrentStage();
    nextStage->populateMatches(teamIds);

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
void Tournament::end(std::vector<uuids::uuid> teamIds){}

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
            j["matches"].push_back(teamJson);
        }
    }

    return j;
}
void Tournament::loadFromJson(){}
