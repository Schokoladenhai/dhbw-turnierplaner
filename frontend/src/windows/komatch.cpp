#include "windows/komatch.h"
#include "ui_komatch.h"

KoMatch::KoMatch(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::KoMatch)
{
    ui->setupUi(this);
}

KoMatch::~KoMatch()
{
    delete ui;
}
