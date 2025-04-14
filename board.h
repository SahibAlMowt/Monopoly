#ifndef BOARD_H
#define BOARD_H

#include <vector>

class Cell;

class Board {
public:
    Board();
    ~Board();

    Cell* getCell(int index) const;
    int size() const;

private:
    std::vector<Cell*> cells;
};

#endif // BOARD_H
