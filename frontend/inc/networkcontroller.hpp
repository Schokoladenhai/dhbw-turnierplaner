#pragma once

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <string>

class NetworkController : public QObject {
    Q_OBJECT
public:
    explicit NetworkController(QObject *parent = nullptr);
    ~NetworkController();

    // Startet die dauerhafte SSE-Verbindung zum Server
    void startSseStream(const QString &hostUrl);

    // Sendet ein Match-Update an die API
    void sendMatchUpdate(const QString &matchId, int points1, int points2, int statusEnum);

    void triggerInitialBroadcast();

signals:
    // Signalisiert dem Frontend, dass eine neue JSON-Struktur vorliegt
    void tournamentUpdated(const std::string &rawJson);

private slots:
    void handleSseReadyRead();
    void handleSseFinished();

private:
    QNetworkAccessManager *networkManager;
    QNetworkReply *sseReply = nullptr;
};