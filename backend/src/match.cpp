#include "match.hpp"
#include "uuid-gen.hpp"
#include "uuid.h"

Match::Match() : id(generateUuid()){}

void Match::setTeam1(uuids::uuid team1){
    if(status == MATCH_WAITING){
        team1Id = team1;
    }
}
void Match::setTeam2(uuids::uuid team2){
    if(status == MATCH_WAITING){
        team2Id = team2;
    }
}

uuids::uuid Match::getTeam1() const{
    return team1Id;
}
uuids::uuid Match::getTeam2() const{
    return team2Id;
}

void Match::addPointTeam1(){
    if(status == MATCH_RUNNING){
        score.points1 += 1;
    }
}
void Match::addPointTeam2(){
    if(status == MATCH_RUNNING){
        score.points2 += 1;
    }
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
        default:
    }
    return false;
}

uuids::uuid Match::getWinner() const{
    if(status == MATCH_FINISHED){
        if(score.points1 > score.points2) return team1Id;
        if(score.points2 > score.points1) return team2Id;
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
    if(!team1Id.is_nil() && !team2Id.is_nil()){
        return true;
    }
    return false;
}
