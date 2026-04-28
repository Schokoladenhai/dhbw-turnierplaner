#include "gui/windows/MainWindow.hpp"
#include "ui_MainWindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Wir erstellen ein vertikales Layout im zentralen Bereich des Fensters
    m_contentLayout = new QVBoxLayout(ui->centralwidget);

    // Testweise ein paar MatchWidgets hinzufügen
    for(int i = 0; i < 3; ++i) {
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