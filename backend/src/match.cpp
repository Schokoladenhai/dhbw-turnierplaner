#include "match.hpp"
#include "uuid-gen.hpp"
#include "json.hpp"
#include "uuid.h"
#include <string>

Match::Match() : id(generateUuid()){}

void Match::setTeam1(uuids::uuid team1Id){
    if(status == MATCH_WAITING){
        this->team1Id = team1Id;
    }
}
void Match::setTeam2(uuids::uuid team2Id){
    if(status == MATCH_WAITING){
        this->team2Id = team2Id;
    }
}

uuids::uuid Match::getTeam1() const{
    return team1Id;
}
uuids::uuid Match::getTeam2() const{
    return team2Id;
}

void Match::setnewScore(const Score& newScore){
    score = newScore;
}

bool Match::advanceStatus(){
    switch (status) {
        case MATCH_WAITING:
            if(isReady()){
                status = MATCH_RUNNING;
                return true;
            }
            break;
        case MATCH_RUNNING:
            status = MATCH_FINISHED;
            return true;
            break;
        case MATCH_FINISHED:
        case MATCH_SKIPED:
        default:
        break;
    }
    return false;
}

void Match::skip(){
    status = MATCH_SKIPED;
}

uuids::uuid Match::getWinner() const{
    if(status == MATCH_FINISHED){
        if(score.points1 > score.points2) return team1Id;
        if(score.points2 > score.points1) return team2Id;
    }
    if(status == MATCH_SKIPED){
        if(team2Id.is_nil()) return team1Id;
        if(team1Id.is_nil()) return team2Id;
    }
    return uuids::uuid{};
}

uuids::uuid Match::getId() const{
    return id;
}

MatchStatus Match::getStatus() const{
    return status;
}

bool Match::isReady() const{
    if(team1Id.is_nil() || team2Id.is_nil()){
        return false;
    }
    return true;
}

using json = nlohmann::json;
json Match::toJson() const {
    json j;
    std::string t1 = uuids::to_string(getTeam1());
    std::string t2 = uuids::to_string(getTeam2());

    j["id"] = uuids::to_string(getId());
    j["status"] = status;
    j["team1"] = t1;
    j["team2"] = t2;

    j["score"][t1] = score.points1;
    j["score"][t2] = score.points2;

    return j;
}
