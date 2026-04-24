#pragma once

#include "match.hpp"
#include "stage.hpp"
#include "uuid.h"
#include <cstdint>
#include <memory>
#include <unordered_map>
#include <vector>

struct MatchNode{
    Match* match;
    uint16_t Index;
};

class KoStage: public Stage{
    private:
    std::unordered_map<uuids::uuid, std::unique_ptr<MatchNode>> matchTree;

    public:
    void generateMatches(const std::vector<Team*>& participants) override;
    std::vector<Team*> getAdvancingTeams() const override;
};
