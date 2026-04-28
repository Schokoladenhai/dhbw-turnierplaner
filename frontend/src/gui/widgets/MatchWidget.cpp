#include "gui/widgets/MatchWidget.hpp"
#include "ui_MatchWidget.h"

MatchWidget::MatchWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MatchWidget)
{
    ui->setupUi(this);

    // Verknüpfung der Signale mit deinen spezifischen Objektnamen
    connect(ui->spinBox, QOverload<int>::of(&QSpinBox::valueChanged),
            this, &MatchWidget::onSpinBoxValueChanged);
            
    connect(ui->spinBox_2, QOverload<int>::of(&QSpinBox::valueChanged),
            this, &MatchWidget::onSpinBoxValueChanged);

    connect(ui->pushButton, &QPushButton::clicked,
            this, &MatchWidget::on_pushButtonSubmit_clicked);
}

MatchWidget::~MatchWidget() {
    delete ui;
}

void MatchWidget::setTeams(const QString& team1, const QString& team2) {
    ui->labelTeam1->setText(team1);
    ui->labelTeam2->setText(team2);
}

int MatchWidget::getScoreTeam1() const {
    return ui->spinBox->value(); // Dein linker Score
}

int MatchWidget::getScoreTeam2() const {
    return ui->spinBox_2->value(); // Dein rechter Score
}

void MatchWidget::on_pushButtonSubmit_clicked() {
    // Signal feuern mit den Werten aus deinen Spinboxen
    emit submitClicked(ui->spinBox->value(), ui->spinBox_2->value());
    
    ui->pushButton->setEnabled(false);
    ui->pushButton->setText("Gespeichert");
}

void MatchWidget::onSpinBoxValueChanged() {
    ui->pushButton->setEnabled(true);
    ui->pushButton->setText("Senden");
}