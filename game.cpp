#include "game.h"
#include "board.h"
#include "player.h"
#include "cell.h"

#include <iostream>
#include <random>
#include <ctime>

GameController::GameController() {
    board = new Board();
    currentPlayerIndex = 0;
    std::srand(static_cast<unsigned int>(std::time(nullptr))); // инициализация генератора случайных чисел
}

GameController::~GameController() {
    delete board;
    for (Player* player : players)
        delete player;
}

void GameController::addPlayer(const QString& name) {
    players.push_back(new Player(name));
}

void GameController::startGame() {
    std::cout << "Игра началась!\n";
    while (true) {
        nextTurn();
        std::cout << "Нажмите Enter для следующего хода...\n";
        std::cin.get();
    }
}

void GameController::nextTurn() {
    if (players.empty()) return;

    Player* player = players[currentPlayerIndex];
    if (player->isBankrupt()) {
        std::cout << player->getName().toStdString() << " — банкрот и пропускает ход.\n";
        currentPlayerIndex = (currentPlayerIndex + 1) % players.size();
        return;
    }

    int roll = rollDice();
    std::cout << player->getName().toStdString() << " бросает кубик: " << roll << "\n";
    player->move(roll, board->size());

    Cell* cell = board->getCell(player->getPosition());
    std::cout << player->getName().toStdString() << " попадает на " << cell->getName().toStdString() << "\n";
    cell->onLand(player);

    std::cout << "Баланс: " << player->getMoney() << "$\n";

    currentPlayerIndex = (currentPlayerIndex + 1) % players.size();
}

int GameController::rollDice() const {
    return (std::rand() % 6 + 1) + (std::rand() % 6 + 1); // два кубика
}

Board* GameController::getBoard() const {
    return board;
}

const std::vector<Player*>& GameController::getPlayers() const {
    return players;
}

