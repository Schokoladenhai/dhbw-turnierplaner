#pragma once

#include "team.hpp"
#include "uuid.h"

struct Score{
    int points1 = 0;
    int points2 = 0;
};

enum MatchStatus{
    MATCH_WAITING,
    MATCH_RUNNING,
    MATCH_FINISHED
};

class Match{
    private:
    uuids::uuid id;
    MatchStatus status = MATCH_WAITING;
    Team* team1;
    Team* team2;
    Score score;

    public:
    Match();

    void setResults(int points1, int points2);
    Team* getWinner() const;
    uuids::uuid getId() const;
    MatchStatus getStatus() const;
    bool isready() const;
};
