#include "property_cell.h"
#include "player.h"
#include <iostream> // только для тестов, в Qt уберём

PropertyCell::PropertyCell(const QString& name, int index, int price, int rent)
    : Cell(name, index), price(price), rent(rent), owner(nullptr) {}

void PropertyCell::onLand(Player* player) {
    if (!owner) {
        if (player->getMoney() >= price) {
            player->adjustMoney(-price);
            owner = player;
            player->addProperty(this);
            std::cout << player->getName().toStdString() << " купил " << name.toStdString() << "\n";
        }
    } else if (owner != player) {
        player->adjustMoney(-rent);
        owner->adjustMoney(rent);
        std::cout << player->getName().toStdString() << " заплатил аренду " << rent << "\n";
    }
}

Player* PropertyCell::getOwner() const {
    return owner;
}
