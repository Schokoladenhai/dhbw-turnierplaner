#pragma once

#include "stage.hpp"
#include "match.hpp"
#include "uuid.h"
#include <vector>

class KoStage: public Stage{
    private:
    std::vector<uuids::uuid> matchTree;

    public:
    void matchUpdate(const uuids::uuid currentMatch, const Score& newScore, MatchStatus newStatus) override;
    bool isValidMatchResult(const Score& score) const override;
    void generateMatches(const std::vector<uuids::uuid>& teamsIds) override;
    std::vector<uuids::uuid> getAdvancingTeams() const override;
};
