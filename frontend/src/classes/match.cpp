#include "../../inc/classes/match.hpp"


Match::Match(std::string matchName, uuids::uuid matchId) :
    name(matchName),id(matchId){}

uuids::uuid Match::getId() const {return id;}
std::string Match::getName() const {return name;}
const std::vector<std::string>& Match::getTeams() const {
    static std::vector<std::string> teamNames;
    teamNames.clear();
    if (!team1.getName().empty()) {
        teamNames.push_back(team1.getName());
    }
    if (!team2.getName().empty()) {
        teamNames.push_back(team2.getName());
    }
    return teamNames;
}
void Match::setScore(uint8_t team1Score, uint8_t team2Score) {
    score[0] = team1Score;
    score[1] = team2Score;
}

void Match::addTeam(const Team team) {
    if (team1.getId()==uuids::uuid()) { // Überprüfen, ob team1 leer ist
        team1 = team;
    } else if (team2.getId()==uuids::uuid()) { // Überprüfen, ob team2 leer ist
        team2 = team;
    }
}   

void Match::rmvTeam(const Team team) {
    if (team1.getId() == team.getId()) {
        team1 = Team(); // Leeres Team zuweisen
    } else if (team2.getId() == team.getId()) {
        team2 = Team(); // Leeres Team zuweisen
    }
}