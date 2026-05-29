#include "windows/komatch.h"
#include "ui_komatch.h"
#include "uuid.h" 

KoMatch::KoMatch(const Match &match, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::KoMatch)
    , m_match(match) 
{
    ui->setupUi(this);

    ui->t1name->setText(QString::fromStdString(m_match.team1.getName()));
    ui->t2name->setText(QString::fromStdString(m_match.team2.getName()));
    ui->t1score->setValue(m_match.score[0]);
    ui->t2score->setValue(m_match.score[1]);

    // Identifikation von Freilosen / unvollständigen Paarungen via Null-UUID
    std::string nullUuid = "00000000-0000-0000-0000-000000000000";
    bool t1Missing = (uuids::to_string(m_match.team1.getId()) == nullUuid || m_match.team1.getName().empty());
    bool t2Missing = (uuids::to_string(m_match.team2.getId()) == nullUuid || m_match.team2.getName().empty());
    bool isIncomplete = (t1Missing != t2Missing); // Genau ein Gegner fehlt noch

    // Drei-Wege-Zustandsmaschine inklusive flexibler Freilos-Logik
    if (m_match.status == "FINISHED") {
        m_state = "closed";
        ui->pushButton->setText("Beendet");
        ui->pushButton->setEnabled(false); 
        ui->t1score->setEnabled(false);      
        ui->t2score->setEnabled(false);
    } 
    else if (m_match.status == "SKIPPED" || m_match.status == "SKIPED") {
        // Freilos ist aktiv: Das vorhandene Team ist weiter, Match bleibt manipulierbar
        m_state = "bye_active";
        ui->pushButton->setText("Aufheben");
        ui->pushButton->setEnabled(true);   
        ui->t1score->setEnabled(false);      
        ui->t2score->setEnabled(false);
    }
    else if (m_match.status == "RUNNING") {
        m_state = "ready_to_finish";
        ui->pushButton->setText("Send");
        ui->pushButton->setEnabled(true);   
        ui->t1score->setEnabled(true);       
        ui->t2score->setEnabled(true);
    } 
    else { 
        // Zustand: MATCH_WAITING
        if (isIncomplete) {
            // Sonderzustand: Match wartet auf Gegner, kann aber vorgezogen werden
            m_state = "ready_for_bye";
            ui->pushButton->setText("Freilos");
        } else {
            // Reguläres Match, das auf den manuellen Start wartet
            m_state = "ready_to_start";
            ui->pushButton->setText("Start");
        }
        ui->pushButton->setEnabled(true);   
        ui->t1score->setEnabled(false);      
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

    if (m_state == "ready_for_bye") {
        // Vorläufiges Freilos aktivieren: Sendet Status-Enum 3 (MATCH_SKIPED)
        // Punkte bleiben 0:0, das Backend wertet das vorhandene Team als Sieger
        emit matchResultChanged(matchIdStr, 0, 0, 3);
    }
    else if (m_state == "bye_active") {
        // Freilos wieder entziehen: Setzt das Match zurück auf MATCH_WAITING (0)
        // Dadurch rückt das Team in der Folgerunde im Backend wieder heraus
        emit matchResultChanged(matchIdStr, 0, 0, 0);
    }
    else if (m_state == "ready_to_start") {
        // Regulärer Spielstart (WAITING -> RUNNING)
        emit matchResultChanged(matchIdStr, 0, 0, 1);
    } 
    else if (m_state == "ready_to_finish") {
        // Regulärer Spielabschluss (RUNNING -> FINISHED)
        int score1 = ui->t1score->value();
        int score2 = ui->t2score->value();
        emit matchResultChanged(matchIdStr, score1, score2, 2);
    }
}