#ifndef GAME_WIDGET_H
#define GAME_WIDGET_H

#include <QWidget>

class GameController;

class GameWidget : public QWidget
{
    Q_OBJECT

public:

    GameWidget(GameController *controller, QWidget *parent = 0);

protected:

    void paintEvent(QPaintEvent *event) override;

private:

    GameController *controller;

};

#endif // GAME_WIDGET_H
