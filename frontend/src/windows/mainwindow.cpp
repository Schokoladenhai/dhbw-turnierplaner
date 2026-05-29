#include "windows/mainwindow.h"
#include "ui_mainwindow.h"
#include "windows/komatch.h"
#include <QGridLayout>
#include <QDebug>
#include <QListWidgetItem>
#include <algorithm> 

extern int numberOfStages;
extern std::vector<Team> teams;
extern std::vector<Match> matches; 

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Initialisierung der Klassenattribute für die Layout-Infrastruktur
    QWidget *firstTab = ui->tabWidget->widget(0);  
    tournamentPage = firstTab; 
    bracketLayout = new QGridLayout(tournamentPage);
    
    //Signal-Slot-Verbindung für die Teambox-Indexänderung
    connect(ui->teambox, &QComboBox::currentIndexChanged, this, [this](int index) {
        ui->playerlist->clear();
        
        if (index < 0 || index >= static_cast<int>(teams.size())) {
            return;
        }
        
        const Team& selectedTeam = teams[index];
        const std::vector<std::string>& players = selectedTeam.getPlayers();
        
        for (const std::string& playerName : players) {
            ui->playerlist->addItem(QString::fromStdString(playerName));
        }
    });

    //Team hinzufügen
    connect(ui->btnnewteam, &QPushButton::clicked, this, [this]() {
        QString qTeamName = ui->linenewteam->text().trimmed();
        if (qTeamName.isEmpty()) return;
        
        uuids::uuid newId = uuids::uuid(); 
        
        Team newTeam(qTeamName.toStdString(), newId);
        teams.push_back(newTeam); // Schreibt direkt in den globalen Vektor
        
        ui->teambox->addItem(qTeamName);
        ui->linenewteam->clear();
        ui->teambox->setCurrentIndex(ui->teambox->count() - 1);
    });

    //Team löschen
    connect(ui->btndeleteteam, &QPushButton::clicked, this, [this]() {
        int currentIndex = ui->teambox->currentIndex();
        if (currentIndex < 0 || currentIndex >= static_cast<int>(teams.size())) return;
        
        teams.erase(teams.begin() + currentIndex); // Löscht aus globalem Vektor
        ui->teambox->removeItem(currentIndex);
        
        if (teams.empty()) {
            ui->playerlist->clear();
        }
    });

    // Spieler hinzufügen
    connect(ui->btnaddplayer, &QPushButton::clicked, this, [this]() {
        int currentTeamIndex = ui->teambox->currentIndex();
        if (currentTeamIndex < 0) return;
        
        QString qPlayerName = ui->teamsline->text().trimmed();
        if (qPlayerName.isEmpty()) return;
        
        teams[currentTeamIndex].addPlayer(qPlayerName.toStdString());
        
        ui->playerlist->addItem(qPlayerName);
        ui->teamsline->clear();
    });

    // Spieler löschen
    connect(ui->btndeleteplayer, &QPushButton::clicked, this, [this]() {
        int currentTeamIndex = ui->teambox->currentIndex();
        int currentPlayerIndex = ui->playerlist->currentRow();
        
        if (currentTeamIndex < 0 || currentPlayerIndex < 0) return;
        
        QListWidgetItem* currentItem = ui->playerlist->currentItem();
        if (!currentItem) return;
        
        std::string playerStdName = currentItem->text().toStdString();
        teams[currentTeamIndex].rmvPlayer(playerStdName);
        
        delete ui->playerlist->takeItem(currentPlayerIndex);
    });

    //Speichern-Button (Diagnose-Ausgabe)
    connect(ui->btnsave, &QPushButton::clicked, this, [this]() {
        qDebug() << "Globaler Vektor status - Anzahl Teams:" << teams.size();
    });
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::updateTournamentUi() 
{
    // Bereinigung des alten Layouts zur Vermeidung von Widget-Überlagerungen und Speicherlecks
    QLayoutItem *item;
    while ((item = bracketLayout->takeAt(0)) != nullptr) {
        if (item->widget()) {
            delete item->widget();
        }
        delete item;
    }

    // ComboBox-Inhalte zurücksetzen und mit aktuellen Serverdaten befüllen
    ui->teambox->blockSignals(true); 
    ui->teambox->clear();
    ui->playerlist->clear();

    for (const auto& team : teams) {
        ui->teambox->addItem(QString::fromStdString(team.getName()));
    }
    ui->teambox->blockSignals(false);

    // Initialen Zustand nach Reset wiederherstellen
    if (!teams.empty()) {
        ui->teambox->setCurrentIndex(0);
        const Team& firstTeam = teams[0];
        for (const std::string& playerName : firstTeam.getPlayers()) {
            ui->playerlist->addItem(QString::fromStdString(playerName));
        }
    }

    // Neuaufbau des Turnierbaums mittels mathematisch präziser Prädikat-Zuordnung
    int initialMatches = numberOfStages; 
    int matchesInRound = initialMatches;

    int totalRounds = 0;
    for (int m = initialMatches; m > 0; m /= 2) {
        totalRounds++;
    }

    int totalRows = (initialMatches * 2) - 1;

    // Grid-Dimensionierung konfigurieren
    for (int row = 0; row < totalRows; ++row) {
        bracketLayout->setRowStretch(row, 1);
    }
    for (int col = 0; col < totalRounds; ++col) {
        bracketLayout->setColumnStretch(col, 1);
    }

    // Iteration über Spalten (Runden) und Zeilen (Matches)
    for (int round = 0; round < totalRounds; ++round) {
        int firstRowOffset = (1 << round) - 1;  
        int rowStep = 1 << (round + 1);         

        for (int i = 0; i < matchesInRound; ++i) {
            int targetRow = firstRowOffset + (i * rowStep);
            
            // Mathematische Berechnung des Backend-Schlüssels ("Match_X")
            int matchesAbove = 0;
            for (int r = round + 1; r < totalRounds; ++r) {
                matchesAbove += (initialMatches >> r);
            }
            int matchKey = matchesAbove + 1 + i;
            std::string targetMatchName = "Match_" + std::to_string(matchKey);

            //Suche im globalen Vektor
            auto it = std::find_if(matches.begin(), matches.end(), [&targetMatchName](const Match& m) {
                return m.getName() == targetMatchName;
            });

            // Initialisierung des lokalen matches
            Match currentMatch(targetMatchName, uuids::uuid{});

            if (it != matches.end()) {
                // Match-Identität im Server-JSON verifiziert
                currentMatch = *it;
            } else {
                // Fallback für zukünftige Runden ohne feststehende Paarungen
                currentMatch.team1 = Team("Offen / TBD", uuids::uuid{});
                currentMatch.team2 = Team("Offen / TBD", uuids::uuid{});
                currentMatch.score = {0, 0};
            }

            // Instanziierung des Widgets mit dem explizit gemappten Match-Modell als Konstruktor-Argument
            KoMatch* komatch = new KoMatch(currentMatch, tournamentPage);
            
            //Signal-Brücke schlagen zur Weiterleitung an MainWindow-Klassenebene
            connect(komatch, &KoMatch::matchResultChanged, this, &MainWindow::requestMatchUpdate);
            
            bracketLayout->addWidget(komatch, targetRow, round, Qt::AlignCenter);
        }
        matchesInRound /= 2;
    }
}