#pragma once

#include <string>
#include <vector>
#include "uuid.h"
#include "uuid-gen.hpp"
#include "json.hpp"

/**
 * @brief Repräsentiert ein Team, das an einem Turnier teilnimmt.
 */
class Team{
    private:
    const uuids::uuid id;               ///< Eindeutige ID des Teams.
    std::string name;                   ///< Name des Teams.
    std::vector<std::string> players;   ///< Liste der Spielernamen in diesem Team.

    public:
    /**
     * @brief Erstellt ein neues Team.
     * @param name Der Name des Teams.
     * @param uuid Optionale UUID. Wenn nicht angegeben, wird eine neue generiert.
     */
    Team(std::string name, uuids::uuid uuid = generateUuid());

    /** @return Die eindeutige ID des Teams. */
    uuids::uuid getId() const;

    /**
     * @brief Fügt dem Team einen neuen Spieler hinzu.
     * @param name Der Name des Spielers.
     */
    void addPlayer(const std::string& name);

    /**
     * @brief Entfernt einen Spieler aus dem Team.
     * @param name Der Name des zu entfernenden Spielers.
     */
    void rmvPlayer(const std::string& name);

    using json = nlohmann::json;
    /** @return Eine JSON-Repräsentation des Teams. */
    json toJson() const;
};
