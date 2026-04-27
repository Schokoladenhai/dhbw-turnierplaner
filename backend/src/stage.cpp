#include "stage.hpp"
#include "match.hpp"
#include "uuid.h"
#include <utility>

void Stage::setOnFinished(Stage::FinishedCallback cb){
    onFinished = std::move(cb);
}

bool Stage::canAcceptMatchUpdate(const uuids::uuid currentMatch, const Score& newScore, const MatchStatus newStatus) const{
    auto it = matches.find(currentMatch);
    if(it != matches.end()){
        Match* match = it->second.get();
        MatchStatus matchStatus = match->getStatus();
        if (matchStatus == MATCH_FINISHED) {
            return false;
        }

        if (newStatus == MATCH_FINISHED && matchStatus == MATCH_RUNNING) {
            return isValidMatchResult(newScore);
        }

        if(matchStatus == MATCH_WAITING && newStatus == MATCH_RUNNING){
            return match->isReady();
        }
    }

    return true;
}
