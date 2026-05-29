#pragma once

#include "httplib.h"
#include "json.hpp"
#include <mutex>
#include <condition_variable>
#include <string>

/**
 * @brief Verwaltet Server-Sent Events (SSE) Verbindungen und sendet Turnier-Updates an alle Clients.
 */
class SseUpdate {
private:
    std::mutex mtx;
    std::condition_variable cv;
    std::string latestData;
    uint64_t version = 0;

public:
    /** @brief Der Endpoint, mit dem sich das Frontend verbindet (z.B. per EventSource) */
    static constexpr const char* ENDPOINT = "/api/stream";

    /**
     * @brief Sendet den neuen Turnier-Status an alle verbundenen Clients.
     * @param jsonData Das komplette Turnier als JSON-Objekt.
     */
    void broadcast(const nlohmann::json& jsonData);

    /**
     * @brief Registriert die GET-Route für den SSE-Stream am Server.
     * @param svr Referenz auf den httplib::Server.
     */
    void registerRoutes(httplib::Server& svr);
};
