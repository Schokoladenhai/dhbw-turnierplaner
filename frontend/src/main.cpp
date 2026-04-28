#include <QApplication>
#include <QPushButton>
#include <QDebug> // Wichtig für Textausgaben

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    // Qt's Version von std::cout
    qDebug() << "Frontend-App gestartet!";

    QPushButton button("Hallo DHBW!");
    button.show();

    return app.exec();
}