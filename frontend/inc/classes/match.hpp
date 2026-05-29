#ifndef MATCH_H
#define MATCH_H

#include <string>
#include <vector>
#include <algorithm>
#include "uuid.h"
#include "team.hpp"
class Match {
public:
    std::string name;
    uuids::uuid id;
    Team team1;
    Team team2;
    std::array<uint8_t, 2> score; // score[0] für team1, score[1] für team2
    int stage; // Runde, in der das Match stattfindet
    std::string status = "WAITING";

public:
    // Konstruktor
    Match(std::string matchName, uuids::uuid matchId);

    // Getter
    uuids::uuid getId() const;
    std::string getName() const;
    const std::vector<std::string>& getTeams() const;
    void setScore(uint8_t team1Score, uint8_t team2Score);


    // Methoden gemäß UML
    void addTeam(const Team team);
    void rmvTeam(const Team team);
};


enum MatchStatus{
    MATCH_WAITING,  ///< Match wartet auf Teams oder den Start.
    MATCH_RUNNING,  ///< Match läuft aktuell.
    MATCH_FINISHED, ///< Match ist beendet und hat ein Ergebnis.
    MATCH_SKIPED    ///< Match wurde übersprungen (z.B. durch ein Freilos/Bye).
};
#endif // TEAM_H