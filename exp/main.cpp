#include "mainwindow.h"

#include "audio.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

 //   AudioPlayer audio;

  //  audio.playSound("../../mortals.mp3");

    MainWindow w;

    w.show();

    return a.exec();
}
