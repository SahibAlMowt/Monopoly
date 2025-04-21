#pragma once

#include <QDialog>
#include <QPushButton>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QLabel>

class PlayerSelectDialog : public QDialog
{
    Q_OBJECT

public:

    explicit PlayerSelectDialog(QWidget *parent = nullptr) : QDialog(parent), playerCount(1)
    {
        setWindowTitle("Выбор количества игроков");

        QVBoxLayout *mainLayout = new QVBoxLayout(this);

        QLabel *label = new QLabel("Сколько игроков? (2-4)");
        label -> setAlignment(Qt::AlignCenter);
        mainLayout -> addWidget(label);

        QGridLayout *grid = new QGridLayout;

        QPushButton *button2 = new QPushButton("2 игрока");
        QPushButton *button3 = new QPushButton("3 игрока");
        QPushButton *button4 = new QPushButton("4 игрока");

        grid -> addWidget(button2, 0, 0);
        grid -> addWidget(button3, 0, 1);
        grid -> addWidget(button4, 0, 2);

        mainLayout -> addLayout(grid);

        connect(button2, &QPushButton::clicked, this, [=]() { playerCount = 2; accept(); });
        connect(button3, &QPushButton::clicked, this, [=]() { playerCount = 3; accept(); });
        connect(button4, &QPushButton::clicked, this, [=]() { playerCount = 4; accept(); });
    }

    int getPlayerCount() const
    {
        return playerCount;
    }

private:

    int playerCount;

};
