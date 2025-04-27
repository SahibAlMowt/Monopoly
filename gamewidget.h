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

struct PlayerState {
    int money;
    QVector<int> properties;
    QVector<int> mortgagedProperties;
    bool inJail;
    int jailTurns;
    bool hasJailCard;
    int houseCount;
    int hotelCount;
    bool bankrupt;

    PlayerState() :
        money(1500),
        inJail(false),
        jailTurns(0),
        hasJailCard(false),
        houseCount(0),
        hotelCount(0),
        bankrupt(false)
    {}
};

struct Card {
    QString text;
    CardType type;
    int value;
    int targetPosition;
    bool keepCard;

    Card(const QString& t, CardType ct, int v = 0, int tp = -1, bool kc = false) :
        text(t),
        type(ct),
        value(v),
        targetPosition(tp),
        keepCard(kc)
    {}
};

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

public slots:

    void select_cell(int index);

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
    TreasuryDialog *treasury_dialog;
    QWidget *kaznaWidget = nullptr;
    QLabel *kaznaLabel = nullptr;
    QGridLayout *mainLayout;
    QVector<CellInfo> cells;

    // Новые поля для игровой логики
    QVector<PlayerState> playerStates;
    QVector<Card> treasuryCards;
    QVector<Card> chanceCards;
    QWidget* playerInfoWidget = nullptr;
    QVector<QLabel*> playerLabels;

    // Методы инициализации и игровой логики
    int GetCellIndex(int);
    void initializeCards();
    void initializeCells();
    void createPlayerInfoWidget();
    void processCard(const Card& card);
    void buyProperty(int cellIndex);
    void payRent(int cellIndex);
    void mortgageProperty(int cellIndex);
    void unmortgageProperty(int cellIndex);
    void buildHouse(int cellIndex);
    void updatePlayerInfoDisplay();
    bool handleBankruptcy();
    int calculateRent(int cellIndex);
    bool playerOwnsAllPropertiesInGroup(int playerId, const QString& colorGroup);

    // Методы обработки игровых действий
    void check_cell_type();
    void show_treasure_dialog();
    void show_treasure_card_in_tab();

    QVector<CellWidget*> cell_vec;

    int selected_cell_index = -1;


};

#endif // GAMEWIDGET_H
