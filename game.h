#ifndef GAMECONTROLLER_H
#define GAMECONTROLLER_H

#include <vector>
#include <QString>

class Player;
class Board;

class GameController {
public:
    GameController();
    ~GameController();

    void addPlayer(const QString& name);
    void startGame();
    void nextTurn();
    const std::vector<Player*>& getPlayers() const;
    Board* getBoard() const;


private:
    Board* board;
    std::vector<Player*> players;
    int currentPlayerIndex;

    int rollDice() const;
};

#endif // GAMECONTROLLER_H
