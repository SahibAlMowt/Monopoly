#include "player.h"
#include "cell.h"

Player::Player(const QString& name)
    : name(name), money(1500), position(0), bankrupt(false) {}

void Player::move(int steps, int boardSize) {
    position = (position + steps) % boardSize;
}

void Player::adjustMoney(int amount) {
    money += amount;
    if (money < 0) {
        bankrupt = true;
    }
}

void Player::addProperty(Cell* property) {
    properties.push_back(property);
}

const QString& Player::getName() const { return name; }
int Player::getMoney() const { return money; }
int Player::getPosition() const { return position; }
bool Player::isBankrupt() const { return bankrupt; }
