#pragma once

#include "match.hpp"
#include <memory>
#include <vector>
#include <memory.h>

class Stage {

    using FinishedCallback = std::function<void(std::vector<Team*>)>;

    protected:
    std::vector<std::unique_ptr<Match>> matches;
    // Wird aufgerufen nach dem letzten Match mit if(onFinished){onFinished(getAdvancingTeams())}
    FinishedCallback onFinished;

    public:
    virtual ~Stage() = default;

    std::vector<Match*> getMatches() const;
    // Das Tournament setzt da etwas wie [this](std::vector<Team*> teams) {this->runNextStage(teams)}
    void setOnFinished(FinishedCallback cb);

    virtual void generateMatches(const std::vector<Team*>& participants) = 0;
    virtual std::vector<Team*> getAdvancingTeams() const = 0;
};
