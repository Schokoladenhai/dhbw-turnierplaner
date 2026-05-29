#pragma once

#include "uuid.h"
#include "json.hpp"

/**
 * @brief Repräsentiert den Punktestand eines Matches.
 */
struct Score{
    int points1 = 0; ///< Punkte von Team 1
    int points2 = 0; ///< Punkte von Team 2
};

/**
 * @brief Definiert die möglichen Zustände, in denen sich ein Match befinden kann.
 */
enum MatchStatus{
    MATCH_WAITING,  ///< Match wartet auf Teams oder den Start.
    MATCH_RUNNING,  ///< Match läuft aktuell.
    MATCH_FINISHED, ///< Match ist beendet und hat ein Ergebnis.
    MATCH_SKIPED    ///< Match wurde übersprungen (z.B. durch ein Freilos/Bye).
};

NLOHMANN_JSON_SERIALIZE_ENUM(MatchStatus, {
    {MATCH_WAITING, "WAITING"},
    {MATCH_RUNNING, "RUNNING"},
    {MATCH_FINISHED, "FINISHED"},
    {MATCH_SKIPED, "SKIPPED"},
})

/**
 * @brief Repräsentiert ein einzelnes Aufeinandertreffen zweier Teams.
 */
class Match{
    private:
    const uuids::uuid id;         ///< Eindeutige ID des Matches.
    MatchStatus status = MATCH_WAITING; ///< Aktueller Status des Matches.
    uuids::uuid team1Id{};        ///< ID des ersten Teams.
    uuids::uuid team2Id{};        ///< ID des zweiten Teams.
    Score score{};                ///< Aktueller Punktestand.

    public:
    /**
     * @brief Standardkonstruktor. Erstellt ein neues Match mit einer eindeutigen UUID.
     */
    Match();

    /**
     * @brief Setzt das erste Team des Matches.
     * @param newTeam1Id Die UUID von Team 1.
     */
    void setTeam1(uuids::uuid newTeam1Id);

    /**
     * @brief Setzt das zweite Team des Matches.
     * @param newTeam2Id Die UUID von Team 2.
     */
    void setTeam2(uuids::uuid newTeam2Id);

    /** @return Die UUID von Team 1. */
    uuids::uuid getTeam1() const;

    /** @return Die UUID von Team 2. */
    uuids::uuid getTeam2() const;

    /**
     * @brief Aktualisiert den Punktestand des Matches.
     * @param newScore Der neue Punktestand.
     */
    void setnewScore(const Score& newScore);

    /**
     * @brief Setzt den Status des Matches in die nächste logische Phase (z.B. WAITING -> RUNNING -> FINISHED).
     * @return true, wenn der Status erfolgreich vorangeschritten ist.
     */
    bool advanceStatus();

    /**
     * @brief Überspringt das Match und setzt den Status auf MATCH_SKIPED.
     */
    void skip();

    /**
     * @brief Ermittelt den Gewinner des Matches anhand des aktuellen Punktestands.
     * @return Die UUID des Gewinnerteams. Bei einem Unentschieden oder ungültigem Status das Verhalten je nach Implementierung.
     */
    uuids::uuid getWinner() const;

    /** @return Die eindeutige ID des Matches. */
    uuids::uuid getId() const;

    /** @return Den aktuellen Status des Matches. */
    MatchStatus getStatus() const;

    /**
     * @brief Prüft, ob das Match bereit ist, gestartet zu werden (z.B. beide Teams sind zugewiesen).
     * @return true, wenn das Match startklar ist.
     */
    bool isReady() const;

    using json = nlohmann::json;
    /** @return Eine JSON-Repräsentation des Matches. */
    json toJson() const;
};
