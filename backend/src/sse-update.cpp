#include "sse-update.hpp"
#include <chrono>

void SseUpdate::broadcast(const nlohmann::json& jsonData) {
    std::lock_guard<std::mutex> lock(mtx);
    latestData = jsonData.dump();
    version++;
    cv.notify_all();
}

void SseUpdate::registerRoutes(httplib::Server& svr) {
    svr.Get(ENDPOINT, [this](const httplib::Request& req, httplib::Response& res) {

        // SSE-spezifische Header setzen
        res.set_header("Cache-Control", "no-cache");
        res.set_header("Connection", "keep-alive");
        res.set_header("Access-Control-Allow-Origin", "*");

        res.set_chunked_content_provider("text/event-stream",
            [this](size_t offset, httplib::DataSink &sink) {
                uint64_t localVersion = 0;

                {
                    std::lock_guard<std::mutex> lock(mtx);
                    if (!latestData.empty()) {
                        std::string msg = "data: " + latestData + "\n\n";
                        sink.write(msg.c_str(), msg.size());
                        localVersion = version;
                    }
                }

                while (true) {
                    std::string dataToSend;
                    bool sendPing = false;

                    {
                        std::unique_lock<std::mutex> lock(mtx);
                        bool updated = cv.wait_for(lock, std::chrono::seconds(3),
                            [this, localVersion] { return version > localVersion; });

                        if (updated) {
                            dataToSend = latestData;
                            localVersion = version;
                        } else {
                            sendPing = true;
                        }
                    }

                    if (sendPing) {
                        if (!sink.write(":\n\n", 3)) return false;
                    } else {
                        std::string msg = "data: " + dataToSend + "\n\n";
                        if (!sink.write(msg.c_str(), msg.size())) return false;
                    }
                }

                return true;
            }
        );
    });
}
