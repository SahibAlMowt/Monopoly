#ifndef PLAYER_H
#define PLAYER_H

#include <QLabel>
#include <QWidget>

class Player : public QLabel
{
    Q_OBJECT

public:
    enum PlayerColor {
        Red,
        Blue,
        Green,
        Yellow
    };

    explicit Player(PlayerColor color = Red, QWidget *parent = nullptr);
    void moveTo(QLayout *layout, int row, int col);
    PlayerColor getColor() const { return playerColor; }

private:
    PlayerColor playerColor;
};

#endif // PLAYER_H
