#include "gamewidget.h"
#include "ui_gamewidget.h"
#include "cell.h"

#include <QScreen>
#include <QGridLayout>
#include <QPushButton>
#include <QRandomGenerator>
#include <QTimer>
#include <QMessageBox>

#include <QDebug>

#include <QPropertyAnimation>

GameWindow::GameWindow(int playerCount, QWidget *parent) : QDialog(parent), ui(new Ui::Game)
{
    this->playerCount = playerCount;
    ui->setupUi(this);

    // Инициализация карт и ячеек
    initializeCells();
    initializeCards();

    // Инициализируем состояния игроков
    playerStates.clear();
    for (int i = 0; i < playerCount; i++) {
        playerStates.append(PlayerState());
    }

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


    ui -> go_button -> setParent(this);
    ui -> cube_roll -> setParent(this);
    ui -> quit_button -> setParent(this);
    ui -> build_houses -> setParent(this);

    int rightMargin = 20;
    int bottomMargin = 20;
    int buttonSpacing = 10;
    int buttonWidth = 100;
    int buttonHeight = 40;

    ui -> build_houses -> setGeometry
        (
            screenGeometry.width() - rightMargin - buttonWidth,
            screenGeometry.height() - bottomMargin - buttonHeight * 4 - buttonSpacing * 3,
            buttonWidth, buttonHeight
            );


    ui->go_button->setGeometry
        (
        screenGeometry.width() - rightMargin - buttonWidth,
        screenGeometry.height() - bottomMargin - buttonHeight * 3 - buttonSpacing * 2,
        buttonWidth, buttonHeight
        );


    ui->cube_roll->setGeometry
        (
        screenGeometry.width() - rightMargin - buttonWidth,
        screenGeometry.height() - bottomMargin - buttonHeight * 2 - buttonSpacing,
        buttonWidth, buttonHeight
        );

    ui->quit_button->setGeometry(
        screenGeometry.width() - rightMargin - buttonWidth,
        screenGeometry.height() - bottomMargin - buttonHeight,
        buttonWidth, buttonHeight
        );


    ui -> go_button -> raise();
    ui -> cube_roll -> raise();
    ui -> quit_button ->raise();
    ui -> build_houses -> raise();

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

    for(int i = 0; i < cells.size(); i++)
    {
        CellWidget *widget = new CellWidget(cells[i]);
        cell_vec.append(widget);
        qDebug() << "cell_vec = " << i << "\n";
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
    connect(ui -> go_button, &QPushButton::clicked, this, [=]() {move_player(1);});
    connect(ui -> cube_roll, &QPushButton::clicked, this, &GameWindow::start_cubes_roll);
    connect(ui -> build_houses, &QPushButton::clicked, this,[=]()
        {
            if(cell_vec[16])
            {
                cell_vec[16] -> build_house();
                qDebug() << "ok";
            }
            else
            {
                qDebug() << "nope";
            }
        });

    cube_label_1 = new QLabel(this);
    cube_label_2 = new QLabel(this);

    cube_label_1->setScaledContents(true);
    cube_label_2->setScaledContents(true);

    cube_label_1->setVisible(false);
    cube_label_2->setVisible(false);

    // Создаем и инициализируем виджет для отображения информации об игроках
    createPlayerInfoWidget();
    updatePlayerInfoDisplay();

    // Обновляем текст, чтобы показать, чей сейчас ход
    updatePlayerTurnLabel();
}

void GameWindow::initializeCells() {
    // Инициализация угловых ячеек
    cells.clear();

    // Угловые ячейки
    cells.append({"GO", 0, "../../pagani_zonda.jpg", Corner, -1, false, -1, "", 0, 0, 0});  // Start
    cells.append({"AKADEM", 0, "../../resources/academ.png", Corner, -1, false, -1, "", 0, 0, 0});  // Jail
    cells.append({"FREE PARKING", 0, "../../resources/parking.png", Corner, -1, false, -1, "", 0, 0, 0});  // Free Parking
    cells.append({"GO TO AKADEM", 0, "../../resources/go_to_academ.png", Corner, -1, false, -1, "", 0, 0, 0});  // Go to Jail

    // Обычные ячейки
    cells.append({"Казна", 0, "../../resources/treasury.png", Treasury, -1, false, -1, "", 0, 0, 0});
    cells.append({"Кюрдямир", 60, "../../resources/colour1.png", Property, -1, false, -1, "brown", 2, 50, 0});
    cells.append({"Шамахы", 80, "../../resources/colour1.png", Property, -1, false, -1, "brown", 4, 50, 0});
    cells.append({"Шанс", 0, "../../resources/chance.png", Chance, -1, false, -1, "", 0, 0, 0});
    cells.append({"Вокзал", 200, "../../resources/train.png", Railroad, -1, false, -1, "railroad", 25, 0, 0});
    cells.append({"Казна", 0, "../../resources/treasury.png", Treasury, -1, false, -1, "", 0, 0, 0});
    cells.append({"Кяльбаджар", 100, "../../resources/colour2.png", Property, -1, false, -1, "lightblue", 6, 50, 0});
    cells.append({"Лачин", 100, "../../resources/colour2.png", Property, -1, false, -1, "lightblue", 6, 50, 0});
    cells.append({"Шуша", 120, "../../resources/colour2.png", Property, -1, false, -1, "lightblue", 8, 50, 0});
    cells.append({"Загатала", 140, "../../resources/colour3.png", Property, -1, false, -1, "pink", 10, 100, 0});
    cells.append({"Габала", 160, "../../resources/colour3.png", Property, -1, false, -1, "pink", 12, 100, 0});
    cells.append({"Шеки", 180, "../../resources/colour3.png", Property, -1, false, -1, "pink", 14, 100, 0});
    cells.append({"Казна", 0, "../../resources/treasury.png", Treasury, -1, false, -1, "", 0, 0, 0});
    cells.append({"Вокзал", 200, "../../resources/train.png", Railroad, -1, false, -1, "railroad", 25, 0, 0});
    cells.append({"Шанс", 0, "../../resources/chance.png", Chance, -1, false, -1, "", 0, 0, 0});
    cells.append({"Астара", 200, "../../resources/colour4.png", Property, -1, false, -1, "orange", 16, 100, 0});
    cells.append({"Лерик", 200, "../../resources/colour4.png", Property, -1, false, -1, "orange", 16, 100, 0});
    cells.append({"Ленкорань", 220, "../../resources/colour4.png", Property, -1, false, -1, "orange", 18, 100, 0});
    cells.append({"Джульфа", 220, "../../resources/colour5.png", Property, -1, false, -1, "red", 18, 150, 0});
    cells.append({"Ордубад", 220, "../../resources/colour5.png", Property, -1, false, -1, "red", 20, 150, 0});
    cells.append({"Нахычеван", 240, "../../resources/colour5.png", Property, -1, false, -1, "red", 22, 150, 0});
    cells.append({"Шанс", 0, "../../resources/chance.png", Chance, -1, false, -1, "", 0, 0, 0});
    cells.append({"Вокзал", 200, "../../resources/train.png", Railroad, -1, false, -1, "railroad", 25, 0, 0});
    cells.append({"Казна", 0, "../../resources/treasury.png", Treasury, -1, false, -1, "", 0, 0, 0});
    cells.append({"Гусар", 280, "../../resources/colour6.png", Property, -1, false, -1, "yellow", 24, 150, 0});
    cells.append({"Хачмаз", 280, "../../resources/colour6.png", Property, -1, false, -1, "yellow", 24, 150, 0});
    cells.append({"Губа", 300, "../../resources/colour6.png", Property, -1, false, -1, "yellow", 26, 150, 0});
    cells.append({"Мингячевир", 300, "../../resources/colour7.png", Property, -1, false, -1, "green", 26, 200, 0});
    cells.append({"Гёй-Гёль", 300, "../../resources/colour7.png", Property, -1, false, -1, "green", 28, 200, 0});
    cells.append({"Гянджа", 320, "../../resources/colour7.png", Property, -1, false, -1, "green", 30, 200, 0});
    cells.append({"Казна", 0, "../../resources/treasury.png", Treasury, -1, false, -1, "", 0, 0, 0});
    cells.append({"Вокзал", 200, "../../resources/train.png", Railroad, -1, false, -1, "railroad", 25, 0, 0});
    cells.append({"Шанс", 0, "../../resources/chance.png", Chance, -1, false, -1, "", 0, 0, 0});
    cells.append({"Сумгайыт", 360, "../../resources/colour8.png", Property, -1, false, -1, "blue", 35, 200, 0});
    cells.append({"Баку", 400, "../../resources/colour8.png", Property, -1, false, -1, "blue", 50, 200, 0});
    cells.append({"Сверхналог", 100, "../../resources/dollar.png", Tax, -1, false, -1, "", 0, 0, 0});
}

void GameWindow::initializeCards() {
    // Инициализация карт казны
    treasuryCards = {
        Card("Банковская ошибка в вашу пользу. Получите 200$", Money, 200),
        Card("Оплатите счет за лечение - 50$", Money, -50),
        Card("Вы выиграли в лотерею. Получите 100$", Money, 100),
        Card("С днем рождения! Получите 50$ от каждого игрока", Money, 50),
        Card("Налог на имущество. Заплатите 25$ за каждый дом", PropertyRelated, -25),
        Card("Выход из тюрьмы. Сохраните эту карту", JailRelated, 0, -1, true),
        Card("Банк выплачивает вам дивиденды в размере 50$", Money, 50),
        Card("Вы заняли первое место в конкурсе красоты. Получите 100$", Money, 100),
        Card("Возвращайтесь на Старт", Movement, 0, 0),
        Card("Оплатите страховку здоровья - 50$", Money, -50),
        Card("Оплатите услуги доктора - 100$", Money, -100),
        Card("Получите 25$ за консультационные услуги", Money, 25),
        Card("Вы наследуете 100$", Money, 100),
        Card("Школьные сборы. Заплатите 150$", Money, -150),
        Card("Возврат налога. Получите 20$", Money, 20),
        Card("Ваш день рождения. Получите 10$ от каждого игрока", Money, 10)
    };

    // Инициализация карт шанса
    chanceCards = {
        Card("Отправляйтесь на Старт", Movement, 0, 0),
        Card("Отправляйтесь в тюрьму. Идите прямо в тюрьму. Не проходите Старт.", JailRelated, 0, 10),
        Card("Отправляйтесь на Шушу", Movement, 0, 12),
        Card("Отправляйтесь на ближайший вокзал", Movement, 0, -2), // Специальный код для вокзала
        Card("Банк выплачивает вам дивиденды в размере 50$", Money, 50),
        Card("Выход из тюрьмы. Сохраните эту карту", JailRelated, 0, -1, true),
        Card("Вернитесь на 3 поля назад", Movement, -3),
        Card("Отправляйтесь на Баку", Movement, 0, 39),
        Card("Общий ремонт собственности: заплатите 25$ за каждый дом и 100$ за отель", PropertyRelated, -1),
        Card("Штраф за превышение скорости - 15$", Money, -15),
        Card("Штраф за пьянство - 20$", Money, -20),
        Card("Продвинитесь на следующую ячейку Казны", Movement, 0, -3), // Специальный код для Казны
        Card("Вы избраны председателем правления. Выплатите каждому игроку 50$", Money, -50),
        Card("Ваша ссуда на строительство выплачена. Получите 150$", Money, 150),
        Card("Переместитесь на 5 полей вперед", Movement, 5),
        Card("Продвинитесь до следующей ячейки собственности. Если она свободна, вы можете её купить", Movement, 0, -4) // Специальный код для следующей собственности
    };
}

void GameWindow::createPlayerInfoWidget() {
    playerInfoWidget = new QWidget(this);
    playerInfoWidget->setStyleSheet("background-color: white; border: 1px solid black;");
    playerInfoWidget->setFixedWidth(250);

    QVBoxLayout* infoLayout = new QVBoxLayout(playerInfoWidget);

    // Заголовок панели
    QLabel* headerLabel = new QLabel("Информация об игроках");
    headerLabel->setAlignment(Qt::AlignCenter);
    headerLabel->setStyleSheet("font-weight: bold; font-size: 14px;");
    infoLayout->addWidget(headerLabel);

    // Создаем метки для каждого игрока
    playerLabels.clear();
    for (int i = 0; i < playerCount; i++) {
        QLabel* playerLabel = new QLabel();
        playerLabel->setTextFormat(Qt::RichText);
        playerLabel->setWordWrap(true);
        playerLabels.append(playerLabel);
        infoLayout->addWidget(playerLabel);

        // Добавляем разделитель между игроками, кроме последнего
        if (i < playerCount - 1) {
            QFrame* line = new QFrame();
            line->setFrameShape(QFrame::HLine);
            line->setFrameShadow(QFrame::Sunken);
            infoLayout->addWidget(line);
        }
    }

    // Добавляем панель в основной макет
    mainLayout->addWidget(playerInfoWidget, 0, 1, 1, 1, Qt::AlignRight | Qt::AlignTop);
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
    // Проверяем, находится ли игрок в тюрьме
    if (playerStates[currentPlayerIndex].inJail) {
        QMessageBox::information(this, "Тюрьма", "Вы в тюрьме! Чтобы выйти, бросьте дубль, используйте карту выхода или заплатите 50₼");

        // Уменьшаем количество ходов в тюрьме
        playerStates[currentPlayerIndex].jailTurns--;

        // Если время в тюрьме истекло, игрок выходит автоматически
        if (playerStates[currentPlayerIndex].jailTurns <= 0) {
            playerStates[currentPlayerIndex].inJail = false;
            QMessageBox::information(this, "Тюрьма", "Ваше время в тюрьме истекло, вы свободны!");
        }

        // Переходим к следующему игроку
        next_player();
        return;
    }

    // Перемещаем текущего игрока
    int oldPosition = playerPositions[currentPlayerIndex];
    playerPositions[currentPlayerIndex] = (playerPositions[currentPlayerIndex] + steps) % path.size();
    auto [row, col] = path[playerPositions[currentPlayerIndex]];
    players[currentPlayerIndex] -> moveTo(boardLayout, row, col);

    // Проверяем, пересек ли игрок клетку "Старт"
    if (playerPositions[currentPlayerIndex] < oldPosition) {
        // Игрок сделал полный круг, начисляем 200₼
        playerStates[currentPlayerIndex].money += 200;
        QMessageBox::information(this, "Пересечение Старта", "Вы прошли через Старт и получили 200₼!");
        updatePlayerInfoDisplay();
    }

    // Проверяем типа ячейки и выполняем соответствующие действия
    check_cell_type();
}

void GameWindow::next_player()
{
    // Проверка на банкротство игрока
    if (playerStates[currentPlayerIndex].money < 0) {
        if (!handleBankruptcy()) {
            // Если игрок не смог разрешить ситуацию, он банкрот
            playerStates[currentPlayerIndex].bankrupt = true;
            QMessageBox::warning(this, "Банкротство",
                                 "Вы обанкротились! Ваша собственность возвращается банку.");

            // Освобождаем собственность игрока
            for (int propIndex : playerStates[currentPlayerIndex].properties) {
                cells[propIndex].owner = -1;
                cells[propIndex].isMortgaged = false;
                cells[propIndex].houseCount = 0;
            }

            playerStates[currentPlayerIndex].properties.clear();
            playerStates[currentPlayerIndex].mortgagedProperties.clear();

            // Проверяем, остались ли игроки в игре
            int remainingPlayers = 0;
            int lastPlayerIndex = -1;

            for (int i = 0; i < playerCount; i++) {
                if (!playerStates[i].bankrupt) {
                    remainingPlayers++;
                    lastPlayerIndex = i;
                }
            }

            if (remainingPlayers <= 1) {
                // Игра окончена, объявляем победителя
                QString winnerColor;
                switch (players[lastPlayerIndex]->getColor()) {
                case Player::Red: winnerColor = "Красный"; break;
                case Player::Blue: winnerColor = "Синий"; break;
                case Player::Green: winnerColor = "Зеленый"; break;
                case Player::Yellow: winnerColor = "Желтый"; break;
                }

                QMessageBox::information(this, "Игра окончена",
                                         QString("%1 игрок победил в игре!").arg(winnerColor));

                // Возвращаемся в главное меню
                emit return_to_menu();
                this->hide();
                return;
            }
        }
    }

    // Переключаемся на следующего игрока
    do {
        currentPlayerIndex = (currentPlayerIndex + 1) % playerCount;
    } while (playerStates[currentPlayerIndex].bankrupt); // Пропускаем обанкротившихся игроков

    // Обновляем текст, показывающий чей сейчас ход
    updatePlayerTurnLabel();
}

bool GameWindow::handleBankruptcy() {
    // Проверяем, может ли игрок продать дома или заложить собственность, чтобы избежать банкротства
    int deficit = -playerStates[currentPlayerIndex].money;
    int availableFunds = 0;

    // Подсчитываем стоимость домов (при продаже стоимость вдвое меньше)
    for (int propIndex : playerStates[currentPlayerIndex].properties) {
        if (cells[propIndex].houseCount > 0) {
            availableFunds += (cells[propIndex].housePrice * cells[propIndex].houseCount) / 2;
        }
    }

    // Подсчитываем стоимость незаложенной собственности (при закладывании получаем половину цены)
    for (int propIndex : playerStates[currentPlayerIndex].properties) {
        if (!cells[propIndex].isMortgaged) {
            availableFunds += cells[propIndex].price / 2;
        }
    }

    if (availableFunds >= deficit) {
        // Игрок может избежать банкротства, предлагаем варианты
        QMessageBox::warning(this, "Опасность банкротства",
                             QString("У вас отрицательный баланс: %1₼. Вам нужно продать дома или заложить собственность.").arg(playerStates[currentPlayerIndex].money));

        // Здесь можно реализовать диалог с опциями для продажи/закладывания
        // Для простоты сейчас просто автоматически продаем сначала дома, потом закладываем собственность

        int neededMoney = deficit;

        // Сначала продаем дома
        for (int propIndex : playerStates[currentPlayerIndex].properties) {
            if (neededMoney <= 0) break;

            if (cells[propIndex].houseCount > 0) {
                int houseSaleValue = (cells[propIndex].housePrice * cells[propIndex].houseCount) / 2;
                playerStates[currentPlayerIndex].money += houseSaleValue;
                QMessageBox::information(this, "Продажа домов",
                                         QString("Вы продали %1 дома на %2 за %3₼")
                                             .arg(cells[propIndex].houseCount)
                                             .arg(cells[propIndex].name)
                                             .arg(houseSaleValue));

                cells[propIndex].houseCount = 0;
                neededMoney -= houseSaleValue;
            }
        }

        // Если денег все еще не хватает, закладываем собственность
        for (int propIndex : playerStates[currentPlayerIndex].properties) {
            if (neededMoney <= 0) break;

            if (!cells[propIndex].isMortgaged) {
                int mortgageValue = cells[propIndex].price / 2;
                playerStates[currentPlayerIndex].money += mortgageValue;
                cells[propIndex].isMortgaged = true;
                playerStates[currentPlayerIndex].mortgagedProperties.append(propIndex);

                QMessageBox::information(this, "Залог собственности",
                                         QString("Вы заложили %1 за %2₼")
                                             .arg(cells[propIndex].name)
                                             .arg(mortgageValue));

                neededMoney -= mortgageValue;
            }
        }

        updatePlayerInfoDisplay();
        return true; // Игрок избежал банкротства
    }

    return false; // Игрок не смог избежать банкротства
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

    QWidget *boardWidget = boardLayout->parentWidget();
    int boardWidth = boardWidget->width();
    int boardHeight = boardWidget->height();
    QPoint boardTopLeft = boardWidget->mapTo(this, QPoint(0, 0));


    int totalWidth = cube_size.width() * 2 + spacing;
    int x = boardTopLeft.x() + (boardWidth - totalWidth) / 2;
    int y = boardTopLeft.y() + (boardHeight - cube_size.height()) / 2;

    cube_label_1->setParent(this);
    cube_label_2->setParent(this);

    cube_label_1->setGeometry(x, y, cube_size.width(), cube_size.height());
    cube_label_2->setGeometry(x + cube_size.width() + spacing, y, cube_size.width(), cube_size.height());

    cube_label_1->setVisible(true);
    cube_label_2->setVisible(true);
    cube_label_1->raise();
    cube_label_2->raise();

    // Потом запускаем анимацию броска как обычно
    int roll_animation_step = 0;
    int max_roll_steps = 10;
    QTimer *cube_roll_timer = new QTimer(this);

    connect(cube_roll_timer, &QTimer::timeout, this, [=]() mutable
            {
                if (roll_animation_step < max_roll_steps)
                {
                    int random1 = QRandomGenerator::global()->bounded(1, 7);
                    int random2 = QRandomGenerator::global()->bounded(1, 7);

                    cube_label_1->setPixmap(QPixmap(cube_images[random1 - 1]).scaled(cube_size, Qt::KeepAspectRatio, Qt::SmoothTransformation));
                    cube_label_2->setPixmap(QPixmap(cube_images[random2 - 1]).scaled(cube_size, Qt::KeepAspectRatio, Qt::SmoothTransformation));

                    roll_animation_step++;
                }
                else
                {
                    cube_roll_timer->stop();
                    cube_roll_timer->deleteLater();

                    int final1 = QRandomGenerator::global()->bounded(1, 7);
                    int final2 = QRandomGenerator::global()->bounded(1, 7);

                    cube_label_1->setPixmap(QPixmap(cube_images[final1 - 1]).scaled(cube_size, Qt::KeepAspectRatio, Qt::SmoothTransformation));
                    cube_label_2->setPixmap(QPixmap(cube_images[final2 - 1]).scaled(cube_size, Qt::KeepAspectRatio, Qt::SmoothTransformation));

                    if (playerStates[currentPlayerIndex].inJail && final1 == final2) {
                        playerStates[currentPlayerIndex].inJail = false;
                        QMessageBox::information(this, "Выход из тюрьмы", "Вы выбросили дубль и выходите из тюрьмы!");
                    }

                    QTimer::singleShot(2200, this, [=]()
                                       {
                                           cube_label_1->setVisible(false);
                                           cube_label_2->setVisible(false);

                                           move_player(final1 + final2);

                                           if (final1 == final2 && !playerStates[currentPlayerIndex].inJail) {
                                               QMessageBox::information(this, "Дубль!", "Вы выбросили дубль! Ходите еще раз.");
                                               currentPlayerIndex = (currentPlayerIndex - 1 + playerCount) % playerCount;
                                           }
                                       });
                }
            });

    cube_roll_timer->start(100);
}



void GameWindow::check_cell_type()
{
    int currentPosition = playerPositions[currentPlayerIndex];
    CellInfo& info = cells[currentPosition];

    qDebug() << "Игрок на ячейке:" << info.name << "тип:" << static_cast<int>(info.type);

    switch (info.type)
    {
    case Treasury:
        // Выбираем случайную карту из колоды казны
        if (!treasuryCards.isEmpty()) {
            int cardIndex = QRandomGenerator::global()->bounded(treasuryCards.size());
            Card card = treasuryCards[cardIndex];

            // Отображаем карту
            if (kaznaWidget) {
                kaznaLabel->setText(card.text);
                kaznaWidget->show();
            }

            // Обрабатываем действие карты
            processCard(card);

            // Если карта не сохраняется у игрока, возвращаем ее назад в колоду
            if (!card.keepCard) {
                treasuryCards.removeAt(cardIndex);
                treasuryCards.append(card);
            }
        }
        break;

    case Chance:
        // Аналогично для карт шанса
        if (!chanceCards.isEmpty()) {
            int cardIndex = QRandomGenerator::global()->bounded(chanceCards.size());
            Card card = chanceCards[cardIndex];

            // Показываем карту
            QMessageBox::information(this, "Шанс", card.text);

            // Обрабатываем действие карты
            processCard(card);

            // Если карта не сохраняется у игрока, возвращаем ее назад в колоду
            if (!card.keepCard) {
                chanceCards.removeAt(cardIndex);
                chanceCards.append(card);
            }
        }
        break;

    case Property:
        // Проверяем, есть ли у ячейки владелец
        if (info.owner == -1) {
            // Если нет владельца, предлагаем купить собственность
            if (QMessageBox::question(this, "Покупка собственности",
                                      QString("Хотите купить %1 за %2₼?").arg(info.name).arg(info.price),
                                      QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes) {
                buyProperty(currentPosition);
            }
        } else if (info.owner != currentPlayerIndex) {
            // Если есть владелец и это не текущий игрок, платим ренту
            if (!info.isMortgaged) {
                payRent(currentPosition);
            } else {
                QMessageBox::information(this, "Заложенная собственность",
                                         "Эта собственность заложена, рента не платится.");
            }
        }
        break;

    case Railroad:
        // Аналогично собственности, но с особыми правилами для вокзалов
        if (info.owner == -1) {
            if (QMessageBox::question(this, "Покупка вокзала",
                                      QString("Хотите купить %1 за %2₼?").arg(info.name).arg(info.price),
                                      QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes) {
                buyProperty(currentPosition);
            }
        } else if (info.owner != currentPlayerIndex) {
            if (!info.isMortgaged) {
                payRent(currentPosition);
            } else {
                QMessageBox::information(this, "Заложенная собственность",
                                         "Этот вокзал заложен, рента не платится.");
            }
        }
        break;

    case Tax:
        // Налог - просто вычитаем деньги
        {
            int taxAmount = info.price;
            playerStates[currentPlayerIndex].money -= taxAmount;
            QMessageBox::information(this, "Налог",
                                     QString("Вы заплатили %1₼ налога").arg(taxAmount));
            updatePlayerInfoDisplay();
        }
        break;

    case Corner:
        // Обработка угловых ячеек
        if (currentPosition == 3) { // ID ячейки "GO TO AKADEM"
            QMessageBox::information(this, "В тюрьму", "Вы отправляетесь в тюрьму!");

            // Перемещаем игрока в тюрьму
            playerStates[currentPlayerIndex].inJail = true;
            playerStates[currentPlayerIndex].jailTurns = 3;

            // Перемещаем фишку на ячейку тюрьмы
            playerPositions[currentPlayerIndex] = 1; // Позиция тюрьмы
            auto [row, col] = path[1];
            players[currentPlayerIndex]->moveTo(boardLayout, row, col);
        }
        else if (currentPosition == 1) { // ID ячейки "AKADEM" (тюрьма)
            QMessageBox::information(this, "Посещение тюрьмы", "Вы просто посещаете тюрьму.");
        }
        else if (currentPosition == 2) { // ID ячейки "FREE PARKING"
            QMessageBox::information(this, "Бесплатная парковка", "Отдохните на бесплатной парковке.");
        }
        break;
    }

    // Через некоторое время переходим к следующему игроку
    QTimer::singleShot(1000, this, &GameWindow::next_player);
}

void GameWindow::processCard(const Card& card)
{
    PlayerState& currentPlayer = playerStates[currentPlayerIndex];

    switch (card.type) {
    case Money:
        if (card.value > 0) {
            // Получение денег
            currentPlayer.money += card.value;
            QMessageBox::information(this, "Деньги", QString("Вы получили %1₼").arg(card.value));
        } else if (card.value < 0) {
            // Оплата денег
            currentPlayer.money += card.value; // value уже отрицательный
            QMessageBox::information(this, "Деньги", QString("Вы заплатили %1₼").arg(-card.value));
        }
        break;

    case Movement:
        if (card.targetPosition >= 0) {
            // Перемещение на конкретную ячейку
            int oldPosition = playerPositions[currentPlayerIndex];
            playerPositions[currentPlayerIndex] = card.targetPosition;
            auto [row, col] = path[card.targetPosition];
            players[currentPlayerIndex]->moveTo(boardLayout, row, col);

            // Проверяем, пересек ли игрок "Старт"
            if (card.targetPosition < oldPosition && card.targetPosition != 1) { // Не считаем перемещение в тюрьму как пересечение старта
                currentPlayer.money += 200;
                QMessageBox::information(this, "Пересечение Старта", "Вы прошли через Старт и получили 200₼!");
            }

            // После перемещения нужно также обработать новую ячейку
            check_cell_type();
        } else if (card.value != 0) {
            // Перемещение на несколько ячеек вперед/назад
            int newPosition = (playerPositions[currentPlayerIndex] + card.value) % path.size();
            if (newPosition < 0) newPosition += path.size();

            playerPositions[currentPlayerIndex] = newPosition;
            auto [row, col] = path[newPosition];
            players[currentPlayerIndex]->moveTo(boardLayout, row, col);

            // После перемещения обрабатываем новую ячейку
            check_cell_type();
        }
        // Специальные коды для особых движений
        else if (card.targetPosition == -2) {
            // Перемещение на ближайший вокзал
            int currentPos = playerPositions[currentPlayerIndex];
            // Находим ближайший вокзал
            int closestRailroad = -1;
            int minDistance = path.size();

            for (int i = 0; i < cells.size(); i++) {
                if (cells[i].type == Railroad) {
                    int distance = (i - currentPos) % path.size();
                    if (distance < 0) distance += path.size();
                    if (distance < minDistance && distance > 0) {
                        minDistance = distance;
                        closestRailroad = i;
                    }
                }
            }

            if (closestRailroad != -1) {
                playerPositions[currentPlayerIndex] = closestRailroad;
                auto [row, col] = path[closestRailroad];
                players[currentPlayerIndex]->moveTo(boardLayout, row, col);

                QMessageBox::information(this, "Перемещение",
                                         QString("Вы перемещаетесь на ближайший вокзал: %1").arg(cells[closestRailroad].name));

                // Обрабатываем новую ячейку
                check_cell_type();
            }
        }
        else if (card.targetPosition == -3) {
            // Перемещение на следующую ячейку Казны
            int currentPos = playerPositions[currentPlayerIndex];
            int nextTreasury = -1;

            for (int i = 1; i <= path.size(); i++) {
                int checkPos = (currentPos + i) % path.size();
                if (cells[checkPos].type == Treasury) {
                    nextTreasury = checkPos;
                    break;
                }
            }

            if (nextTreasury != -1) {
                playerPositions[currentPlayerIndex] = nextTreasury;
                auto [row, col] = path[nextTreasury];
                players[currentPlayerIndex]->moveTo(boardLayout, row, col);

                QMessageBox::information(this, "Перемещение",
                                         "Вы перемещаетесь на следующую ячейку Казны");

                // Обрабатываем новую ячейку
                check_cell_type();
            }
        }
        else if (card.targetPosition == -4) {
            // Перемещение на следующую свободную собственность
            int currentPos = playerPositions[currentPlayerIndex];
            int nextProperty = -1;

            for (int i = 1; i <= path.size(); i++) {
                int checkPos = (currentPos + i) % path.size();
                if ((cells[checkPos].type == Property || cells[checkPos].type == Railroad) && cells[checkPos].owner == -1) {
                    nextProperty = checkPos;
                    break;
                }
            }

            if (nextProperty != -1) {
                playerPositions[currentPlayerIndex] = nextProperty;
                auto [row, col] = path[nextProperty];
                players[currentPlayerIndex]->moveTo(boardLayout, row, col);

                QMessageBox::information(this, "Перемещение",
                                         QString("Вы перемещаетесь на следующую свободную собственность: %1").arg(cells[nextProperty].name));

                // Предлагаем купить эту собственность
                if (QMessageBox::question(this, "Покупка собственности",
                                          QString("Хотите купить %1 за %2₼?").arg(cells[nextProperty].name).arg(cells[nextProperty].price),
                                          QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes) {
                    buyProperty(nextProperty);
                }
            }
        }
        break;

    case JailRelated:
        if (card.keepCard) {
            // Карта выхода из тюрьмы
            currentPlayer.hasJailCard = true;
            QMessageBox::information(this, "Выход из тюрьмы", "Вы получили карту выхода из тюрьмы. Сохраните ее.");
        } else {
            // Отправка в тюрьму
            currentPlayer.inJail = true;
            currentPlayer.jailTurns = 3;

            playerPositions[currentPlayerIndex] = 1; // Позиция тюрьмы
            auto [row, col] = path[1];
            players[currentPlayerIndex]->moveTo(boardLayout, row, col);

            QMessageBox::information(this, "Тюрьма", "Вы отправляетесь в тюрьму!");
        }
        break;

    case PropertyRelated:
        // Действия, связанные с собственностью (например, оплата за каждый дом)
        if (card.value < 0) {
            int totalCost = 0;

            // Подсчет стоимости за дома и отели
            for (int propIndex : currentPlayer.properties) {
                if (cells[propIndex].houseCount == 5) {
                    // Отель (стоимость отеля обычно больше)
                    totalCost += 100;
                } else {
                    // Дома
                    totalCost += -card.value * cells[propIndex].houseCount;
                }
            }

            if (totalCost > 0) {
                currentPlayer.money -= totalCost;
                QMessageBox::information(this, "Ремонт",
                                         QString("Вы заплатили %1₼ за ремонт домов").arg(totalCost));
            }
        }
        break;

    case SkipTurn:
        // Реализация пропуска хода
        QMessageBox::information(this, "Пропуск хода", "Вы пропускаете следующий ход");
        break;
    }

    // Обновляем отображение информации об игроке
    updatePlayerInfoDisplay();
}

void GameWindow::buyProperty(int cellIndex)
{
    CellInfo& info = cells[cellIndex];
    PlayerState& currentPlayer = playerStates[currentPlayerIndex];

    // Проверяем, есть ли у игрока достаточно денег
    if (currentPlayer.money >= info.price) {
        // Списываем деньги
        currentPlayer.money -= info.price;

        // Назначаем собственность игроку
        info.owner = currentPlayerIndex;
        currentPlayer.properties.append(cellIndex);

        QMessageBox::information(this, "Покупка успешна",
                                 QString("Вы приобрели %1 за %2₼").arg(info.name).arg(info.price));

        // Обновляем отображение информации
        updatePlayerInfoDisplay();
    } else {
        QMessageBox::warning(this, "Недостаточно средств",
                             QString("У вас недостаточно денег для покупки %1").arg(info.name));
    }
}

void GameWindow::payRent(int cellIndex)
{
    CellInfo& info = cells[cellIndex];

    if (info.owner == -1 || info.isMortgaged) {
        // Если нет владельца или собственность заложена, рента не платится
        return;
    }

    int rentAmount = calculateRent(cellIndex);

    // Списываем ренту у текущего игрока
    playerStates[currentPlayerIndex].money -= rentAmount;

    // Начисляем ренту владельцу
    playerStates[info.owner].money += rentAmount;

    // Определяем цвет владельца
    QString ownerColor;
    switch (players[info.owner]->getColor()) {
    case Player::Red: ownerColor = "красному"; break;
    case Player::Blue: ownerColor = "синему"; break;
    case Player::Green: ownerColor = "зеленому"; break;
    case Player::Yellow: ownerColor = "желтому"; break;
    }

    QMessageBox::information(this, "Оплата ренты",
                             QString("Вы заплатили %1₼ ренты %2 игроку").arg(rentAmount).arg(ownerColor));

    // Обновляем отображение информации
    updatePlayerInfoDisplay();
}

int GameWindow::calculateRent(int cellIndex)
{
    CellInfo& info = cells[cellIndex];

    if (info.type == Property) {
        // Для обычной собственности
        int baseRent = info.rent;

        // Проверяем, владеет ли игрок всеми свойствами этого цвета
        if (playerOwnsAllPropertiesInGroup(info.owner, info.colorGroup)) {
            baseRent *= 2;
        }

        // Увеличиваем ренту в зависимости от количества домов
        switch (info.houseCount) {
        case 0: return baseRent;
        case 1: return baseRent * 5;
        case 2: return baseRent * 15;
        case 3: return baseRent * 45;
        case 4: return baseRent * 80;
        case 5: return baseRent * 125; // Отель
        }
    } else if (info.type == Railroad) {
        // Для вокзалов
        int railroadCount = 0;

        // Подсчитываем, сколько вокзалов у владельца
        for (int propIndex : playerStates[info.owner].properties) {
            if (cells[propIndex].type == Railroad) {
                railroadCount++;
            }
        }

        // Рента за вокзалы: 25, 50, 100, 200
        return 25 * (1 << (railroadCount - 1));
    }

    // По умолчанию
    return info.rent;
}

bool GameWindow::playerOwnsAllPropertiesInGroup(int playerId, const QString& colorGroup)
{
    // Подсчитываем количество свойств в группе
    int totalInGroup = 0;
    int playerOwnsInGroup = 0;

    for (int i = 0; i < cells.size(); i++) {
        if (cells[i].colorGroup == colorGroup && cells[i].type == Property) {
            totalInGroup++;
            if (cells[i].owner == playerId) {
                playerOwnsInGroup++;
            }
        }
    }

    return totalInGroup > 0 && playerOwnsInGroup == totalInGroup;
}

void GameWindow::mortgageProperty(int cellIndex)
{
    CellInfo& info = cells[cellIndex];
    PlayerState& currentPlayer = playerStates[currentPlayerIndex];

    // Проверяем, принадлежит ли собственность игроку и не заложена ли она уже
    if (info.owner == currentPlayerIndex && !info.isMortgaged) {
        // Закладываем собственность
        info.isMortgaged = true;
        currentPlayer.mortgagedProperties.append(cellIndex);

        // Получаем деньги (половина стоимости)
        int mortgageValue = info.price / 2;
        currentPlayer.money += mortgageValue;

        QMessageBox::information(this, "Залог собственности",
                                 QString("Вы заложили %1 и получили %2₼").arg(info.name).arg(mortgageValue));

        // Обновляем отображение информации
        updatePlayerInfoDisplay();
    }
}

void GameWindow::unmortgageProperty(int cellIndex)
{
    CellInfo& info = cells[cellIndex];
    PlayerState& currentPlayer = playerStates[currentPlayerIndex];

    // Проверяем, принадлежит ли собственность игроку и заложена ли она
    if (info.owner == currentPlayerIndex && info.isMortgaged) {
        // Стоимость выкупа (110% от суммы заклада)
        int unmortgagePrice = (info.price / 2) * 1.1;

        // Проверяем, есть ли у игрока достаточно денег
        if (currentPlayer.money >= unmortgagePrice) {
            // Выкупаем собственность
            info.isMortgaged = false;
            currentPlayer.money -= unmortgagePrice;

            // Удаляем из списка заложенной собственности
            currentPlayer.mortgagedProperties.removeAll(cellIndex);

            QMessageBox::information(this, "Выкуп собственности",
                                     QString("Вы выкупили %1 за %2₼").arg(info.name).arg(unmortgagePrice));

            // Обновляем отображение информации
            updatePlayerInfoDisplay();
        } else {
            QMessageBox::warning(this, "Недостаточно средств",
                                 QString("У вас недостаточно денег для выкупа %1").arg(info.name));
        }
    }
}

void GameWindow::buildHouse(int cellIndex)
{
    CellInfo& info = cells[cellIndex];
    PlayerState& currentPlayer = playerStates[currentPlayerIndex];

    // Проверяем, принадлежит ли собственность игроку, не заложена ли она, и можно ли построить дом
    if (info.owner == currentPlayerIndex && !info.isMortgaged && info.type == Property) {
        // Проверяем, владеет ли игрок всеми свойствами этой группы (монополия)
        if (playerOwnsAllPropertiesInGroup(currentPlayerIndex, info.colorGroup)) {
            // Проверяем, не достигнуто ли максимальное количество домов
            if (info.houseCount < 5) {
                // Проверяем, есть ли у игрока достаточно денег
                if (currentPlayer.money >= info.housePrice) {
                    // Строим дом
                    info.houseCount++;
                    currentPlayer.money -= info.housePrice;

                    if (info.houseCount == 5) {
                        QMessageBox::information(this, "Строительство отеля",
                                                 QString("Вы построили отель на %1 за %2₼").arg(info.name).arg(info.housePrice));
                    } else {
                        QMessageBox::information(this, "Строительство дома",
                                                 QString("Вы построили дом #%1 на %2 за %3₼")
                                                     .arg(info.houseCount)
                                                     .arg(info.name)
                                                     .arg(info.housePrice));
                    }

                    // Увеличиваем общее количество домов игрока
                    if (info.houseCount == 5) {
                        currentPlayer.houseCount -= 4; // Удаляем 4 дома
                        currentPlayer.hotelCount++; // Добавляем 1 отель
                    } else {
                        currentPlayer.houseCount++;
                    }

                    // Обновляем отображение информации
                    updatePlayerInfoDisplay();
                } else {
                    QMessageBox::warning(this, "Недостаточно средств",
                                         QString("У вас недостаточно денег для строительства дома на %1").arg(info.name));
                }
            } else {
                QMessageBox::warning(this, "Максимум строений", "На этой собственности уже построен отель");
            }
        } else {
            QMessageBox::warning(this, "Нет монополии",
                                 "Для строительства домов вы должны владеть всеми собственностями этого цвета");
        }
    }
}

void GameWindow::updatePlayerInfoDisplay()
{
    // Обновляем информацию о каждом игроке
    for (int i = 0; i < playerCount; i++) {
        QString playerName;
        QString playerColor;

        switch (players[i]->getColor()) {
        case Player::Red:
            playerName = "Красный";
            playerColor = "red";
            break;
        case Player::Blue:
            playerName = "Синий";
            playerColor = "blue";
            break;
        case Player::Green:
            playerName = "Зеленый";
            playerColor = "green";
            break;
        case Player::Yellow:
            playerName = "Желтый";
            playerColor = "yellow";
            break;
        }

        // Формируем текст с информацией об игроке
        QString infoText = QString("<span style='color: %1;'>%2 игрок</span><br>")
                               .arg(playerColor).arg(playerName);

        infoText += QString("Деньги: <b>%1₼</b><br>").arg(playerStates[i].money);

        if (playerStates[i].bankrupt) {
            infoText += "<span style='color: red;'><b>БАНКРОТ</b></span><br>";
        }
        else {
            if (!playerStates[i].properties.isEmpty()) {
                infoText += "Собственность:<br>";
                for (int propIndex : playerStates[i].properties) {
                    QString propInfo = cells[propIndex].name;

                    if (cells[propIndex].isMortgaged) {
                        propInfo += " (заложена)";
                    }

                    if (cells[propIndex].houseCount > 0) {
                        if (cells[propIndex].houseCount == 5) {
                            propInfo += " (отель)";
                        } else {
                            propInfo += QString(" (%1 дом(а))").arg(cells[propIndex].houseCount);
                        }
                    }

                    infoText += QString("- %1<br>").arg(propInfo);
                }
            }

            if (playerStates[i].inJail) {
                infoText += QString("<span style='color: red;'>В тюрьме (осталось ходов: %1)</span><br>").arg(playerStates[i].jailTurns);
            }

            if (playerStates[i].hasJailCard) {
                infoText += "Имеет карту выхода из тюрьмы<br>";
            }
        }

        playerLabels[i]->setText(infoText);
    }
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
