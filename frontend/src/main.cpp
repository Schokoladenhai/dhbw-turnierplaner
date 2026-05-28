#include <QApplication>
#include <QPushButton>
#include <QDebug> // Wichtig für Textausgaben
#include "main.hpp"
#include <fstream>
#include "jsoninterpreter.hpp"

std::vector<Team> teams;
std::vector<Match> matches;
std::string tournamentName;
int numberOfStages;


int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    std::ifstream fileStream("gemini-code-1779975893211.json");
    std::stringstream stringBuffer;
    stringBuffer << fileStream.rdbuf();
    
    // Puffer in nativen std::string konvertieren
    std::string jsonRaw = stringBuffer.str();

    // 3. Schließen des Streams (erfolgt alternativ auch automatisch per Destruktor)
    fileStream.close();

    // 4. Übergabe an den JSON-Parser
    jsontoclasses(jsonRaw);
    int anzTeams = teams.size();
    qDebug() << "Anzahl Teams: " << anzTeams;
    qDebug() << "Anzahl Runden: " << numberOfStages;
    // Qt's Version von std::cout
    qDebug() << "Frontend-App gestartet!";

    MainWindow w;
    w.show();

    return app.exec();
}

