#pragma once

#include "match.hpp"
#include "uuid.h"
#include <functional>
#include <unordered_map>
#include <memory>
#include <vector>

class Stage {

    using FinishedCallback = std::function<void(std::vector<uuids::uuid>)>;

    protected:
    std::unordered_map<uuids::uuid, std::unique_ptr<Match>> matches;
    // Wird aufgerufen nach dem letzten Match mit if(onFinished){onFinished(getAdvancingTeams())}
    FinishedCallback onFinished;

    public:
    virtual ~Stage() = default;

    // Das Tournament setzt da etwas wie [this](std::vector<Team*> teams) {this->runNextStage(teams)}
    void setOnFinished(FinishedCallback cb);
    virtual bool canAcceptMatchUpdate(const uuids::uuid currentMatch, const Score& newScore, MatchStatus newStatus) const;

    virtual bool isValidMatchResult(const Score& score) const;
    virtual void generateMatches(const std::vector<uuids::uuid>& teamIds) = 0;
    virtual std::vector<uuids::uuid> getAdvancingTeams() const = 0;
};
