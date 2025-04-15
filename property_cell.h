#ifndef PROPERTY_CELL_H
#define PROPERTY_CELL_H

#include "cell.h"

class Player;

class PropertyCell : public Cell
{

public:

    PropertyCell(const QString &name, int index, int price, int rent);

    void onLand(Player *player) override;

    Player *getOwner() const;

private:

    int price;
    int rent;
    Player *owner;

};

#endif // PROPERTY_CELL_H
