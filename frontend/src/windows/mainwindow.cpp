#include "windows/mainwindow.h"
#include "ui_mainwindow.h"
#include "windows/komatch.h"
#include <QGridLayout>

extern int numberOfStages; // Holt die globale Anzahl der Start-Matches
extern std::vector<Team> teams; // Holt die globale Liste der Teams

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this); // Lädt das gesamte QTabWidget mitsamt deinen Buttons

    // --- DIE ENTKOPPLUNG: Zuweisung nach Index, nicht nach Name ---
    // Wir holen uns die beiden Reiter über ihren harten Index (0 = erster Tab links, 1 = zweiter Tab)
    QWidget *firstTab = ui->tabWidget->widget(0);  
    QWidget *secondTab = ui->tabWidget->widget(1); 

    // ENTSCHEIDUNG: Welcher Tab ist im Designer wirklich leer?
    // Wenn deine Buttons auf dem ersten Tab (links) liegen, muss der Baum auf den 'secondTab'.
    // Wenn deine Buttons auf dem zweiten Tab liegen, nimm 'firstTab'.
    QWidget *tournamentPage = firstTab; // <--- Hier einfach tauschen, falls es falsch herum ist!

    // Das Layout für den Baum wird jetzt exakt auf der leeren Seite verankert
    QGridLayout *bracketLayout = new QGridLayout(tournamentPage);
    
    // --- AB HIER BLEIBT DEIN CODE IDENTISCH ---
    int initialMatches = numberOfStages; 
    int matchesInRound = initialMatches;

    int totalRounds = 0;
    for (int m = initialMatches; m > 0; m /= 2) {
        totalRounds++;
    }

    int totalRows = (initialMatches * 2) - 1;

    for (int row = 0; row < totalRows; ++row) {
        bracketLayout->setRowStretch(row, 1);
    }
    for (int col = 0; col < totalRounds; ++col) {
        bracketLayout->setColumnStretch(col, 1);
    }

    for (int round = 0; round < totalRounds; ++round) {
        int firstRowOffset = (1 << round) - 1;  
        int rowStep = 1 << (round + 1);         

        for (int i = 0; i < matchesInRound; ++i) {
            int targetRow = firstRowOffset + (i * rowStep);
            QWidget* komatch = new KoMatch(tournamentPage);
            bracketLayout->addWidget(komatch, targetRow, round, Qt::AlignCenter);
        }
        matchesInRound /= 2;
    }
    
    // HIER kannst du nun die connect()-Zonensignale für ui->teambox etc. anfügen...

if (!teams.empty()) {
        // Blockiert temporär die Signale, damit die Spielerliste nicht bei jedem addItem() neu gezeichnet wird
        ui->teambox->blockSignals(true);

        for (const auto& team : teams) {
            // Konvertiert den std::string Namen der Klasse in QString für die ComboBox
            ui->teambox->addItem(QString::fromStdString(team.getName()));
        }

        // Signale wieder freigeben
        ui->teambox->blockSignals(false);

        // Setzt den Fokus auf das erste Team (Index 0) und triggert manuell das Update der Spielerliste
        ui->teambox->setCurrentIndex(0);
        
        // Da currentIndexChanged durch das vorherige Blockieren nicht gefeuert hat,
        // rufen wir die Aktualisierung für das erste Team einmalig explizit auf:
        const Team& firstTeam = teams[0];
        for (const std::string& playerName : firstTeam.getPlayers()) {
            ui->playerlist->addItem(QString::fromStdString(playerName));
        }
    }

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

    // 2. Team hinzufügen
    connect(ui->btnnewteam, &QPushButton::clicked, this, [this]() {
        QString qTeamName = ui->linenewteam->text().trimmed();
        if (qTeamName.isEmpty()) return;
        
        // UUID-Generierung entsprechend deiner verwendeten Bibliothek einfügen
        uuids::uuid newId = uuids::uuid(); 
        
        Team newTeam(qTeamName.toStdString(), newId);
        teams.push_back(newTeam); // Schreibt direkt in den globalen Vektor
        
        ui->teambox->addItem(qTeamName);
        ui->linenewteam->clear();
        ui->teambox->setCurrentIndex(ui->teambox->count() - 1);
    });

    // 3. Team löschen
    connect(ui->btndeleteteam, &QPushButton::clicked, this, [this]() {
        int currentIndex = ui->teambox->currentIndex();
        if (currentIndex < 0 || currentIndex >= static_cast<int>(teams.size())) return;
        
        teams.erase(teams.begin() + currentIndex); // Löscht aus globalem Vektor
        ui->teambox->removeItem(currentIndex);
        
        if (teams.empty()) {
            ui->playerlist->clear();
        }
    });

    // 4. Spieler hinzufügen
    connect(ui->btnaddplayer, &QPushButton::clicked, this, [this]() {
        int currentTeamIndex = ui->teambox->currentIndex();
        if (currentTeamIndex < 0) return;
        
        QString qPlayerName = ui->teamsline->text().trimmed();
        if (qPlayerName.isEmpty()) return;
        
        teams[currentTeamIndex].addPlayer(qPlayerName.toStdString());
        
        ui->playerlist->addItem(qPlayerName);
        ui->teamsline->clear();
    });

    // 5. Spieler löschen
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

    // 6. Speichern-Button
    connect(ui->btnsave, &QPushButton::clicked, this, [this]() {
        qDebug() << "Globaler Vektor status - Anzahl Teams:" << teams.size();
    });
}
MainWindow::~MainWindow()
{
    delete ui;
}