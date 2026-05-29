#include "match-api-controller.hpp"
#include "json.hpp"
#include "httplib.h"
#include "tournament.hpp"

using json = nlohmann::json;

MatchApiController::MatchApiController(Tournament* tournament, SseUpdate* sse)
        : tournament(tournament), sse(sse) {}

void MatchApiController::registerRoutes(httplib::Server& svr) {

    svr.Post(ENDPOINT, [this](const httplib::Request& req, httplib::Response& res) {
        try {
            auto j = json::parse(req.body);

            std::string idStr = j.at("id").get<std::string>();
            std::string p1_str = j.at("points1").get<std::string>();
            std::string p2_str = j.at("points2").get<std::string>();

            int points1 = p1_str.empty() ? 0 : std::stoi(p1_str);
            int points2 = p2_str.empty() ? 0 : std::stoi(p2_str);

            auto matchId = uuids::uuid::from_string(idStr);

            MatchStatus newStatus = j.at("status").get<MatchStatus>();

            Score newScore;
            newScore.points1 = points1;
            newScore.points2 = points2;

            if (tournament->getStatus() != TOURNAMENT_RUNNING) {
                res.status = 400;
                res.set_content("{\"error\": \"Turnier läuft derzeit nicht.\"}", "application/json");
                return;
            }

            Stage* currentStage = tournament->getCurrentStage();
            if (currentStage == nullptr) {
                res.status = 500;
                res.set_content("{\"error\": \"Keine aktive Phase (Stage) gefunden.\"}", "application/json");
                return;
            }

            bool success = currentStage->matchUpdate(matchId.value(), newScore, newStatus);

            if (success) {

                sse->broadcast(tournament->toJson());
                res.status = 200; // OK
                res.set_content("{\"message\": \"Match erfolgreich aktualisiert.\"}", "application/json");
            } else {
                res.status = 400; // Bad Request
                res.set_content("{\"error\": \"Update durch die Stage-Logik abgelehnt (z.B. ungültiges Ergebnis).\"}", "application/json");
            }

        } catch (const json::exception& e) {
            res.status = 400;
            res.set_content(std::string("{\"error\": \"JSON Parsing-Fehler: ") + e.what() + "\"}", "application/json");
        } catch (const std::exception& e) {
            res.status = 500;
            res.set_content(std::string("{\"error\": \"Server-Fehler: ") + e.what() + "\"}", "application/json");
        }
    });
}
