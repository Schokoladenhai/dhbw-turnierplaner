#pragma once

#include "team.hpp"

struct Score{
    int points1 = 0;
    int points2 = 0;
};

enum MatchStatus{
    WAITING,
    RUNNING,
    FINISHED
};

class Match{
    public:
    Team* team1;
    Team* team2;
    Score score;
    MatchStatus status;

    Match();

    void setResults(int points1, int points2);
    Team* getWinner() const;
    bool isready() const;
};
