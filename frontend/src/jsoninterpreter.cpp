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

    // 2. Teams extrahieren
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

    // Hier folgt Schritt 3: Stages und Matches
    // 3. Stages und eingebettete Matches extrahieren
    if (j.contains("stages") && j["stages"].is_array()) {
        for (const auto& stageJson : j["stages"]) {
            
            // Überprüfen, ob die Stage ein valides "matches"-Objekt besitzt
            if (stageJson.contains("matches") && stageJson["matches"].is_object()) {
                
                // .items() erlaubt das Iterieren über Key-Value-Paare eines JSON-Objekts
                // matchKey ist z.B. "1", matchJson enthält das eigentliche Match-Datenobjekt
                for (const auto& [matchKey, matchJson] : stageJson["matches"].items()) {
                    
                    std::string matchIdStr = matchJson.at("id").get<std::string>();
                    uuids::uuid matchUuid = uuids::uuid::from_string(matchIdStr).value_or(uuids::uuid{});

                    // Instanziierung des Matches (Key wird als Match-Name adaptiert)
                    Match match("Match_" + matchKey, matchUuid);

                    // Extraktion der referenzierten Team-IDs als String
                    std::string team1Id = matchJson.at("team1").get<std::string>();
                    std::string team2Id = matchJson.at("team2").get<std::string>();

                    // Relationen auflösen: Teams aus der Lookup-Map holen und dem Match hinzufügen
                    // .at() wirft eine Exception, falls das Backend eine ungültige ID liefert
                    match.addTeam(teamLookup.at(team1Id));
                    match.addTeam(teamLookup.at(team2Id));

                    // Dynamische Keys im Score-Objekt auslesen
                    if (matchJson.contains("score") && matchJson["score"].is_object()) {
                        auto const& scoreJson = matchJson["score"];
                        
                        // Da die Keys im JSON exakt den Team-IDs entsprechen, 
                        // nutzen wir die Strings team1Id und team2Id als Abfrage-Keys
                        uint8_t score1 = scoreJson.at(team1Id).get<uint8_t>();
                        uint8_t score2 = scoreJson.at(team2Id).get<uint8_t>();
                        
                        match.setScore(score1, score2);
                    }

                    // Das fertig konfigurierte Match in den globalen Vektor pushen
                    matches.push_back(match);
                }
            }
        }
    }
}