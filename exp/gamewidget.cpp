#include "gamewidget.h"
#include "ui_gamewidget.h"

GameWindow::GameWindow(QWidget *parent) :QDialog(parent),ui(new Ui::Dialog)
{
    ui -> setupUi(this);
}

GameWindow::~GameWindow()
{
    delete ui;
}

void GameWindow::on_quit_button_clicked()
{
    emit return_to_menu();
    this -> hide();
}
