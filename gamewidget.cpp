#include "gamewidget.h"
#include "ui_gamewidget.h"
#include "cell.h"

#include <QScreen>
#include <QGridLayout>
#include <QPushButton>
#include <QRandomGenerator>
#include <QTimer>

#include <QDebug>

GameWindow::GameWindow(int playerCount, QWidget *parent) : QDialog(parent), ui(new Ui::Game)
{
    this->playerCount = playerCount;
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
    cells =
    {
        // Corner cells (indexed differently for clarity)
        { "GO", 0, "../../pagani_zonda.jpg", Corner },      // Start (index 40)
        { "AKADEM", 0, "../../pagani_zonda.jpg", Corner },    // Jail (index 41)
        { "FREE PARKING", 0, "../../pagani_zonda.jpg", Corner }, // Parking (index 42)
        { "GO TO AKADEM", 0, "../../pagani_zonda.jpg", Corner },   // Go to Jail (index 43)

        // Regular cells (0-39)
        { "Казна", 0, "../../resources/treasury.png", Treasury },
        { "Кюрдямир", 60, "", Property },
        { "Шамахы", 80, "", Property },
        { "Шанс", 0, "../../resources/chance.png", Chance },
        { "Вокзал", 200, "../../resources/train.png", Railroad },
        { "Казна", 0, "../../resources/treasury.png", Treasury },
        { "Кяльбаджар", 100, "", Property },
        { "Лачин", 100, "", Property },
        { "Шуша", 120, "", Property },
        { "Загатала", 140, "", Property },
        { "Габала", 160, "", Property },
        { "Шеки", 180, "", Property },
        { "Казна", 0, "../../resources/treasury.png", Treasury },
        { "Вокзал", 200, "../../resources/train.png", Railroad },
        { "Шанс", 0, "../../resources/chance.png", Chance },
        { "Астара", 200, "", Property },
        { "Лерик", 200, "", Property },
        { "Ленкорань", 220, "", Property },
        { "Джульфа", 220, "", Property },
        { "Ордубад", 220, "", Property },
        { "Нахычеван", 240, "", Property },
        { "Шанс", 0, "../../resources/chance.png", Chance },
        { "Вокзал", 200, "../../resources/train.png", Railroad },
        { "Казна", 0, "../../resources/treasury.png", Treasury },
        { "Гусар", 280, "", Property },
        { "Хачмаз", 280, "", Property },
        { "Губа", 300, "", Property },
        { "Мингячевир", 300, "", Property },
        { "Гёй-Гёль", 300, "", Property },
        { "Гянджа", 320, "", Property },
        { "Казна", 0, "../../resources/treasury.png", Treasury },
        { "Вокзал", 200, "../../resources/train.png", Railroad },
        { "Шанс", 0, "../../resources/chance.png", Chance },
        { "Сумгайыт", 360, "", Property },
        { "Баку", 400, "", Property },
        { "Сверхналог", 100, "../../resources/dollar.png", Tax }
    };

    // Create main layout - stretched to fill the entire screen
    mainLayout = new QGridLayout(this);
    mainLayout->setSpacing(0);
    mainLayout->setContentsMargins(0, 0, 0, 0); // Remove all margins to maximize space

    // Create game board grid
    boardLayout = new QGridLayout();
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

    // Add label to show which player's turn it is
    playerTurnLabel = new QLabel("Ход игрока 1");
    playerTurnLabel->setAlignment(Qt::AlignCenter);
    playerTurnLabel->setStyleSheet("font-size: 18pt; font-weight: bold; color: #990000;");
    centralLayout->addWidget(playerTurnLabel);

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

    // Создаем маршрут движения
    for(int i = 0; i <= 10; i++)
    {
        path.append({10, 10 - i});
    }

    for(int i = 1; i <= 9; i++)
    {
        path.append({10 - i, 0});
    }

    path.append({0, 0});

    for(int i = 1; i <= 10; i++)
    {
        path.append({0, i});
    }

    for(int i = 1; i <= 9; i++)
    {
        path.append({i, 10});
    }

    treasury_dialog = new TreasuryDialog(this);

    // Создаем игроков в зависимости от выбранного количества
    Player::PlayerColor colors[] = {
        Player::Red,
        Player::Blue,
        Player::Green,
        Player::Yellow
    };

    // Инициализируем позиции всех игроков и создаем фишки
    for (int i = 0; i < playerCount; i++) {
        players.append(new Player(colors[i], boardContainer));
        playerPositions.append(0); // Все начинают с позиции 0

        // Устанавливаем начальную позицию
        auto [row, col] = path[0];
        QLayoutItem *item = boardLayout->itemAtPosition(row, col);

        if (item && item->widget()) {
            QWidget *cell = item->widget();
            players[i]->setParent(cell);
            // Размещаем фишки так, чтобы они не перекрывались
            players[i]->move(5 + i*5, 5 + i*5);
            players[i]->show();
        }
    }

    // Настраиваем интерфейс и события
    connect(ui->go_button, &QPushButton::clicked, this, [=]() {move_player(1);});
    connect(ui->cube_roll, &QPushButton::clicked, this, &GameWindow::start_cubes_roll);

    cube_label_1 = new QLabel(this);
    cube_label_2 = new QLabel(this);

    cube_label_1->setScaledContents(true);
    cube_label_2->setScaledContents(true);

    cube_label_1->setVisible(false);
    cube_label_2->setVisible(false);

    // Обновляем текст, чтобы показать, чей сейчас ход
    updatePlayerTurnLabel();
}

