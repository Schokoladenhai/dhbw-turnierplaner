#pragma once

#include "match.hpp"
#include "uuid.h"
#include <functional>
#include <memory.h>
#include <vector>

class Stage {

    using FinishedCallback = std::function<void(std::vector<Team*>)>;
    using ReloadCallback = std::function<void()>;

    protected:
    std::unordered_map<uuids::uuid, std::unique_ptr<Match>> matches;
    // Wird aufgerufen nach dem letzten Match mit if(onFinished){onFinished(getAdvancingTeams())}
    FinishedCallback onFinished;
    ReloadCallback onReload;

    public:
    virtual ~Stage() = default;

    std::unordered_map<uuids::uuid, Match*> getMatches() const;
    // Das Tournament setzt da etwas wie [this](std::vector<Team*> teams) {this->runNextStage(teams)}
    void setOnFinished(FinishedCallback cb);
    void setOnReload(ReloadCallback cb);

    virtual void generateMatches(const std::vector<Team*>& participants) = 0;
    virtual std::vector<Team*> getAdvancingTeams() const = 0;
};
