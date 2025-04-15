#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "gamewidget.h"


MainWindow::MainWindow(QWidget *parent): QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->start_game_button, &QPushButton::clicked, this, &MainWindow::start_game_button);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::start_game_button()
{
    if (!gameWindow)
    {
        gameWindow = new GameWindow(this);

        connect(gameWindow, &GameWindow::return_to_menu, this, &MainWindow::show);
    }

    gameWindow -> show();

    this -> hide();
}
