#include <iostream>
#include <memory>
#include <vector>

// Inkludiere deine Header
#include "team.hpp"
#include "match.hpp"
#include "ko-stage.hpp"
#include "tournament.hpp"
#include "uuid.h"

int main() {
    std::cout << "--- Starte Projekt-Tests ---\n\n";

    // ---------------------------------------------------------
    // 1. Test: Team-Klasse
    // ---------------------------------------------------------
    std::cout << "[1] Teste Team-Erstellung...\n";
    Team team1("Team Alpha");
    team1.addPlayer("Alice");

    Team team2("Team Bravo");
    team2.addPlayer("Bob");

    std::cout << "    Teams erstellt.\n";
    std::cout << "    Team 1 UUID existiert: " << (team1.getId().is_nil() ? "Nein" : "Ja") << "\n";

    // ---------------------------------------------------------
    // 2. Test: Match-Klasse direkt testen
    // ---------------------------------------------------------
    std::cout << "\n[2] Teste Match-Logik direkt...\n";
    Match testMatch;

    // Status WAITING prüfen
    std::cout << "    Initialer Status ist MATCH_WAITING: "
              << (testMatch.getStatus() == MATCH_WAITING) << "\n";

    testMatch.setTeam1(team1.getId());
    testMatch.setTeam2(team2.getId());
    std::cout << "    Match isReady() nach Team-Zuweisung: " << testMatch.isReady() << "\n";

    // Status auf RUNNING ändern
    testMatch.advanceStatus();

    // Score setzen und beenden
    Score finalScore;
    finalScore.points1 = 3;
    finalScore.points2 = 1;
    testMatch.setnewScore(finalScore);
    testMatch.advanceStatus(); // Setzt Status auf FINISHED

    std::cout << "    Status nach advanceStatus(): " << testMatch.getStatus() << " (sollte 2 / FINISHED sein)\n";
    std::cout << "    Gewinner-ID entspricht Team 1: " << (testMatch.getWinner() == team1.getId()) << "\n";


    // ---------------------------------------------------------
    // 3. Test: Tournament und Stage Setup
    // ---------------------------------------------------------
    std::cout << "\n[3] Teste Tournament & KoStage Setup...\n";
    Tournament tourney;
    tourney.updateName("Mein erstes Turnier");

    // Teams als unique_ptr für das Tournament erstellen
    auto tA = std::make_unique<Team>("Team A");
    auto tB = std::make_unique<Team>("Team B");

    // UUIDs zwischenspeichern für die Stage
    std::vector<uuids::uuid> teamIds = {tA->getId(), tB->getId()};

    // Teams an Tournament übergeben
    tourney.addTeam(std::move(tA));
    tourney.addTeam(std::move(tB));
    std::cout << "    Teams zum Tournament hinzugefuegt.\n";

    // Stage erstellen und Matches generieren
    auto koStage = std::make_unique<KoStage>();
    koStage->generateMatches(2, 1);
    koStage->populateMatches(teamIds);
    std::cout << "    KoStage Matches generiert und mit Teams befuellt.\n";

    // HINWEIS: Hier fehlt der Test für koStage->matchUpdate(...),
    // da wir keine Getter für die Match-IDs in der Stage haben!

    // Stage ans Tournament hängen
    tourney.pushStage(std::move(koStage));
    std::cout << "    Stage an Tournament uebergeben.\n";

    // tourney.start(); // Kann aufgerufen werden, sobald runNextStage implementiert ist

    std::cout << "\n--- Alle Basis-Tests abgeschlossen ---\n";
    return 0;
}
