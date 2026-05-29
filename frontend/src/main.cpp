#include <QApplication>
#include <QPushButton>
#include <QDebug> 
#include "main.hpp"
#include <fstream>
#include "jsoninterpreter.hpp"
#include "networkcontroller.hpp" // Erforderlich für die API-Anbindung
#include "windows/mainwindow.h"

std::vector<Team> teams;
std::vector<Match> matches;
std::string tournamentName;
int numberOfStages;

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    // Hauptfenster instanziieren und anzeigen
    MainWindow w;
    w.show();

    // Instanziierung des Netzwerk-Controllers auf dem Stack
    NetworkController netController;

    // 1. Reaktiver Empfangskanal (SSE): Vom Webserver -> Frontend
    QObject::connect(&netController, &NetworkController::tournamentUpdated, &app, [&w](const std::string &rawJson) {
        std::string editableJson = rawJson;
        
        // Deserialisierung in globale Klassenstrukturen durchführen
        jsontoclasses(editableJson); 
        qDebug() << "[Main] Globale Daten via SSE aktualisiert. Teams im Speicher:" << teams.size();
        
        // Neuaufbau der grafischen Widgets erzwingen
        w.updateTournamentUi();
    });

    // 2. Ergebnis-Rückkanal (POST): Von der GUI (MainWindow) -> Netzwerk-Controller
    QObject::connect(&w, &MainWindow::requestMatchUpdate, &netController, &NetworkController::sendMatchUpdate);

    netController.triggerInitialBroadcast(); // Optional: Initialer Trigger für die UI, um sofortige Sichtbarkeit zu gewährleisten

    // Asynchronen HTTP-GET-Request für den Server-Sent-Events-Stream initiieren
    netController.startSseStream("http://127.0.0.1:8080");

    return app.exec();
}