#include "gamewidget.h"
#include "ui_gamewidget.h"
#include "cell.h"

GameWindow::GameWindow(QWidget *parent) :QDialog(parent),ui(new Ui::Dialog)
{
    ui -> setupUi(this);

    const int size = 10;

    QGridLayout *grid = new QGridLayout(this);
    grid->setSpacing(0);
    grid->setContentsMargins(0, 0, 0, 0);

    int index = 1;

    grid->addWidget(new CellWidget("Ячейка " + QString::number(0)), size - 1, size - 1);
    grid->addWidget(new CellWidget("Ячейка " + QString::number(9)), size - 1, 0);
    grid->addWidget(new CellWidget("Ячейка " + QString::number(18)), 0, 0);
    grid->addWidget(new CellWidget("Ячейка " + QString::number(27)), 0, size - 1);

    for (int i = size - 2; i > 0; i--)
    {
        grid->addWidget(new CellWidget("Ячейка " + QString::number(index++)), size - 1, i);
    }

    index++;

    for (int i = size - 2; i > 0; i--)
    {
        grid->addWidget(new CellWidget("Ячейка " + QString::number(index++)), i, 0);
    }

    index++;

    for (int i = 1; i < size - 1; i++)
    {
        grid->addWidget(new CellWidget("Ячейка " + QString::number(index++)), 0, i);
    }

    index++;

    for (int i = 1; i < size - 1; i++)
    {
        grid->addWidget(new CellWidget("Ячейка " + QString::number(index++)), i, size - 1);
    }
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
