#ifndef PLAYER_H
#define PLAYER_H

#include <QString>
#include <vector>

class Cell;

class Player {
public:
    Player(const QString& name);

    void move(int steps, int boardSize);
    void adjustMoney(int amount);
    void addProperty(Cell* property);

    const QString& getName() const;
    int getMoney() const;
    int getPosition() const;
    bool isBankrupt() const;

private:
    QString name;
    int money;
    int position;
    bool bankrupt;
    std::vector<Cell*> properties;
};

#endif // PLAYER_H
