#pragma once

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
    const uuids::uuid id;
    MatchStatus status = MATCH_WAITING;
    uuids::uuid team1Id{};
    uuids::uuid team2Id{};
    Score score{};

    public:
    Match();

    void setTeam1(uuids::uuid team1Id);
    void setTeam2(uuids::uuid team2Id);
    uuids::uuid getTeam1() const;
    uuids::uuid getTeam2() const;
    void setnewScore(Score& newScore);
    bool advanceStatus();
    uuids::uuid getWinner() const;
    uuids::uuid getId() const;
    MatchStatus getStatus() const;
    bool isReady() const;
};
