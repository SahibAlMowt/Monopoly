#include "cell.h"

Cell::Cell(const QString &name, int index) : name(name), index(index) {}

const QString &Cell::getName() const
{
    return name;
}

int Cell::getIndex() const
{
    return index;
}
