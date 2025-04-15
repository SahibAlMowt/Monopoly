#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "game.h"
#include "game_widget.h"

#include <QVBoxLayout>
#include <QPushButton>

MainWindow::MainWindow(QWidget *parent): QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui -> setupUi(this);

    gameController = new GameController();
    gameController -> addPlayer("Player 1");
    gameController -> addPlayer("Player 2");

    gameWidget = new GameWidget(gameController, this);

    QPushButton *rollButton = new QPushButton("Cube", this);
    connect(rollButton, &QPushButton::clicked, this, &MainWindow::onRollDiceClicked);

    QWidget *central = new QWidget(this);
    QVBoxLayout *layout = new QVBoxLayout(central);
    layout -> addWidget(gameWidget);
    layout -> addWidget(rollButton);
    setCentralWidget(central);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete gameController;
}

void MainWindow::onRollDiceClicked()
{
    gameController -> nextTurn();
    gameWidget -> update();
}
