#include "ko-stage.hpp"
#include "match.hpp"
#include <memory>
#include "uuid.h"
#include <vector>

void KoStage::matchUpdate(const uuids::uuid currentMatch, const Score& newScore, MatchStatus newStatus){

}

bool KoStage::isValidMatchResult(const Score& score) const{
    return score.points1 != score.points2;
}

void KoStage::generateMatches(const std::vector<uuids::uuid>& teamsIds) {

}

std::vector<uuids::uuid> KoStage::getAdvancingTeams() const {
    std::vector<uuids::uuid> winner{};
    if(matchTree.empty() || matches.empty()){
        return winner;
    }
    auto it = matches.find(matchTree.front());
    if(it != matches.end()){
        Match* final = it->second.get();
        if(final->getStatus() == MATCH_FINISHED){
            winner.push_back(final->getWinner());
        }
    }
    return winner;
}
