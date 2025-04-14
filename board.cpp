#include "board.h"
#include "property_cell.h"

Board::Board() {
    // Простой пример: поле из 5 клеток, можно расширить
    cells.push_back(new PropertyCell("Start Street", 0, 100, 10));
    cells.push_back(new PropertyCell("Blue Street", 1, 120, 12));
    cells.push_back(new PropertyCell("Red Street", 2, 140, 14));
    cells.push_back(new PropertyCell("Green Street", 3, 160, 16));
    cells.push_back(new PropertyCell("Black Street", 4, 180, 18));
}

Board::~Board() {
    for (Cell* cell : cells)
        delete cell;
}

Cell* Board::getCell(int index) const {
    return cells.at(index % cells.size());
}

int Board::size() const {
    return static_cast<int>(cells.size());
}
