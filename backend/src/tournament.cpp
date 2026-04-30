#include "tournament.hpp"
#include "stage.hpp"
#include "team.hpp"
#include "uuid.h"
#include <string>
#include <memory>
#include <vector>
#include <utility>

Tournament::Tournament(){}
void Tournament::updateName(std::string newName){
    name = newName;
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

void Tournament::start(){}
void Tournament::runNextStage(std::vector<uuids::uuid> teamIds){
    if(currentStageIndex < stages.size() - 1){
        ++currentStageIndex;
        Stage* nextStage = stages[currentStageIndex].get();
        nextStage->populateMatches(teamIds);
    }else {
        end(teamIds);
    }
}
void Tournament::end(std::vector<uuids::uuid> teamIds){}

void Tournament::saveToJson(){}
void Tournament::loadFromJson(){}
