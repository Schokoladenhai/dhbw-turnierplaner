#pragma once

#include <QWidget>
#include <QString>

namespace Ui {
    class MatchWidget;
}

class MatchWidget : public QWidget {
    Q_OBJECT

public:
    explicit MatchWidget(QWidget *parent = nullptr);
    ~MatchWidget();

    // Methode, um die Teamnamen von außen zu setzen
    void setTeams(const QString& team1, const QString& team2);

    // Getter für die aktuellen Tore
    int getScoreTeam1() const;
    int getScoreTeam2() const;

signals:
    /**
     * NEU: Dieses Signal schickt die Tore direkt als Parameter mit.
     * So muss der Empfänger (das GroupWidget) nicht erst die Getter aufrufen.
     */
    void submitClicked(int score1, int score2);

private slots:
    /**
     * NEU: Reagiert auf den Klick deines 'pushButton'.
     */
    void on_pushButtonSubmit_clicked();

    /**
     * Reagiert auf Änderungen in den Spinboxen (um den Button wieder zu aktivieren).
     */
    void onSpinBoxValueChanged();

private:
    Ui::MatchWidget *ui;
};