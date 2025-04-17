#include "gamewidget.h"
#include "ui_gamewidget.h"
#include "cell.h"

GameWindow::GameWindow(QWidget *parent) :QDialog(parent),ui(new Ui::Dialog)
{
    ui -> setupUi(this);

  //  setFixedSize(1000, 900);

    QVector<CellInfo> cells =
    {
        { "Ok", 400, "../../pagani_zonda.jpg" },
        { "Ok", 400, "../../pagani_zonda.jpg" },
        { "Ok", 400, "../../pagani_zonda.jpg" },
        { "Ok", 400, "../../pagani_zonda.jpg" },
        { "Ok", 400, "../../pagani_zonda.jpg" },
        { "Ok", 400, "../../pagani_zonda.jpg" },
        { "Ok", 400, "../../pagani_zonda.jpg" },
        { "Ok", 400, "../../pagani_zonda.jpg" },
        { "Ok", 400, "../../pagani_zonda.jpg" },
        { "Ok", 400, "../../pagani_zonda.jpg" },
        { "Ok", 400, "../../pagani_zonda.jpg" },
        { "Ok", 400, "../../pagani_zonda.jpg" },
        { "Ok", 400, "../../pagani_zonda.jpg" },
        { "Ok", 400, "../../pagani_zonda.jpg" },
        { "Ok", 400, "../../pagani_zonda.jpg" },
        { "Ok", 400, "../../pagani_zonda.jpg" },
        { "Ok", 400, "../../pagani_zonda.jpg" },
        { "Ok", 400, "../../pagani_zonda.jpg" },
        { "Ok", 400, "../../pagani_zonda.jpg" },
        { "Ok", 400, "../../pagani_zonda.jpg" },
        { "Ok", 400, "../../pagani_zonda.jpg" },
        { "Ok", 400, "../../pagani_zonda.jpg" },
        { "Ok", 400, "../../pagani_zonda.jpg" },
        { "Ok", 400, "../../pagani_zonda.jpg" },
        { "Ok", 400, "../../pagani_zonda.jpg" },
        { "Ok", 400, "../../pagani_zonda.jpg" },
        { "Ok", 400, "../../pagani_zonda.jpg" },
        { "Ok", 400, "../../pagani_zonda.jpg" },
        { "Ok", 400, "../../pagani_zonda.jpg" },
        { "Ok", 400, "../../pagani_zonda.jpg" },
        { "Ok", 400, "../../pagani_zonda.jpg" },
        { "Ok", 400, "../../pagani_zonda.jpg" },
        { "Ok", 400, "../../pagani_zonda.jpg" },
        { "Ok", 400, "../../pagani_zonda.jpg" },
        { "Ok", 400, "../../pagani_zonda.jpg" },
        { "Ok", 400, "../../pagani_zonda.jpg" },
        { "Ok", 400, "../../pagani_zonda.jpg" },
        { "Ok", 400, "../../pagani_zonda.jpg" },
        { "Ok", 400, "../../pagani_zonda.jpg" },
        { "Ok", 400, "../../pagani_zonda.jpg" }
    };

    const int size = 10;
    QGridLayout *grid = new QGridLayout(this);

    grid -> setSpacing(0);
    grid -> setContentsMargins(5, 5, 5, 5);

    int index = 0;

    for (int col = size - 1; col >= 0; col--)
    {
        CellType type = (col == size - 1 || col == 0) ? CellType::Corner : CellType::Horizontal;
        grid -> addWidget(new CellWidget(cells[index], type), size - 1, col);
        index++;
    }

    for (int row = size - 2; row >= 0; row--)
    {
        CellType type = (row == 0) ? CellType::Corner : CellType::Vertical;
        grid -> addWidget(new CellWidget(cells[index], type), row, 0);
        index++;
    }

    for (int col = 1; col < size; col++)
    {
        CellType type = (col == size - 1) ? CellType::Corner : CellType::Horizontal;
        grid -> addWidget(new CellWidget(cells[index], type), 0, col);
        index++;
    }

    for (int row = 1; row < size - 1; row++)
    {
        CellType type = CellType::Vertical;
        grid -> addWidget(new CellWidget(cells[index], type), row, size - 1);
        index++;
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
