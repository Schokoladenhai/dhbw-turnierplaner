#ifndef KOMATCH_H
#define KOMATCH_H

#include <QWidget>
#include <QString>
#include "classes/match.hpp"

namespace Ui {
class KoMatch;
}

class KoMatch : public QWidget
{
    Q_OBJECT

public:
    explicit KoMatch(const Match &match, QWidget *parent = nullptr);
    ~KoMatch();

signals:
    void matchResultChanged(const QString &matchId, int score1, int score2, int status);

private slots:
    void on_pushButton_clicked();

private:
    Ui::KoMatch *ui;
    Match m_match;
    QString m_state = "open"; 
};

#endif // KOMATCH_H