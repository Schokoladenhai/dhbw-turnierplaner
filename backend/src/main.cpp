#include "httplib.h"
#include "tournament-builder.hpp"
#include "tournament.hpp"
#include "team.hpp"
#include "ko-stage.hpp"
#include "match-api-controller.hpp"

#include <iostream>
#include <memory>
#include <vector>

int main() {
    // ==========================================
    // 1. TURNIER ERSTELLEN & STARTEN
    // ==========================================
    TournamentBuilder builder;

    std::vector<std::unique_ptr<Team>> myTeams;
    myTeams.push_back(std::make_unique<Team>("Team Alpha"));
    myTeams.push_back(std::make_unique<Team>("Team Bravo"));
    myTeams.push_back(std::make_unique<Team>("Die wilden Pointer"));
    myTeams.push_back(std::make_unique<Team>("FC Segfault"));

    std::vector<uuids::uuid> teamIds;
    for (const auto& team : myTeams) {
        teamIds.push_back(team->getId());
    }

    auto koStage = std::make_unique<KoStage>();
    koStage->generateMatches(4, 3);
    koStage->populateMatches(teamIds);

    std::unique_ptr<Tournament> dummyTournament = builder
        .setName("C++ Coding Championship")
        .addTeams(std::move(myTeams))
        .addStage(std::move(koStage))
        .build();

    if (!dummyTournament) {
        std::cerr << "Fehler beim Erstellen des Turniers!" << std::endl;
        return 1;
    }

    // Turnier starten
    dummyTournament->start();
    std::cout << "Turnier erfolgreich gestartet! Status: " << dummyTournament->getStatus() << "\n\n";

    // Ausgabe der initialen Turnierstruktur (Hier kannst du dir eine Match-ID zum Testen raussuchen)
    std::cout << "=== Initiale Turnier-Struktur ===\n";
    std::cout << dummyTournament->toJson().dump(2) << "\n";
    std::cout << "=================================\n\n";


    // ==========================================
    // 2. WEBSERVER STARTEN
    // ==========================================
    // 1. Server & SSE vorbereiten
    httplib::Server svr;
    SseUpdate sseStream;
    // 2. Controller mit Pointer auf SSE erstellen
    MatchApiController matchApi(dummyTournament.get(), &sseStream);
    // 3. Beide Routen am Server registrieren
    sseStream.registerRoutes(svr);
    matchApi.registerRoutes(svr);
    // 4. Starte den Server
    svr.listen("0.0.0.0", 8080);


    return 0;
}
