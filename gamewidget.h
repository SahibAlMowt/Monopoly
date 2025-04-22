#ifndef GAMEWIDGET_H
#define GAMEWIDGET_H

#include <QDialog>
#include <QHBoxLayout>
#include <QVector>
#include <QLabel>

#include "player.h"
#include "cell.h"

namespace Ui
{
    class Game;
}

class TreasuryDialog : public QDialog
{
    Q_OBJECT

public:

    explicit TreasuryDialog(QWidget *parent = nullptr);
    void show_random_card();

private:

    QLabel *cardTextLabel;
    QPushButton *okButton;
    QVector<QString> treasury_cards;
};

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

    void check_cell_type();
    TreasuryDialog *treasury_dialog;
    void show_treasure_dialog();

    QWidget *kaznaWidget = nullptr;
    QLabel *kaznaLabel = nullptr;

    void show_treasure_card_in_tab();

    QGridLayout *mainLayout;

    QVector<CellInfo> cells;
};





#endif // GAMEWIDGET_H
