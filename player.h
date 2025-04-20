// player.h
#ifndef PLAYER_H
#define PLAYER_H

#include <QLabel>
#include <QWidget>

class Player : public QLabel
{

    Q_OBJECT

public:

    explicit Player(QWidget *parent = nullptr);
    void moveTo(QLayout *layout, int row, int col);

};

#endif // PLAYER_H
