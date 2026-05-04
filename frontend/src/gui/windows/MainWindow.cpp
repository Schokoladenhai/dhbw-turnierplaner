#include "gui/windows/MainWindow.hpp"
#include "ui_MainWindow.h"
#include "classes/match.hpp"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Wir erstellen ein vertikales Layout im zentralen Bereich des Fensters
    m_contentLayout = new QVBoxLayout(ui->centralwidget);

    Match match1("Match 1", uuids::uuid{});
    Team teamA("Team A", uuids::uuid{});
    Team teamB("Team B", uuids::uuid{});
    match1.addTeam(teamA);
    match1.addTeam(teamB);
    MatchWidget* matchWidget1 = new MatchWidget(this);
    matchWidget1->setTeams(
    QString::fromStdString(teamA.getName()), 
    QString::fromStdString(teamB.getName())
    );
    m_contentLayout->addWidget(matchWidget1);
    // Testweise ein paar MatchWidgets hinzufügen
    for(int i = 0; i < 10; ++i) {
        MatchWidget* match = new MatchWidget(this);
        match->setTeams("Team " + QString::number(i*2+1), "Team " + QString::number(i*2+2));
        
        m_contentLayout->addWidget(match);
        m_matches.push_back(match);
    }

    // Drückt alle Widgets nach oben, damit sie nicht über das ganze Fenster gestreckt werden
    m_contentLayout->addStretch();
}

MainWindow::~MainWindow() {
    delete ui;
}