#include "networkcontroller.hpp"
#include <QNetworkReply>
#include <QByteArray>
#include <QDebug>
#include <QUrl>
#include <QNetworkRequest>
#include <nlohmann/json.hpp>



NetworkController::NetworkController(QObject *parent)
    : QObject(parent), networkManager(new QNetworkAccessManager(this)) {}

NetworkController::~NetworkController() {
    if (sseReply) {
        sseReply->disconnect();
        sseReply->abort();
    }
}

void NetworkController::startSseStream(const QString &hostUrl) {
    QUrl url(hostUrl + "/api/stream"); 
    QNetworkRequest request(url);
    
    request.setHeader(QNetworkRequest::ContentTypeHeader, "text/event-stream");
    request.setAttribute(QNetworkRequest::CacheLoadControlAttribute, QNetworkRequest::AlwaysNetwork);

    sseReply = networkManager->get(request);
    
    // Protokolliert den HTTP-Antwortcode (z.B. 200 bei Erfolg)
    connect(sseReply, &QNetworkReply::metaDataChanged, this, [this]() {
        int statusCode = sseReply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
        qDebug() << "[NetworkController] HTTP-Statuscode erhalten:" << statusCode;
    });

    connect(sseReply, &QNetworkReply::readyRead, this, &NetworkController::handleSseReadyRead);
    connect(sseReply, &QNetworkReply::finished, this, &NetworkController::handleSseFinished);
}

void NetworkController::handleSseReadyRead() {
    if (!sseReply) return;

    while (sseReply->canReadLine()) {
        QByteArray line = sseReply->readLine().trimmed();
        
        // Macht die unsichtbaren 3-Sekunden-Pings (":") auf der Konsole sichtbar
        qDebug() << "[NetworkController] Raw-Zeile vom Server:" << line;

        if (line.startsWith("data: ")) {
            QByteArray rawJson = line.mid(6).trimmed();
            qDebug() << "[NetworkController] JSON-Nutzdaten erkannt, verarbeite...";
            emit tournamentUpdated(rawJson.toStdString());
        } 
        else if (!line.isEmpty() && !line.startsWith(":")) {
            emit tournamentUpdated(line.toStdString());
        }
    }
}

void NetworkController::handleSseFinished() {
    if (sseReply) {
        qWarning() << "[NetworkController] Verbindung beendet oder fehlgeschlagen:" << sseReply->errorString();
        sseReply->deleteLater();
        sseReply = nullptr;
    }
}

// NEU: Implementierung des Ergebnis-Rückkanals via HTTP-POST
void NetworkController::sendMatchUpdate(const QString &matchId, int points1, int points2, int statusEnum) {
    QUrl url("http://localhost:8080/api/match/update"); 
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    nlohmann::json jsonPayload;
    jsonPayload["id"] = matchId.toStdString();
    jsonPayload["points1"] = std::to_string(points1);
    jsonPayload["points2"] = std::to_string(points2);
    
    // KORREKTUR: Numerischen Wert in den vom Server erwarteten Enum-String mappen
    std::string statusStr = "WAITING";
    if (statusEnum == 1) statusStr = "RUNNING";
    else if (statusEnum == 2) statusStr = "FINISHED";
    else if (statusEnum == 3) statusStr = "SKIPPED"; // Mappt auf serverseitiges MATCH_SKIPED
    
    jsonPayload["status"] = statusStr;

    QByteArray bodyData = QByteArray::fromStdString(jsonPayload.dump());
    qDebug() << "[NetworkController] Sende korrigierten POST-Payload:" << bodyData;

    QNetworkReply *reply = networkManager->post(request, bodyData);
    
    connect(reply, &QNetworkReply::finished, this, [reply]() {
        if (reply->error() == QNetworkReply::NoError) {
            qDebug() << "[NetworkController] POST erfolgreich. Server-Antwort:" << reply->readAll();
        } else {
            qCritical() << "[NetworkController] POST fehlgeschlagen:" << reply->errorString();
        }
        reply->deleteLater();
    });
}

void NetworkController::triggerInitialBroadcast() {
    qDebug() << "[NetworkController] Starte automatischen Initial-Trigger für die UI...";
    
    // Verwendung der von Ihnen verifizierten funktionierenden Route und Parameter
    sendMatchUpdate("f5045fc9-a94c-4c99-88d0-6eaefb8d242f", 3, 1, 1);
}