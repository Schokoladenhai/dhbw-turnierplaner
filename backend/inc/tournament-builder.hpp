#pragma once

#include "tournament.hpp"
#include "stage.hpp"
#include "uuid.h"
#include "team.hpp"
#include "json.hpp"
#include <memory>
#include <unordered_map>
#include <string>
#include <vector>
#include <memory>

/**
 * @brief Wendet das Builder-Pattern an, um ein Tournament-Objekt schrittweise zu konstruieren.
 * * Diese Klasse vereinfacht die komplexe Initialisierung eines Turniers durch Methodenverkettung (Method Chaining).
 */
class TournamentBuilder {
    private:
    std::string name = "Tournament"; ///< Temporärer Name des Turniers.
    std::unordered_map<uuids::uuid, std::unique_ptr<Team>> teams{}; ///< Temporärer Speicher für die Teams.
    std::vector<std::unique_ptr<Stage>> stages{};                   ///< Temporärer Speicher für die Phasen.

    public:
    /**
     * @brief Setzt den Namen des Turniers.
     * @param newName Der gewünschte Turniername.
     * @return Referenz auf den Builder für Method Chaining.
     */
    TournamentBuilder& setName(std::string newName);

    /**
     * @brief Fügt ein einzelnes Team hinzu.
     * @param newTeam Unique Pointer auf das neue Team.
     * @return Referenz auf den Builder.
     */
    TournamentBuilder& addTeam(std::unique_ptr<Team> newTeam);

    /**
     * @brief Fügt eine Liste von Teams hinzu.
     * @param newTeams Vektor mit Unique Pointern auf die Teams.
     * @return Referenz auf den Builder.
     */
    TournamentBuilder& addTeams(std::vector<std::unique_ptr<Team>> newTeams);

    /**
     * @brief Fügt eine einzelne Turnierphase (Stage) hinzu.
     * @param newStage Unique Pointer auf die neue Phase.
     * @return Referenz auf den Builder.
     */
    TournamentBuilder& addStage(std::unique_ptr<Stage> newStage);

    /**
     * @brief Fügt mehrere Turnierphasen in der angegebenen Reihenfolge hinzu.
     * @param newStages Vektor mit Unique Pointern auf die Phasen.
     * @return Referenz auf den Builder.
     */
    TournamentBuilder& addStages(std::vector<std::unique_ptr<Stage>> newStages);

    using json = nlohmann::json;
    /**
     * @brief Rekonstruiert den Builder-Zustand aus einem JSON-Objekt.
     * @param tournamentJson Das JSON-Objekt mit den Turnierdaten.
     * @return Referenz auf den Builder.
     */
    TournamentBuilder& loadJson(json tournamentJson);

    /**
     * @brief Schließt den Bauvorgang ab und erstellt das finale Turnier-Objekt.
     * @return Ein Unique Pointer auf das fertige Tournament.
     */
    std::unique_ptr<Tournament> build();
};
