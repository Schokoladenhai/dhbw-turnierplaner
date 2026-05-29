#include "windows/komatch.h"
#include "ui_komatch.h"
#include "uuid.h" 

KoMatch::KoMatch(const Match &match, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::KoMatch)
    , m_match(match) 
{
    ui->setupUi(this);

    // Datenbindung aus dem persistenten Modell an die UI-Komponenten
    ui->t1name->setText(QString::fromStdString(m_match.team1.getName()));
    ui->t2name->setText(QString::fromStdString(m_match.team2.getName()));
    ui->t1score->setValue(m_match.score[0]);
    ui->t2score->setValue(m_match.score[1]);

    // Präzise Drei-Wege-Zustandsmaschine über m_state
    if (m_match.status == "FINISHED") {
        m_state = "closed";
        ui->pushButton->setText("Reset");
        ui->pushButton->setEnabled(true);   // Button aktiv für den Übergang zu RUNNING
        ui->t1score->setEnabled(false);
        ui->t2score->setEnabled(false);
    } 
    else if (m_match.status == "RUNNING") {
        m_state = "ready_to_finish";
        ui->pushButton->setText("Send");
        ui->pushButton->setEnabled(true);   // Button aktiv für den Übergang zu FINISHED
        ui->t1score->setEnabled(true);       // Punkteboxen freigeschaltet
        ui->t2score->setEnabled(true);
    } 
    else { 
        // Greift bei "WAITING" oder leerem Initial-Status
        m_state = "ready_to_start";
        ui->pushButton->setText("Start");
        ui->pushButton->setEnabled(true);   // WICHTIG: Button MUSS aktiv sein, um das Match zu starten!
        ui->t1score->setEnabled(false);      // Punkteboxen blockiert, da Spiel noch nicht läuft
        ui->t2score->setEnabled(false);
    }
}

KoMatch::~KoMatch()
{
    delete ui;
}

void KoMatch::on_pushButton_clicked()
{
    QString matchIdStr = QString::fromStdString(uuids::to_string(m_match.id));

    if (m_state == "ready_to_start") {
        // Zustand: WAITING -> RUNNING
        // Schaltet das Match auf dem Server aktiv (Status-Enum 1)
        emit matchResultChanged(matchIdStr, 0, 0, 1);
        ui->t1score->setEnabled(true);       // Punkteboxen freigeschaltet
        ui->t2score->setEnabled(true);

    } 
    else if (m_state == "ready_to_finish") {
        // Zustand: RUNNING -> FINISHED
        // Übermittelt das eingetragene Ergebnis an das Backend (Status-Enum 2)
        int score1 = ui->t1score->value();
        int score2 = ui->t2score->value();
        emit matchResultChanged(matchIdStr, score1, score2, 2);
        ui->t1score->setEnabled(false);      // Punkteboxen blockiert, da Spiel abgeschlossen ist
        ui->t2score->setEnabled(false);
    } 
    else if (m_state == "closed") {
        // Zustand: FINISHED -> RUNNING
        // Setzt ein beendetes Match für Korrekturen zurück auf aktiv (Status-Enum 1)
        emit matchResultChanged(matchIdStr, 0, 0, 1);
        ui->t1score->setEnabled(true);       // Punkteboxen freigeschaltet
        ui->t2score->setEnabled(true);
    }
}