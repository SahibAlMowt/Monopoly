#include "game.h"
#include "board.h"
#include "player.h"
#include "cell.h"

#include <random>
#include <ctime>

GameController::GameController()
{
    board = new Board();
    currentPlayerIndex = 0;
    std::srand(static_cast<unsigned int>(std::time(nullptr)));
}

GameController::~GameController()
{
    delete board;
    for (Player *player : players)
    {
        delete player;
    }
}

void GameController::addPlayer(const QString &name)
{
    players.push_back(new Player(name));
}

void GameController::startGame()
{
    while (true)
    {
        nextTurn();
    }
}

void GameController::nextTurn()
{
    if (players.empty())
    {
        return;
    }

    Player *player = players[currentPlayerIndex];

    if (player->isBankrupt())
    {
        currentPlayerIndex = (currentPlayerIndex + 1) % players.size();
        return;
    }

    int roll = rollDice();

    player -> move(roll, board -> size());

    Cell *cell = board -> getCell(player -> getPosition());

    cell -> onLand(player);

    currentPlayerIndex = (currentPlayerIndex + 1) % players.size();
}

int GameController::rollDice() const
{
    return (std::rand() % 6 + 1) + (std::rand() % 6 + 1);
}

Board *GameController::getBoard() const
{
    return board;
}

const std::vector<Player*> &GameController::getPlayers() const
{
    return players;
}

