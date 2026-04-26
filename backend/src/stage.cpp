#include "stage.hpp"
#include "match.hpp"
#include "uuid.h"
#include <utility>

void Stage::setOnFinished(Stage::FinishedCallback cb){
    onFinished = std::move(cb);
}

bool Stage::canAcceptMatchUpdate(const uuids::uuid currentMatch, const Score& newScore, MatchStatus newStatus) const{
    auto it = matches.find(currentMatch);
    if(it != matches.end()){
        Match* current = it->second.get();
        if (current->getStatus() == MATCH_FINISHED) {
            return false;
        }

        if (newStatus == MATCH_FINISHED) {
            return isValidMatchResult(newScore);
        }
    }

    return true;
}
