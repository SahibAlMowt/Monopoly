#include "gamewidget.h"
#include "ui_gamewidget.h"
#include "cell.h"
#include <QScreen>
#include <QGridLayout>
#include <QPushButton>

GameWindow::GameWindow(QWidget *parent) : QDialog(parent), ui(new Ui::Game)
{
    ui->setupUi(this);

    // Get screen dimensions for responsive sizing
    QScreen *screen = QGuiApplication::primaryScreen();
    QRect screenGeometry = screen->geometry();
    int screenHeight = screenGeometry.height();

    // Make the base size as large as possible to fill the screen
    int baseSize = screenHeight / 12; // Larger base unit for maximum filling

    // Set this dialog to fill the screen completely and center the board
    setMinimumSize(screenGeometry.width(), screenGeometry.height());
    setStyleSheet("background-color: #f0f0f0;"); // Light background for any empty space

    // Make sure we're showing fullscreen
    showFullScreen();

    // Create game board cells data
    QVector<CellInfo> cells =
        {
            // Corner cells (indexed differently for clarity)
            { "GO", 0, "../../pagani_zonda.jpg" },      // Start (index 40)
            { "JAIL", 0, "../../pagani_zonda.jpg" },    // Jail (index 41)
            { "FREE PARKING", 0, "../../pagani_zonda.jpg" }, // Parking (index 42)
            { "GO TO JAIL", 0, "../../pagani_zonda.jpg" },   // Go to Jail (index 43)

            // Regular cells (0-39)
            { "1", 400, "../../pagani_zonda.jpg" },
            { "2", 400, "../../pagani_zonda.jpg" },
            { "3", 400, "../../pagani_zonda.jpg" },
            { "4", 400, "../../pagani_zonda.jpg" },
            { "5", 400, "../../pagani_zonda.jpg" },
            { "6", 400, "../../pagani_zonda.jpg" },
            { "7", 400, "../../pagani_zonda.jpg" },
            { "8", 400, "../../pagani_zonda.jpg" },
            { "9", 400, "../../pagani_zonda.jpg" },
            { "10", 400, "../../pagani_zonda.jpg" },
            { "11", 400, "../../pagani_zonda.jpg" },
            { "12", 400, "../../pagani_zonda.jpg" },
            { "13", 400, "../../pagani_zonda.jpg" },
            { "14", 400, "../../pagani_zonda.jpg" },
            { "15", 400, "../../pagani_zonda.jpg" },
            { "16", 400, "../../pagani_zonda.jpg" },
            { "17", 400, "../../pagani_zonda.jpg" },
            { "18", 400, "../../pagani_zonda.jpg" },
            { "19", 400, "../../pagani_zonda.jpg" },
            { "20", 400, "../../pagani_zonda.jpg" },
            { "21", 400, "../../pagani_zonda.jpg" },
            { "22", 400, "../../pagani_zonda.jpg" },
            { "23", 400, "../../pagani_zonda.jpg" },
            { "24", 400, "../../pagani_zonda.jpg" },
            { "25", 400, "../../pagani_zonda.jpg" },
            { "26", 400, "../../pagani_zonda.jpg" },
            { "27", 400, "../../pagani_zonda.jpg" },
            { "28", 400, "../../pagani_zonda.jpg" },
            { "29", 400, "../../pagani_zonda.jpg" },
            { "30", 400, "../../pagani_zonda.jpg" },
            { "31", 400, "../../pagani_zonda.jpg" },
            { "32", 400, "../../pagani_zonda.jpg" },
            { "33", 400, "../../pagani_zonda.jpg" },
            { "34", 400, "../../pagani_zonda.jpg" },
            { "35", 400, "../../pagani_zonda.jpg" },
            { "36", 400, "../../pagani_zonda.jpg" },
            { "37", 400, "../../pagani_zonda.jpg" },
            { "38", 400, "../../pagani_zonda.jpg" },
            { "39", 400, "../../pagani_zonda.jpg" },
            { "40", 400, "../../pagani_zonda.jpg" }
        };

    // Create main layout - stretched to fill the entire screen
    QGridLayout *mainLayout = new QGridLayout(this);
    mainLayout->setSpacing(0);
    mainLayout->setContentsMargins(0, 0, 0, 0); // Remove all margins to maximize space

    // Create game board grid
    QGridLayout *boardLayout = new QGridLayout();
    boardLayout->setSpacing(0);
    boardLayout->setContentsMargins(0, 0, 0, 0);

    // 1. Place corner cells
    CellWidget *cornerBottomRight = new CellWidget(cells[0]); // GO
    CellWidget *cornerBottomLeft = new CellWidget(cells[1]);  // JAIL
    CellWidget *cornerTopLeft = new CellWidget(cells[2]);     // FREE PARKING
    CellWidget *cornerTopRight = new CellWidget(cells[3]);    // GO TO JAIL

    // Set fixed size for corner cells
    int cornerSize = baseSize * 1.5;
    cornerBottomRight->setFixedSize(cornerSize, cornerSize);
    cornerBottomLeft->setFixedSize(cornerSize, cornerSize);
    cornerTopLeft->setFixedSize(cornerSize, cornerSize);
    cornerTopRight->setFixedSize(cornerSize, cornerSize);

    // Add corners to the grid
    boardLayout->addWidget(cornerBottomRight, 10, 10);
    boardLayout->addWidget(cornerBottomLeft, 10, 0);
    boardLayout->addWidget(cornerTopLeft, 0, 0);
    boardLayout->addWidget(cornerTopRight, 0, 10);

    // 2. Bottom row (right to left, excluding corners)
    for (int i = 1; i < 10; i++) {
        CellWidget *cell = new CellWidget(cells[i + 3]); // +3 offset for corner cells
        cell->setFixedSize(baseSize, baseSize * 1.5);
        boardLayout->addWidget(cell, 10, 10 - i);
    }

    // 3. Left column (bottom to top, excluding corners)
    for (int i = 1; i < 10; i++) {
        CellWidget *cell = new CellWidget(cells[i + 12]); // +12 offset from previous cells
        cell->setFixedSize(baseSize * 1.5, baseSize);
        boardLayout->addWidget(cell, 10 - i, 0);
    }

    // 4. Top row (left to right, excluding corners)
    for (int i = 1; i < 10; i++) {
        CellWidget *cell = new CellWidget(cells[i + 21]); // +21 offset from previous cells
        cell->setFixedSize(baseSize, baseSize * 1.5);
        boardLayout->addWidget(cell, 0, i);
    }

    // 5. Right column (top to bottom, excluding corners)
    for (int i = 1; i < 10; i++) {
        CellWidget *cell = new CellWidget(cells[i + 30]); // +30 offset from previous cells
        cell->setFixedSize(baseSize * 1.5, baseSize);
        boardLayout->addWidget(cell, i, 10);
    }

    // 6. Add central area for game logo/info
    QWidget *centralArea = new QWidget();
    centralArea->setStyleSheet("background-color: #EAEAEA;"); // Light gray background

    // Create a simple layout for the central area
    QVBoxLayout *centralLayout = new QVBoxLayout(centralArea);

    // Add a label with game name
    QLabel *gameTitle = new QLabel("MONOPOLY");
    gameTitle->setAlignment(Qt::AlignCenter);
    gameTitle->setStyleSheet("font-size: 24pt; font-weight: bold; color: #990000;");
    centralLayout->addWidget(gameTitle);

    // Add central area to the board
    boardLayout->addWidget(centralArea, 1, 1, 9, 9);

    // Add board layout to main layout - taking up the entire space with perfect centering
    // Use a spacer widget to center the board horizontally
    QWidget *boardContainer = new QWidget();
    boardContainer->setLayout(boardLayout);

    // Calculate necessary size to maintain square proportions while maximizing height
    int boardSize = screenHeight;
    boardContainer->setFixedSize(boardSize, boardSize);

    // Add the board container to the main layout with horizontal centering
    mainLayout->addWidget(boardContainer, 0, 0, 1, 1, Qt::AlignCenter);

    // Set the layout - no additional buttons since they're in the UI file
    setLayout(mainLayout);
}

GameWindow::~GameWindow()
{
    delete ui;
}

void GameWindow::on_quit_button_clicked()
{
    emit return_to_menu();
    this->hide();
}
