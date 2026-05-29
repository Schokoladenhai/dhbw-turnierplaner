#pragma once

#include "stage.hpp"
#include "uuid.h"
#include "team.hpp"
#include "json.hpp"
#include <memory>
#include <unordered_map>
#include <string>
#include <vector>

/**
 * @brief Beschreibt den globalen Status des gesamten Turniers.
 */
enum TournamentStatus{
    TOURNAMENT_READY,   ///< Turnier ist konfiguriert und bereit zum Start.
    TOURNAMENT_RUNNING, ///< Turnier läuft derzeit.
    TOURNAMENT_FINISHED ///< Turnier ist komplett abgeschlossen.
};

NLOHMANN_JSON_SERIALIZE_ENUM(TournamentStatus, {
    {TOURNAMENT_READY, "READY"},
    {TOURNAMENT_RUNNING, "RUNNING"},
    {TOURNAMENT_FINISHED, "FINISHED"}
})

/**
 * @brief Die Hauptklasse, die ein komplettes Turnier verwaltet.
 * * Verknüpft die teilnehmenden Teams mit verschiedenen Turnierphasen (Stages).
 */
class Tournament {
    private:
    TournamentStatus status = TOURNAMENT_READY; ///< Derzeitiger Turnierstatus.
    unsigned int currentStageIndex = 0;         ///< Index der aktuell laufenden Phase (Stage).
    std::string name;                           ///< Name des Turniers.
    std::unordered_map<uuids::uuid, std::unique_ptr<Team>> teams; ///< Alle am Turnier teilnehmenden Teams.
    std::vector<std::unique_ptr<Stage>> stages;                   ///< Die sequenziellen Phasen des Turniers.

    public:
    /**
     * @brief Konstruiert ein neues Turnier.
     * * @param name Name des Turniers.
     * @param teams Map aller teilnehmenden Teams.
     * @param stages Vektor der Turnierphasen.
     */
    Tournament(std::string name, std::unordered_map<uuids::uuid, std::unique_ptr<Team>> teams, std::vector<std::unique_ptr<Stage>> stages);

    /** @return Den aktuellen Status des Turniers. */
    TournamentStatus getStatus();

    /**
     * @brief Gibt die momentan aktive Turnierphase zurück.
     * @return Pointer auf die aktuelle Stage.
     */
    Stage* getCurrentStage() const;

    /**
     * @brief Startet das Turnier und initialisiert die erste Phase.
     * @return true, wenn das Turnier erfolgreich gestartet wurde.
     */
    bool start();

    /**
     * @brief Schließt die aktuelle Phase ab und startet die nächste Phase mit den Gewinnern.
     * * Wird normalerweise als Callback (`onFinished`) von einer `Stage` aufgerufen.
     * * @param teamIds Die UUIDs der Teams, die sich für die nächste Phase qualifiziert haben.
     */
    void runNextStage(std::vector<uuids::uuid> teamIds);

    /**
     * @brief Beendet das Turnier.
     * @param teamIds Die UUIDs der endgültigen Gewinner/Platzierten.
     */
    void end(std::vector<uuids::uuid> teamIds);

    using json = nlohmann::json;
    /** @return Eine JSON-Repräsentation des kompletten Turniers, inklusive Teams und Phasen. */
    json toJson() const;
};
