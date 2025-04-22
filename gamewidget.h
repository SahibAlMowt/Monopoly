#ifndef GAMEWIDGET_H
#define GAMEWIDGET_H

#include <QDialog>
#include <QHBoxLayout>
#include <QVector>
#include <QLabel>

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
    void next_player();
    void updatePlayerTurnLabel();

private:

    Ui::Game *ui;

    QWidget *central;
    QHBoxLayout *bottomRowLayout;

    QVector<Player*> players;
    QVector<int> playerPositions;
    int currentPlayerIndex = 0;
    int playerCount;

    QLabel *playerTurnLabel;

    QVector<QPair<int, int>> path;

    QLabel *cube_label_1;
    QLabel *cube_label_2;

    QGridLayout *boardLayout;
};


#endif // GAMEWIDGET_H
