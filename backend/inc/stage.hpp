#pragma once

#include "match.hpp"
#include "uuid.h"
#include <cstddef>
#include <functional>
#include <unordered_map>
#include <memory>
#include <vector>

class Stage {

    using FinishedCallback = std::function<void(std::vector<uuids::uuid>)>;

    protected:
    std::unordered_map<uuids::uuid, std::unique_ptr<Match>> matches{};
    // Wird aufgerufen nach dem letzten Match mit if(onFinished){onFinished(getAdvancingTeams())}
    FinishedCallback onFinished = NULL;;

    public:
    virtual ~Stage() = default;

    // Das Tournament setzt da etwas wie [this](std::vector<Team*> teams) {this->runNextStage(teams)}
    void setOnFinished(FinishedCallback cb);
    virtual bool canAcceptMatchUpdate(const uuids::uuid currentMatch, const Score& newScore, const MatchStatus newStatus) const;
    virtual bool matchUpdate(const uuids::uuid currentMatch, const Score& newScore, const MatchStatus newStatus);

    virtual bool isValidMatchResult(const Score& score) const;
    virtual void generateMatches(const int totalteams, const int totalMatches) = 0;
    virtual void populateMatches(const std::vector<uuids::uuid>& teamIds) = 0;
    virtual std::vector<uuids::uuid> getAdvancingTeams() const = 0;
};
