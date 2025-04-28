#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QApplication>
#include <QMediaPlayer>
#include <QSlider>
#include <QMediaPlayer>
#include <QSlider>

#include "gamewidget.h"

QT_BEGIN_NAMESPACE

namespace Ui
{
    class MainWindow;
}

QT_END_NAMESPACE

class MainWindow : public QMainWindow
{

    Q_OBJECT

public:

    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:

    void start_game_button();

    void play_audio();
    void stop_audio();
    void set_volume_audio(int volume);

    void media_status(QMediaPlayer::MediaStatus status);

private:

    Ui::MainWindow *ui;
    GameWindow *gameWindow = nullptr;

    QMediaPlayer *player;

};
#endif // MAINWINDOW_H
