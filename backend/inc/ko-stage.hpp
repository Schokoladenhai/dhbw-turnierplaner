#pragma once

#include "match.hpp"
#include "stage.hpp"
#include "uuid.h"
#include <cstdint>
#include <unordered_map>
#include <vector>

struct MatchNode{
    Match* match;
    uint16_t Index;
};

class KoStage: public Stage{
    private:
    std::unordered_map<uuids::uuid, MatchNode> matchTree;

    public:
    void generateMatches(const std::vector<uuids::uuid>& teamsIds) override;
    std::vector<uuids::uuid> getAdvancingTeams() const override;
};
