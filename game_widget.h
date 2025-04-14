#ifndef GAMEWIDGET_H
#define GAMEWIDGET_H

#include <QWidget>

class GameController;

class GameWidget : public QWidget {
    Q_OBJECT

public:
    GameWidget(GameController* controller, QWidget *parent = nullptr);

protected:
    void paintEvent(QPaintEvent* event) override;

private:
    GameController* controller;
};

#endif // GAMEWIDGET_H
