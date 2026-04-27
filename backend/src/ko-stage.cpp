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

        if(newStatus == MATCH_SKIPED){
            match->skip();
        }
        if(newStatus != match->getStatus()){
            match->advanceStatus();
        }

        if(newStatus == MATCH_FINISHED || newStatus == MATCH_SKIPED){
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
                if(itNewMatch != matches.end()){
                    Match* newMatch = itNewMatch->second.get();
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
    matchTree.reserve(totalMatches);

    for (int i = 0; i < totalMatches; ++i) {
        auto new_match = std::make_unique<Match>();
        uuids::uuid id = new_match->getId();
        matches.emplace(id, std::move(new_match));
        matchTree.push_back(id);
    }
}

void KoStage::populateMatches(const std::vector<uuids::uuid>& teamIds) {
    if (teamIds.empty() || matchTree.empty()) return;

    int totalTeams = teamIds.size();
    int numFirstRoundMatches = (totalTeams + 1) / 2;

    int firstRoundStartIndex = matchTree.size() - numFirstRoundMatches;
    for (int i = 0; i < numFirstRoundMatches; ++i) {
        uuids::uuid matchId = matchTree[firstRoundStartIndex + i];
        auto it = matches.find(matchId);

        if (it != matches.end()) {
            Match* match = it->second.get();

            match->setTeam1(teamIds[i * 2]);

            if ((i * 2 + 1) < totalTeams) {
                match->setTeam2(teamIds[i * 2 + 1]);
            }else{
                matchUpdate(match->getId(), {}, MATCH_SKIPED);
            }
        }
    }
}

std::vector<uuids::uuid> KoStage::getAdvancingTeams() const {
    std::vector<uuids::uuid> winner{uuids::uuid{}};
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
