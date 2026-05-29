#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGridLayout>
#include <QString>
#include <vector>
#include "classes/team.hpp"
#include "classes/match.hpp"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    // Methode zur dynamischen Aktualisierung der UI bei Datenempfang
    void updateTournamentUi();

signals:
    // KORREKTUR: Signal zur Weiterleitung der Update-Anforderung an die main.cpp/NetworkController
    void requestMatchUpdate(const QString &matchId, int score1, int score2, int status);

private:
    Ui::MainWindow *ui;
    QGridLayout *bracketLayout = nullptr; 
    QWidget *tournamentPage = nullptr;     
};

#endif // MAINWINDOW_H