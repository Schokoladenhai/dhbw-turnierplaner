#ifndef KOMATCH_H
#define KOMATCH_H

#include <QWidget>

namespace Ui {
class KoMatch;
}

class KoMatch : public QWidget
{
    Q_OBJECT

public:
    explicit KoMatch(QWidget *parent = nullptr);
    ~KoMatch();

private:
    Ui::KoMatch *ui;
};

#endif // KOMATCH_H
