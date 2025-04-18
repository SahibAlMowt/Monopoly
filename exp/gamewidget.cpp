#include "gamewidget.h"
#include "ui_gamewidget.h"
#include "cell.h"
#include <QScreen>

GameWindow::GameWindow(QWidget *parent) :QDialog(parent),ui(new Ui::Game)
{
    ui -> setupUi(this);

  //  setFixedSize(1000, 900);

    QVector<CellInfo> cells =
    {
        { "1", 400, "../../pagani_zonda.jpg" },
        { "2", 400, "../../pagani_zonda.jpg" },
        { "3", 400, "../../pagani_zonda.jpg" },
        { "4", 400, "../../pagani_zonda.jpg" },
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
        { "##", 400, "../../pagani_zonda.jpg" },
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
        { "456", 400, "../../pagani_zonda.jpg" },
        { "Ok", 400, "../../pagani_zonda.jpg" },
        { "Ok", 400, "../../pagani_zonda.jpg" },
        { "Ok", 400, "../../pagani_zonda.jpg" },
        { "Ok", 400, "../../pagani_zonda.jpg" },
        { "Ok", 400, "../../pagani_zonda.jpg" },
        { "Ok", 400, "../../pagani_zonda.jpg" },
        { "Ok", 400, "../../pagani_zonda.jpg" },
        { "Ok", 400, "../../pagani_zonda.jpg" },
        { "Ok", 400, "../../pagani_zonda.jpg" },
        { "Start", 300, "../../pagani_zonda.jpg"},
        {"Jail", 0, "../../pagani_zonda.jpg" },
        { "Parking", 300, "../../pagani_zonda.jpg"},
        { "Go Jail", 300, "../../pagani_zonda.jpg"}
    };


    const int size = 10;
    QGridLayout *grid = new QGridLayout(this);

    // где-нибудь в коде, например, в конструкторе:
    QScreen *screen = QGuiApplication::primaryScreen();
    QRect screenGeometry = screen->geometry();

    int screenWidth = screenGeometry.width();
    int screenHeight = screenGeometry.height();
    int otstup = (screenWidth - screenHeight) / 2;
    int cellWidth = screenWidth/14;

    //             CellWidget(const CellInfo &info, QWidget *parent = nullptr);

    /*CellWidget *cellHorizontal = new CellWidget();
    cell->setFixedSize(cellWidth*2, cellWidth);

    CellWidget *cellVertical = new CellWidget();
    cell->setFixedSize(cellWidth, cellWidth*2);

    CellWidget *cellCorner = new CellWidget();
    cell->setFixedSize(cellWidth*2, cellWidth*2);*/

    grid -> setContentsMargins(otstup, 0, 0, 0);
    grid->addWidget(new CellWidget(cells[40]), 0);



    //for(int i=0; i!=10; i++){}

    /*grid -> setSpacing(0);
    grid -> setContentsMargins(50, 50, 50, 50);

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
    }*/

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
