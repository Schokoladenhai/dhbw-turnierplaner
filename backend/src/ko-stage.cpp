#include "ko-stage.hpp"
#include "match.hpp"
#include <iterator>
#include <memory>
#include "uuid.h"
#include <vector>

bool KoStage::matchUpdate(const uuids::uuid currentMatch, const Score& newScore, const MatchStatus newStatus){
    if(!canAcceptMatchUpdate(currentMatch, newScore, newStatus)){
        return false;
    }
    auto it = matches.find(currentMatch);
    if(it != matches.end()){
        Match* match = it->second.get();
        match->setnewScore(newScore);

        if(newStatus != match->getStatus()){
            match->advanceStatus();
        }

        if(newStatus == MATCH_FINISHED){
            uuids::uuid winner = match->getWinner();
            if(winner.is_nil()){
                return false;
            }
            auto itTree = std::find(matchTree.begin(), matchTree.end(), currentMatch);
            if(itTree != matchTree.end()){
                int oldIndex = std::distance(matchTree.begin(), itTree) + 1;
                int newIndex = (oldIndex >> 1) - 1;
                if(newIndex == 0){
                    auto winner = getAdvancingTeams();
                    if(winner[0].is_nil()){
                        return false;
                    }
                    if(onFinished){
                        onFinished(winner);
                    }
                    return true;
                }
                auto itNewMatch = matches.find(matchTree[newIndex]);
                if(it != matches.end()){
                    Match* newMatch = it->second.get();
                    if(newMatch->getTeam1().is_nil()){
                        newMatch->setTeam1(winner);
                    }else{
                        newMatch->setTeam2(winner);
                    }
                }
            }
        }
    }

    return true;
}

bool KoStage::isValidMatchResult(const Score& score) const{
    return score.points1 != score.points2;
}

void KoStage::generateMatches(const int totalTeams, const int totalMatches) {
    matches.reserve(totalMatches);
    matchTree.reserve(totalTeams);

    for (int i = 0; i < totalMatches; ++i) {
        auto new_match = std::make_unique<Match>();
        uuids::uuid id = new_match->getId();
        matches.emplace(id, std::move(new_match));
        matchTree.push_back(id);
    }
}

void KoStage::populateMatches(const std::vector<uuids::uuid>& teamIds) {

}

std::vector<uuids::uuid> KoStage::getAdvancingTeams() const {
    std::vector<uuids::uuid> winner{};
    winner.reserve(1);
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
