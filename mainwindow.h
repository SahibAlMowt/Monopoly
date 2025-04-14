#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class GameController;
class GameWidget;

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onRollDiceClicked();

private:
    Ui::MainWindow *ui;
    GameController* gameController;
    GameWidget* gameWidget;
};

#endif // MAINWINDOW_H
