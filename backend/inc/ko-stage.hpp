#pragma once

#include "stage.hpp"
#include "match.hpp"
#include "uuid.h"
#include "json.hpp"
#include <vector>

class KoStage: public Stage{
    private:
    std::vector<uuids::uuid> matchTree{};

    public:
    bool matchUpdate(const uuids::uuid currentMatch, const Score& newScore, const MatchStatus newStatus) override;
    bool isValidMatchResult(const Score& score) const override;
    void generateMatches(const int totalTeams, const int totalMatches) override;
    void populateMatches(const std::vector<uuids::uuid>& teamIds) override;
    std::vector<uuids::uuid> getAdvancingTeams() const override;

    using json = nlohmann::json;
    json toJson() const override;
};
