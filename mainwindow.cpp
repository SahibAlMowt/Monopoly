#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "gamewidget.h"

#include "playerdialog.h"

//#include <QDebug>

#include <QFileInfo>


MainWindow::MainWindow(QWidget *parent): QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui -> setupUi(this);

    player = new QMediaPlayer(this);


    connect(ui -> play_audio, &QPushButton::clicked, this, &MainWindow::play_audio);

    connect(ui -> stop_audio, &QPushButton::clicked, this, &MainWindow::stop_audio);


    ui -> slide_audio -> setValue(50);
    connect(ui -> slide_audio, &QSlider::valueChanged, this, &MainWindow::set_volume_audio);

    QFileInfo file_info("../../resources/music.mp3");

    if (!file_info.exists())
    {
        qDebug() << "file donk exists" << file_info.absoluteFilePath();
    }

    QUrl url = QUrl::fromLocalFile(file_info.absoluteFilePath());

    player->setMedia(QMediaContent(url));
    player -> setVolume(50);

    connect(ui -> start_game_button, &QPushButton::clicked, this, &MainWindow::start_game_button);

    connect(ui -> quit_main_menu, &QPushButton::clicked, qApp, &QApplication::quit);

    connect(player, &QMediaPlayer::mediaStatusChanged, this, &MainWindow::media_status);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::start_game_button()
{
    PlayerSelectDialog dialog(this);

    if (dialog.exec() == QDialog::Accepted && !gameWindow)
    {
        int playerCount = dialog.getPlayerCount();

        gameWindow = new GameWindow(playerCount, nullptr);

        connect(gameWindow, &GameWindow::return_to_menu, this, &MainWindow::show);
    }

    gameWindow -> show();

    this -> hide();
}



void MainWindow::play_audio()
{
    player -> play();
}

void MainWindow::stop_audio()
{
    player -> stop();
    player -> setPosition(0);
}

void MainWindow::set_volume_audio(int volume)
{
    player -> setVolume(volume);
}

void MainWindow::media_status(QMediaPlayer::MediaStatus status)
{
    if (status == QMediaPlayer::EndOfMedia)
    {
        player -> play();
    }
}


