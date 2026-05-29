#pragma once

#include "json.hpp"
#include "stage.hpp"
#include "match.hpp"
#include "uuid.h"
#include <vector>

/**
 * @brief Repräsentiert eine K.-o.-Phase (Knockout-System) innerhalb eines Turniers.
 * * In dieser Phase scheidet der Verlierer eines Matches direkt aus,
 * während der Gewinner in die nächste Runde vorrückt.
 */
class KoStage: public Stage {
    private:
    /**
     * @brief Speichert die Baumstruktur der Matches für das K.-o.-System.
     */
    std::vector<uuids::uuid> matchTree{};

    public:
    /**
     * @brief Aktualisiert den Status und das Ergebnis eines spezifischen Matches.
     * * @param currentMatch Die UUID des zu aktualisierenden Matches.
     * @param newScore Der neue Punktestand des Matches.
     * @param newStatus Der neue Status des Matches (z.B. FINISHED).
     * @return true, wenn das Update erfolgreich war, andernfalls false.
     */
    bool matchUpdate(const uuids::uuid currentMatch, const Score& newScore, const MatchStatus newStatus) override;

    /**
     * @brief Prüft, ob ein Spielergebnis für diese Phase gültig ist.
     * * In einer K.-o.-Phase darf es typischerweise kein Unentschieden geben.
     * * @param score Der zu prüfende Punktestand.
     * @return true, wenn das Ergebnis gültig ist (z.B. es gibt einen klaren Sieger).
     */
    bool isValidMatchResult(const Score& score) const override;

    /**
     * @brief Generiert die leere Match-Struktur für die K.-o.-Phase.
     * * @param totalTeams Die Gesamtanzahl der teilnehmenden Teams in dieser Phase.
     * @param totalMatches Die Gesamtanzahl der zu generierenden Matches.
     */
    void generateMatches(const int totalTeams, const int totalMatches) override;

    /**
     * @brief Befüllt die generierten Matches mit den tatsächlichen Teams.
     * * @param teamIds Eine Liste der UUIDs der teilnehmenden Teams.
     */
    void populateMatches(const std::vector<uuids::uuid>& teamIds) override;

    /**
     * @brief Ermittelt alle Teams, die diese Phase erfolgreich abgeschlossen haben und weiterkommen.
     * * @return Ein Vektor mit den UUIDs der weiterkommenden Teams (die Sieger der finalen Matches).
     */
    std::vector<uuids::uuid> getAdvancingTeams() const override;

    using json = nlohmann::json;
    /**
     * @brief Serialisiert die K.-o.-Phase in ein JSON-Objekt.
     * * @return Das JSON-Objekt, das diese Phase repräsentiert.
     */
    json toJson() const override;
};
