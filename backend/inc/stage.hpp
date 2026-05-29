#pragma once

#include "match.hpp"
#include "uuid.h"
#include "json.hpp"
#include <cstddef>
#include <functional>
#include <unordered_map>
#include <memory>
#include <vector>

/**
 * @brief Abstrakte Basisklasse für eine Turnierphase (z.B. Gruppenphase, K.-o.-Phase).
 */
class Stage {
    using FinishedCallback = std::function<void(std::vector<uuids::uuid>)>;

    protected:
    /**
     * @brief Speichert alle Matches dieser Phase, zugeordnet zu ihrer UUID.
     */
    std::unordered_map<uuids::uuid, std::unique_ptr<Match>> matches{};

    /**
     * @brief Callback, der aufgerufen wird, wenn die Phase abgeschlossen ist.
     * Übergibt die UUIDs der weiterkommenden Teams an die Turnier-Logik.
     */
    FinishedCallback onFinished = NULL;

    public:
    virtual ~Stage() = default;

    /**
     * @brief Holt einen Pointer auf ein Match anhand seiner ID.
     * @param id Die UUID des gesuchten Matches.
     * @return Pointer auf das Match oder nullptr, falls nicht gefunden.
     */
    Match* getMatchById(uuids::uuid id) const;

    /**
     * @brief Setzt den Callback, der am Ende der Phase ausgeführt werden soll.
     * * @param cb Die Callback-Funktion (z.B. ein Lambda aus der Tournament-Klasse).
     */
    void setOnFinished(FinishedCallback cb);

    /**
     * @brief Prüft, ob ein Update für ein bestimmtes Match unter den aktuellen Umständen akzeptiert werden darf.
     * * @param currentMatch UUID des Matches.
     * @param newScore Der vorgeschlagene neue Punktestand.
     * @param newStatus Der vorgeschlagene neue Status.
     * @return true, wenn das Update legitim ist.
     */
    virtual bool canAcceptMatchUpdate(const uuids::uuid currentMatch, const Score& newScore, const MatchStatus newStatus) const;

    /**
     * @brief Führt das Update für ein Match durch. Rein virtuelle Methode.
     * * @param currentMatch UUID des Matches.
     * @param newScore Der neue Punktestand.
     * @param newStatus Der neue Status.
     * @return true bei Erfolg.
     */
    virtual bool matchUpdate(const uuids::uuid currentMatch, const Score& newScore, const MatchStatus newStatus) = 0;

    /** @brief Prüft, ob das Ergebnis (z.B. kein Unentschieden) für diese Phasen-Art gültig ist. */
    virtual bool isValidMatchResult(const Score& score) const = 0;

    /** @brief Generiert die Matches basierend auf der Team- und Matchanzahl. */
    virtual void generateMatches(const int totalteams, const int totalMatches) = 0;

    /** @brief Weist den leeren Matches die Team-IDs zu. */
    virtual void populateMatches(const std::vector<uuids::uuid>& teamIds) = 0;

    /** @brief Gibt eine Liste der Teams zurück, die in die nächste Phase einziehen. */
    virtual std::vector<uuids::uuid> getAdvancingTeams() const = 0;

    using json = nlohmann::json;
    /** @brief Serialisiert die gesamte Phase nach JSON. */
    virtual json toJson() const = 0;
};
