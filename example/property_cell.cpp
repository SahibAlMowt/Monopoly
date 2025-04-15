#include "property_cell.h"
#include "player.h"

PropertyCell::PropertyCell(const QString& name, int index, int price, int rent): Cell(name, index), price(price), rent(rent), owner(0) {}

void PropertyCell::onLand(Player* player)
{
    if (!owner)
    {
        if (player -> getMoney() >= price)
        {
            player -> adjustMoney(-price);
            owner = player;
            player -> addProperty(this);
        }
    }
    else if (owner != player)
    {
        player -> adjustMoney(-rent);
        owner -> adjustMoney(rent);
    }
}

Player *PropertyCell::getOwner() const
{
    return owner;
}
