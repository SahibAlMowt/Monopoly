#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "gamewidget.h"

#include <QDebug>


MainWindow::MainWindow(QWidget *parent): QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui -> setupUi(this);

    player = new QMediaPlayer(this);


    connect(ui -> play_audio, &QPushButton::clicked, this, &MainWindow::play_audio);

    connect(ui -> stop_audio, &QPushButton::clicked, this, &MainWindow::stop_audio);


    ui -> slide_audio -> setValue(50);
    connect(ui -> slide_audio, &QSlider::valueChanged, this, &MainWindow::set_volume_audio);

    player->setMedia(QUrl::fromLocalFile("../../mortals.mp3"));
    player->setVolume(50);

    connect(ui -> start_game_button, &QPushButton::clicked, this, &MainWindow::start_game_button);

    connect(ui -> quit_main_menu, &QPushButton::clicked, qApp, &QApplication::quit);
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

    gameWindow -> showFullScreen();

    this -> hide();
}

void MainWindow::play_audio()
{
    player -> play();
}

void MainWindow::stop_audio()
{
    player->stop();
}

void MainWindow::set_volume_audio(int volume)
{
    player -> setVolume(volume);
}

