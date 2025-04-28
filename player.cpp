#include "player.h"

#include <QGridLayout>

Player::Player(PlayerColor color, QWidget *parent) : QLabel(parent), playerColor(color)
{
    setFixedSize(20, 20);
    
    QString colorStyle;
    switch (color) {
        case Red:
            colorStyle = "red";
            break;
        case Blue:
            colorStyle = "blue";
            break;
        case Green:
            colorStyle = "green";
            break;
        case Yellow:
            colorStyle = "yellow";
            break;
    }
    
    setStyleSheet("background-color: " + colorStyle + "; border-radius: 10px;");
}

void Player::moveTo(QLayout *layout, int row, int col)
{
    QGridLayout *grid = qobject_cast<QGridLayout*>(layout);

    if (!grid)
    {
        return;
    }

    QLayoutItem *item = grid -> itemAtPosition(row, col);

    if (item && item -> widget())
    {
        QWidget *cell = item -> widget();
        this -> setParent(cell);

        int offset = static_cast<int>(playerColor) * 5;
        this -> move(5 + offset, 5 + offset);
        this -> show();
    }
}
