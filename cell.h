#ifndef CELL_H
#define CELL_H

#include <QString>

class Player;

class Cell {
public:
    Cell(const QString& name, int index);
    virtual ~Cell() = default;

    virtual void onLand(Player* player) = 0;

    const QString& getName() const;
    int getIndex() const;

protected:
    QString name;
    int index;
};

#endif // CELL_H
