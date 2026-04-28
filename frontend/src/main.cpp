#include <QApplication>
#include <QPushButton>
#include <QDebug> // Wichtig für Textausgaben
#include "main.hpp"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    // Qt's Version von std::cout
    qDebug() << "Frontend-App gestartet!";

    MainWindow w;
    w.show();

    return app.exec();
}