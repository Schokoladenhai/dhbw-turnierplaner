#include "jsoninterpreter.hpp"
#include <nlohmann/json.hpp>
#include <vector>
#include <unordered_map>
#include <string>
#include "classes/match.hpp"
#include "classes/team.hpp"



extern std::vector<Team> teams;
extern std::vector<Match> matches;
extern std::string tournamentName;
extern int numberOfStages;
using json = nlohmann::json;
void jsontoclasses(std::string& rawjson){
    teams.clear();
    matches.clear();
    tournamentName = "";
    numberOfStages = 0;


    auto j = json::parse(rawjson);
    
    if (j.contains("name")) {
        tournamentName = j.at("name").get<std::string>();
    }

    if (j.contains("stages") && j["stages"].is_array()) {
        numberOfStages = static_cast<int>(j["stages"].size())+1;
    }

    // Lokale Lookup-Tabelle für die relationale Zuordnung der Teams
    std::unordered_map<std::string, Team> teamLookup;

    //Teams extrahieren
    if (j.contains("teams") && j["teams"].is_array()) {
        for (const auto& teamJson : j["teams"]) {
            std::string idStr = teamJson.at("id").get<std::string>();
            std::string name = teamJson.at("name").get<std::string>();

            // Konvertierung in deinen UUID-Typ
            uuids::uuid teamUuid = uuids::uuid::from_string(idStr).value_or(uuids::uuid{});

            // Team-Objekt erstellen
            Team team(name, teamUuid);

            // Spieler hinzufügen, falls vorhanden
            if (teamJson.contains("players") && teamJson["players"].is_array()) {
                for (const auto& playerJson : teamJson["players"]) {
                    team.addPlayer(playerJson.get<std::string>());
                }
            }

            // In globalen Vektor speichern
            teams.push_back(team);

            // In lokaler Map für die Match-Auflösung registrieren
            teamLookup[idStr] = team;
        }
    }

    // Stages und eingebettete Matches extrahieren
    if (j.contains("stages") && j["stages"].is_array()) {
        for (const auto& stageJson : j["stages"]) {
            
            if (stageJson.contains("matches") && stageJson["matches"].is_object()) {
                
                for (const auto& [matchKey, matchJson] : stageJson["matches"].items()) {
                    
                    std::string matchIdStr = matchJson.at("id").get<std::string>();
                    uuids::uuid matchUuid = uuids::uuid::from_string(matchIdStr).value_or(uuids::uuid{});

                    Match match("Match_" + matchKey, matchUuid);

                    std::string team1Id = matchJson.at("team1").get<std::string>();
                    std::string team2Id = matchJson.at("team2").get<std::string>();

                    //Absicherung des Map-Zugriffs mittels .contains()
                    if (teamLookup.contains(team1Id)) {
                        match.addTeam(teamLookup.at(team1Id));
                    } else {
                        uuids::uuid emptyUuid = uuids::uuid::from_string(team1Id).value_or(uuids::uuid{});
                        match.addTeam(Team("Offen / TBD", emptyUuid));
                    }

                    if (teamLookup.contains(team2Id)) {
                        match.addTeam(teamLookup.at(team2Id));
                    } else {
                        uuids::uuid emptyUuid = uuids::uuid::from_string(team2Id).value_or(uuids::uuid{});
                        match.addTeam(Team("Offen / TBD", emptyUuid));
                    }

                    // Keys im Score-Objekt auslesen
                    if (matchJson.contains("score") && matchJson["score"].is_object()) {
                        auto const& scoreJson = matchJson["score"];
                        
                        // Zugriff absichern, falls IDs im Score fehlen
                        uint8_t score1 = scoreJson.contains(team1Id) ? scoreJson.at(team1Id).get<uint8_t>() : 0;
                        uint8_t score2 = scoreJson.contains(team2Id) ? scoreJson.at(team2Id).get<uint8_t>() : 0;
                        
                        match.setScore(score1, score2);
                    }

                    if (matchJson.contains("status")) {
                        if (matchJson.at("status").is_string()) {
                            // Verarbeitung, wenn das Backend den serialisierten String sendet ("RUNNING", "FINISHED")
                            match.status = matchJson.at("status").get<std::string>();
                        } 
                        else if (matchJson.at("status").is_number()) {
                            // Fallback-Logik, falls das Backend den reinen Integer-Wert (0, 1, 2) mitschickt
                            int statusInt = matchJson.at("status").get<int>();
                            if (statusInt == 0) match.status = "WAITING";
                            else if (statusInt == 1) match.status = "RUNNING";
                            else if (statusInt == 2) match.status = "FINISHED";
                        }
                    }

                    matches.push_back(match);
                }
            }
        }
    }
}