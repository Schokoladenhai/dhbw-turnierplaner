#pragma once

#include <QMainWindow>
#include <QVBoxLayout>
#include <vector>
#include "gui/widgets/MatchWidget.hpp"

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    
    // Ein Layout, das unsere MatchWidgets untereinander stapelt
    QVBoxLayout *m_contentLayout;

    // Optional: Liste, um später einfach auf alle Matches zugreifen zu können
    std::vector<MatchWidget*> m_matches;
};