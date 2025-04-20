#include "player.h"

#include <QGridLayout>

Player::Player(QWidget *parent) : QLabel(parent)
{
    setFixedSize(20, 20);
    setStyleSheet("background-color: red; border-radius: 10px;");
}

void Player::moveTo(QLayout *layout, int row, int col)
{
    QGridLayout *grid = qobject_cast<QGridLayout*>(layout);

    if (!grid)
    {
        return;
    }

    QLayoutItem *item = grid -> itemAtPosition(row, col);

    if (item && item->widget())
    {
        QWidget *cell = item->widget();
        this->setParent(cell);
        this->move(5, 5);
        this->show();
    }
}
