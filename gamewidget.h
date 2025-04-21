#ifndef GAMEWIDGET_H
#define GAMEWIDGET_H

#include <QDialog>
#include <QHBoxLayout>

#include "player.h"

namespace Ui
{
    class Game;
}

class GameWindow : public QDialog
{
    Q_OBJECT

public:

    explicit GameWindow(int playerCount = 1, QWidget *parent = nullptr);
    ~GameWindow();

signals:

    void return_to_menu();

private slots:

    void on_quit_button_clicked();
    void move_player(int steps);

    void start_cubes_roll();

private:

    Ui::Game *ui;

    QWidget *central;
    QHBoxLayout *bottomRowLayout;

    Player *player1;
    QVector<QPair<int, int>> path;
    int player_index = 0;

    QLabel *cube_label_1;
    QLabel *cube_label_2;

    QTimer *diceTimer;
    int animationStep = 0;
    int maxSteps = 6;
    int currentDiceValue = 1;

    QGridLayout *boardLayout;

    QVector<Player*> players;
    int currentPlayerIndex = 0;



};


#endif // GAMEWIDGET_H
