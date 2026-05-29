#pragma once

#include "httplib.h"
#include "tournament.hpp"
#include "sse-update.hpp"

/**
 * @brief Controller-Klasse, die HTTP-Requests für Match-Updates entgegennimmt.
 */
class MatchApiController {
private:
    Tournament* tournament; ///< Pointer auf das aktive Turnier.
    SseUpdate* sse;

public:
    /** * @brief Konstante für den API-Endpoint.
     * Hier kannst du den Pfad jederzeit zentral anpassen.
     */
    static constexpr const char* ENDPOINT = "/api/match/update";

    /**
     * @brief Konstruktor
     * @param tournament Pointer auf das Turnier, in dem das Match geupdatet werden soll.
     * @param sse Pointer auf den sse stream der die clients updated
     */
    explicit MatchApiController(Tournament* tournament, SseUpdate* sse);

    /**
     * @brief Registriert die POST-Route am übergebenen HTTP-Server.
     * @param svr Referenz auf den httplib::Server.
     */
    void registerRoutes(httplib::Server& svr);
};