// Добавляем новую функцию для обновления текста о текущем игроке
void GameWindow::updatePlayerTurnLabel() {
    // Определяем цвет текущего игрока
    QString colorName;
    switch (players[currentPlayerIndex]->getColor()) {
    case Player::Red:
        colorName = "красного";
        break;
    case Player::Blue:
        colorName = "синего";
        break;
    case Player::Green:
        colorName = "зеленого";
        break;
    case Player::Yellow:
        colorName = "желтого";
        break;
    }

    playerTurnLabel->setText(QString("Ход %1 игрока").arg(colorName));
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

void GameWindow::move_player(int steps)
{
    // Перемещаем текущего игрока
    playerPositions[currentPlayerIndex] = (playerPositions[currentPlayerIndex] + steps) % path.size();
    auto [row, col] = path[playerPositions[currentPlayerIndex]];
    players[currentPlayerIndex] -> moveTo(boardLayout, row, col);

    // Переходим к следующему игроку
    check_cell_type();
}

void GameWindow::next_player()
{
    // Переключаемся на следующего игрока
    currentPlayerIndex = (currentPlayerIndex + 1) % playerCount;

    // Обновляем текст, показывающий чей сейчас ход
    updatePlayerTurnLabel();
}

QStringList cube_images =
{
        "../../resources/cube_1.png",
        "../../resources/cube_2.png",
        "../../resources/cube_3.png",
        "../../resources/cube_4.png",
        "../../resources/cube_5.png",
        "../../resources/cube_6.png"
};


void GameWindow::start_cubes_roll()
{
    QSize cube_size(100, 100);
    int spacing = 20;

    int totalWidth = cube_size.width() * 2 + spacing;
    int x = (width() - totalWidth) / 2;
    int y = (height() - cube_size.height()) / 2;

    cube_label_1->setGeometry(x, y, cube_size.width(), cube_size.height());
    cube_label_2->setGeometry(x + cube_size.width() + spacing, y, cube_size.width(), cube_size.height());

    cube_label_1->setVisible(true);
    cube_label_2->setVisible(true);
    cube_label_1->raise();
    cube_label_2->raise();

    int roll_animation_step = 0;
    int max_roll_steps = 10;
    QTimer *cube_roll_timer = new QTimer(this);

    connect(cube_roll_timer, &QTimer::timeout, this, [=]() mutable
            {
                if (roll_animation_step < max_roll_steps)
                {
                    int random1 = QRandomGenerator::global() -> bounded(1, 7);
                    int random2 = QRandomGenerator::global() -> bounded(1, 7);

                    cube_label_1->setPixmap(QPixmap(cube_images[random1 - 1]).scaled(cube_size, Qt::KeepAspectRatio, Qt::SmoothTransformation));
                    cube_label_2->setPixmap(QPixmap(cube_images[random2 - 1]).scaled(cube_size, Qt::KeepAspectRatio, Qt::SmoothTransformation));

                    roll_animation_step++;
                }
                else
                {
                    cube_roll_timer->stop();
                    cube_roll_timer->deleteLater();

                    int final1 = QRandomGenerator::global() -> bounded(1, 7);
                    int final2 = QRandomGenerator::global() -> bounded(1, 7);

                    cube_label_1->setPixmap(QPixmap(cube_images[final1 - 1]).scaled(cube_size, Qt::KeepAspectRatio, Qt::SmoothTransformation));
                    cube_label_2->setPixmap(QPixmap(cube_images[final2 - 1]).scaled(cube_size, Qt::KeepAspectRatio, Qt::SmoothTransformation));

                    QTimer::singleShot(2200, this, [=]()
                           {
                               cube_label_1 ->setVisible(false);
                               cube_label_2->setVisible(false);

                               move_player(final1 + final2);
                            });
                }
            });

    cube_roll_timer->start(100);
}



TreasuryDialog::TreasuryDialog(QWidget *parent) : QDialog(parent)
{
    setWindowTitle("Казна");
    setModal(true);
    setMinimumSize(400, 500);

    QVBoxLayout *layout = new QVBoxLayout(this);

    cardTextLabel = new QLabel(this);
    cardTextLabel->setAlignment(Qt::AlignCenter);
    cardTextLabel->setWordWrap(true);
    cardTextLabel->setStyleSheet("font-size: 14pt;");

    okButton = new QPushButton("ОК", this);
    connect(okButton, &QPushButton::clicked, this, &QDialog::accept);

    layout->addWidget(cardTextLabel);
    layout->addWidget(okButton, 0, Qt::AlignCenter);

    setLayout(layout);

    treasury_cards =
    {
        {"Банковская ошибка в вашу пользу. Получите 200$"},
        {"Оплатите счет за лечение - 50$"},
        {"Вы выиграли в лотерею. Получите 100$"},
        {"С днем рождения! Получите 50$ от каждого игрока"},
        {"Налог на имущество. Заплатите 25$ за каждый дом"}
    };
}


void TreasuryDialog::show_random_card()
{
    int cardIndex = QRandomGenerator::global()->bounded(treasury_cards.size());
    auto text = treasury_cards[cardIndex];

    cardTextLabel->setText(text);
}

void GameWindow::check_cell_type()
{
    int currentPosition = playerPositions[currentPlayerIndex];

    CellInfo info = cells[currentPosition];
    CellTypeO cellType = info.type;

    qDebug() << "type:" << static_cast<int>(cellType);
    qDebug() << "cell name:" << info.name;


    switch (cellType)
    {
        case Treasury:
            qDebug() << "cell_treasure";
            show_treasure_card_in_tab();
            return;
        case Chance:
            // showChanceWindow(); // Будущая функциональность
            QTimer::singleShot(1000, this, &GameWindow::next_player);
            return;
        case Property:
            // Будущая функциональность для покупки собственности
            QTimer::singleShot(1000, this, &GameWindow::next_player);
            return;
        case Railroad:
            // Обработка вокзалов
            QTimer::singleShot(1000, this, &GameWindow::next_player);
            return;
        case Tax:
            // Обработка налогов
            QTimer::singleShot(1000, this, &GameWindow::next_player);
            return;
        case Corner:
            // Обработка угловых ячеек
            QTimer::singleShot(1000, this, &GameWindow::next_player);
            return;
    }

    qDebug() << "cell";

    QTimer::singleShot(1000, this, &GameWindow::next_player);
}


void GameWindow::show_treasure_dialog()
{
    treasury_dialog->show_random_card();

    qDebug() << "treasure";

    QTimer::singleShot(2000, this, [this]()
    {
        treasury_dialog->show();
        next_player();
    });
}

void GameWindow::show_treasure_card_in_tab()
{
    if (!kaznaWidget) {
        kaznaWidget = new QWidget(this);
        kaznaWidget->setStyleSheet("background-color: #fff0b3; border: 2px solid #aa8800;");
        kaznaWidget->setFixedSize(400, 150);

        QVBoxLayout *kaznaLayout = new QVBoxLayout(kaznaWidget);
        kaznaLabel = new QLabel("Карта казны появится здесь");
        kaznaLayout->addWidget(kaznaLabel);

        QPushButton *closeBtn = new QPushButton("Закрыть");
        kaznaLayout->addWidget(closeBtn);
        connect(closeBtn, &QPushButton::clicked, kaznaWidget, &QWidget::hide);

        kaznaLayout->setContentsMargins(10, 10, 10, 10);

        mainLayout->addWidget(kaznaWidget, 1, 0, 1, 1, Qt::AlignCenter);

    }

    // Случайная карта
    QStringList treasury_cards = {
        "Получите 200 монет!",
        "Заплатите 100 монет налогов.",
        "Пропустите ход.",
        "Карта 'Выход из тюрьмы'",
        "Отдайте 50 монет каждому игроку."
    };

    int index = QRandomGenerator::global()->bounded(treasury_cards.size());
    kaznaLabel->setText(treasury_cards[index]);
    kaznaWidget->show();
}

