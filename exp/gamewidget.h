#ifndef GAMEWIDGET_H
#define GAMEWIDGET_H

#include <QDialog>
#include <QHBoxLayout>

namespace Ui
{
    class Game;
}

class GameWindow : public QDialog
{
    Q_OBJECT

public:

    explicit GameWindow(QWidget *parent = nullptr);
    ~GameWindow();

signals:

    void return_to_menu();

private slots:

    void on_quit_button_clicked();

private:

    Ui::Game *ui;

    QWidget *central;
    QHBoxLayout *bottomRowLayout;

};


#endif // GAMEWIDGET_H
