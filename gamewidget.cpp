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

    initializeCells();
    initializeCards();

    playerStates.clear();
    for (int i = 0; i < playerCount; i++) {
        playerStates.append(PlayerState());
    }

    QScreen *screen = QGuiApplication::primaryScreen();
    QRect screenGeometry = screen->geometry();
    int screenHeight = screenGeometry.height();

    int baseSize = screenHeight / 12;


    setMinimumSize(screenGeometry.width(), screenGeometry.height());
    setStyleSheet("background-color: #f0f0f0;");

    showFullScreen();


    ui -> cube_roll -> setParent(this);
    ui -> quit_button -> setParent(this);
    ui -> monopoly -> setParent(this);

    int rightMargin = 20;
    int bottomMargin = 20;
    int buttonSpacing = 10;
    int buttonWidth = 100;
    int buttonHeight = 40;

  /*  ui -> go_button -> setGeometry
        (
            screenGeometry.width() - rightMargin - buttonWidth,
            screenGeometry.height() - bottomMargin - buttonHeight * 4 - buttonSpacing * 3,
            buttonWidth, buttonHeight
            );*/

    ui -> monopoly -> setGeometry
        (
            screenGeometry.width() - rightMargin - buttonWidth,
            screenGeometry.height() - bottomMargin - buttonHeight * 3 - buttonSpacing * 2,
            buttonWidth, buttonHeight
            );


    ui -> cube_roll -> setGeometry
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

    ui -> cube_roll -> raise();
    ui -> quit_button ->raise();

    ui -> monopoly -> raise();

    //ui -> go_button -> raise();

    mainLayout = new QGridLayout(this);
    mainLayout->setSpacing(0);
    mainLayout->setContentsMargins(0, 0, 0, 0);


    boardLayout = new QGridLayout();
    boardLayout->setSpacing(0);
    boardLayout->setContentsMargins(0, 0, 0, 0);


    CellWidget *cornerBottomRight = new CellWidget(cells[0]);
    CellWidget *cornerBottomLeft = new CellWidget(cells[1]);
    CellWidget *cornerTopLeft = new CellWidget(cells[2]);
    CellWidget *cornerTopRight = new CellWidget(cells[3]);

    int cornerSize = baseSize * 1.5;
    cornerBottomRight->setFixedSize(cornerSize, cornerSize);
    cornerBottomLeft->setFixedSize(cornerSize, cornerSize);
    cornerTopLeft->setFixedSize(cornerSize, cornerSize);
    cornerTopRight->setFixedSize(cornerSize, cornerSize);

    boardLayout->addWidget(cornerBottomRight, 10, 10);
    boardLayout->addWidget(cornerBottomLeft, 10, 0);
    boardLayout->addWidget(cornerTopLeft, 0, 0);
    boardLayout->addWidget(cornerTopRight, 0, 10);

    for (int i = 1; i < 10; i++) {
        CellWidget *cell = new CellWidget(cells[i + 3]);
        cell->setFixedSize(baseSize, baseSize * 1.5);
        boardLayout->addWidget(cell, 10, 10 - i);
    }

    for (int i = 1; i < 10; i++) {
        CellWidget *cell = new CellWidget(cells[i + 12]);
        cell->setFixedSize(baseSize * 1.5, baseSize);
        boardLayout->addWidget(cell, 10 - i, 0);
    }

    for (int i = 1; i < 10; i++) {
        CellWidget *cell = new CellWidget(cells[i + 21]);
        cell->setFixedSize(baseSize, baseSize * 1.5);
        boardLayout->addWidget(cell, 0, i);
    }

    for (int i = 1; i < 10; i++) {
        CellWidget *cell = new CellWidget(cells[i + 30]);
        cell->setFixedSize(baseSize * 1.5, baseSize);
        boardLayout->addWidget(cell, i, 10);
    }


    QWidget *centralArea = new QWidget();
    centralArea->setStyleSheet("background-color: #EAEAEA;");

    QVBoxLayout *centralLayout = new QVBoxLayout(centralArea);

    QLabel *gameTitle = new QLabel("MONOPOLY");
    gameTitle->setAlignment(Qt::AlignCenter);
    gameTitle->setStyleSheet("font-size: 24pt; font-weight: bold; color: #990000;");
    centralLayout->addWidget(gameTitle);

    playerTurnLabel = new QLabel("Ход игрока 1");
    playerTurnLabel->setAlignment(Qt::AlignCenter);
    playerTurnLabel->setStyleSheet("font-size: 18pt; font-weight: bold; color: #990000;");
    centralLayout->addWidget(playerTurnLabel);

    boardLayout->addWidget(centralArea, 1, 1, 9, 9);

    QWidget *boardContainer = new QWidget();
    boardContainer->setLayout(boardLayout);

    int boardSize = screenHeight;
    boardContainer->setFixedSize(boardSize, boardSize);

    mainLayout->addWidget(boardContainer, 0, 0, 1, 1, Qt::AlignCenter);

    setLayout(mainLayout);

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

    Player::PlayerColor colors[] = {
        Player::Red,
        Player::Blue,
        Player::Green,
        Player::Yellow
    };

    for (int i = 0; i < playerCount; i++) {
        players.append(new Player(colors[i], boardContainer));
        playerPositions.append(0);

        auto [row, col] = path[0];
        QLayoutItem *item = boardLayout->itemAtPosition(row, col);

        if (item && item->widget()) {
            QWidget *cell = item->widget();
            players[i]->setParent(cell);
            players[i]->move(5 + i*5, 5 + i*5);
            players[i]->show();
        }
    }

    for (int i = 0; i < path.size(); i++)
    {
        auto [row, col] = path[i];
        QLayoutItem *item = boardLayout -> itemAtPosition(row, col);

        if (item && item->widget())
        {
            CellWidget *cell = qobject_cast<CellWidget*>(item -> widget());
            if (cell)
            {
                const int cellIdx = i;

                connect(cell, &CellWidget::clicked, this, [this, cellIdx]()
                {
                    select_cell(cellIdx);
                });
            }
        }
    }


    //connect(ui -> go_button, &QPushButton::clicked, this, [=]() {move_player(1);});
    connect(ui -> cube_roll, &QPushButton::clicked, this, &GameWindow::start_cubes_roll);

   /* connect(ui->build_houses, &QPushButton::clicked, this, [this]()
    {
        if (selected_cell_index >= 0 && selected_cell_index < path.size())
        {
            auto [row, col] = path[selected_cell_index];
            QLayoutItem *item = boardLayout->itemAtPosition(row, col);
            if (item && item->widget())
            {
                CellWidget *cellWidget = qobject_cast<CellWidget*>(item->widget());
                if (cellWidget)
                {
                    cellWidget->build_house();
            //        qDebug() << "Построен дом на клетке" << selected_cell_index;
                }
            }
        }
        else
        {
       //     qDebug() << "Сначала выберите клетку для строительства";
        }
    });*/

 //   int index = 3;

 //   connect(ui -> monop, &QPushButton::clicked, this, [this, index](){showHouseManagementDialog(index);});


    cube_label_1 = new QLabel(this);
    cube_label_2 = new QLabel(this);

    cube_label_1->setScaledContents(true);
    cube_label_2->setScaledContents(true);

    cube_label_1->setVisible(false);
    cube_label_2->setVisible(false);

    createPlayerInfoWidget();
    updatePlayerInfoDisplay();

    updatePlayerTurnLabel();
}

void GameWindow::initializeCells() {

    cells.clear();


    cells.append({"GO", 0, "../../resources/go.png", Corner, -1, false, -1, "", 0, 0, 0});  // Start
    cells.append({"AKADEM", 0, "../../resources/academ.png", Corner, -1, false, -1, "", 0, 0, 0});  // Jail
    cells.append({"FREE PARKING", 0, "../../resources/parking.png", Corner, -1, false, -1, "", 0, 0, 0});  // Free Parking
    cells.append({"GO TO AKADEM", 0, "../../resources/go_to_academ.png", Corner, -1, false, -1, "", 0, 0, 0});  // Go to Jail

    cells.append({"Казна", 0, "../../resources/treasury.png", Treasury, -1, false, -1, "", 0, 0, 0});
    cells.append({"Кюрдямир", 60, "../../resources/colour1.png", Property, -1, false, -1, "brown", 2, 50, 0});
    cells.append({"Шамахы", 80, "../../resources/colour1.png", Property, -1, false, -1, "brown", 4, 50, 0});
    cells.append({"Шанс", 0, "../../resources/clever.png", Chance, -1, false, -1, "", 0, 0, 0});
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
    cells.append({"Шанс", 0, "../../resources/clever.png", Chance, -1, false, -1, "", 0, 0, 0});
    cells.append({"Астара", 200, "../../resources/colour4.png", Property, -1, false, -1, "orange", 16, 100, 0});
    cells.append({"Лерик", 200, "../../resources/colour4.png", Property, -1, false, -1, "orange", 16, 100, 0});
    cells.append({"Ленкорань", 220, "../../resources/colour4.png", Property, -1, false, -1, "orange", 18, 100, 0});



    cells.append({"Джульфа", 220, "../../resources/colour5.png", Property, -1, false, -1, "red", 18, 150, 0});
    cells.append({"Ордубад", 220, "../../resources/colour5.png", Property, -1, false, -1, "red", 20, 150, 0});
    cells.append({"Нахычеван", 240, "../../resources/colour5.png", Property, -1, false, -1, "red", 22, 150, 0});
    cells.append({"Шанс", 0, "../../resources/clever.png", Chance, -1, false, -1, "", 0, 0, 0});
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
    cells.append({"Шанс", 0, "../../resources/clever.png", Chance, -1, false, -1, "", 0, 0, 0});
    cells.append({"Сумгайыт", 360, "../../resources/colour8.png", Property, -1, false, -1, "blue", 35, 200, 0});
    cells.append({"Баку", 400, "../../resources/colour8.png", Property, -1, false, -1, "blue", 50, 200, 0});
    cells.append({"Сверхналог", 100, "../../resources/dollar.png", Tax, -1, false, -1, "", 0, 0, 0});
}

void GameWindow::initializeCards() {

    treasuryCards = {
        Card("Банковская ошибка в вашу пользу. Получите 200₼", Money, 200),
        Card("Оплатите счет за лечение - 50₼", Money, -50),
        Card("Вы выиграли в лотерею. Получите 100₼", Money, 100),
        Card("С днем рождения! Получите 50₼ от каждого игрока", Money, 50),
        Card("Налог на имущество. Заплатите 25₼ за каждый дом", PropertyRelated, -25),
        Card("Выход из тюрьмы. Сохраните эту карту", JailRelated, 0, -1, true),
        Card("Банк выплачивает вам дивиденды в размере 50₼", Money, 50),
        Card("Вы заняли первое место в конкурсе красоты. Получите 100₼", Money, 100),
        Card("Возвращайтесь на Старт", Movement, 0, 0),
        Card("Оплатите страховку здоровья - 50₼", Money, -50),
        Card("Оплатите услуги доктора - 100₼", Money, -100),
        Card("Получите 25₼ за консультационные услуги", Money, 25),
        Card("Вы наследуете 100₼", Money, 100),
        Card("Школьные сборы. Заплатите 150₼", Money, -150),
        Card("Возврат налога. Получите 20₼", Money, 20),
        Card("Ваш день рождения. Получите 10₼ от каждого игрока", Money, 10)
    };


    chanceCards = {
        Card("Отправляйтесь на Старт", Movement, 0, 0),
        Card("Отправляйтесь в тюрьму. Идите прямо в тюрьму. Не проходите Старт.", JailRelated, 0, 10),
        Card("Отправляйтесь на Шушу", Movement, 0, 12),
        Card("Отправляйтесь на ближайший вокзал", Movement, 0, -2),
        Card("Банк выплачивает вам дивиденды в размере 50₼", Money, 50),
        Card("Выход из тюрьмы. Сохраните эту карту", JailRelated, 0, -1, true),
        Card("Вернитесь на 3 поля назад", Movement, -3),
        Card("Отправляйтесь на Баку", Movement, 0, 38),
        Card("Общий ремонт собственности: заплатите 25₼ за каждый дом и 100₼ за отель", PropertyRelated, -1),
        Card("Штраф за превышение скорости - 15₼", Money, -15),
        Card("Штраф за пьянство - 20₼", Money, -20),
        Card("Продвинитесь на следующую ячейку Казны", Movement, 0, -3),
        Card("Вы избраны председателем правления. Выплатите каждому игроку 50₼", Money, -50),
        Card("Ваша ссуда на строительство выплачена. Получите 150₼", Money, 150),
        Card("Переместитесь на 5 полей вперед", Movement, 5),
        Card("Продвинитесь до следующей ячейки собственности. Если она свободна, вы можете её купить", Movement, 0, -4)
    };
}

void GameWindow::createPlayerInfoWidget() {
    playerInfoWidget = new QWidget(this);
    playerInfoWidget->setStyleSheet("background-color: white; border: 1px solid black;");
    playerInfoWidget->setFixedWidth(250);

    QVBoxLayout* infoLayout = new QVBoxLayout(playerInfoWidget);


    QLabel* headerLabel = new QLabel("Информация об игроках");
    headerLabel->setAlignment(Qt::AlignCenter);
    headerLabel->setStyleSheet("font-weight: bold; font-size: 14px;");
    infoLayout->addWidget(headerLabel);


    playerLabels.clear();
    for (int i = 0; i < playerCount; i++) {
        QLabel* playerLabel = new QLabel();
        playerLabel->setTextFormat(Qt::RichText);
        playerLabel->setWordWrap(true);
        playerLabels.append(playerLabel);
        infoLayout->addWidget(playerLabel);

        if (i < playerCount - 1) {
            QFrame* line = new QFrame();
            line->setFrameShape(QFrame::HLine);
            line->setFrameShadow(QFrame::Sunken);
            infoLayout->addWidget(line);
        }
    }


    mainLayout->addWidget(playerInfoWidget, 0, 1, 1, 1, Qt::AlignRight | Qt::AlignTop);
}


void GameWindow::updatePlayerTurnLabel() {

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
    if (playerStates[currentPlayerIndex].inJail) {
        QMessageBox msgBox(this);
        msgBox.setWindowTitle("Тюрьма");
        msgBox.setText("Вы в тюрьме! Выберите один из вариантов:");
        msgBox.setIcon(QMessageBox::Question);

        QPushButton *rollButton = msgBox.addButton("Бросить кубики", QMessageBox::ActionRole);
        QPushButton *payButton = msgBox.addButton("Заплатить 50₼", QMessageBox::ActionRole);
        QPushButton *cardButton = nullptr;

        if (playerStates[currentPlayerIndex].hasJailCard) {
            cardButton = msgBox.addButton("Использовать карту", QMessageBox::ActionRole);
        }

        QPushButton *stayButton = msgBox.addButton("Остаться в тюрьме", QMessageBox::ActionRole);

        msgBox.exec();

        if (msgBox.clickedButton() == payButton) {
            if (playerStates[currentPlayerIndex].money >= 50) {
                playerStates[currentPlayerIndex].money -= 50;
                playerStates[currentPlayerIndex].inJail = false;
                playerStates[currentPlayerIndex].jailTurns = 0;
                QMessageBox::information(this, "Выход из тюрьмы", "Вы заплатили 50₼ и вышли из тюрьмы!");
                updatePlayerInfoDisplay();

                int oldPosition = playerPositions[currentPlayerIndex];
                playerPositions[currentPlayerIndex] = (playerPositions[currentPlayerIndex] + steps) % path.size();
                auto [row, col] = path[playerPositions[currentPlayerIndex]];
                players[currentPlayerIndex]->moveTo(boardLayout, row, col);

                if (playerPositions[currentPlayerIndex] < oldPosition && playerPositions[currentPlayerIndex] != 10) {
                    playerStates[currentPlayerIndex].money += 200;
                    QMessageBox::information(this, "Пересечение Старта", "Вы прошли через Старт и получили 200₼!");
                    updatePlayerInfoDisplay();
                }

                qDebug() << "Игрок переместился на позицию" << playerPositions[currentPlayerIndex];
                check_cell_type();
                return;
            } else {
                QMessageBox::warning(this, "Недостаточно средств", "У вас недостаточно денег для оплаты выхода из тюрьмы!");
                next_player();
                return;
            }
        } else if (cardButton != nullptr && msgBox.clickedButton() == cardButton) {
            playerStates[currentPlayerIndex].hasJailCard = false;
            playerStates[currentPlayerIndex].inJail = false;
            playerStates[currentPlayerIndex].jailTurns = 0;
            QMessageBox::information(this, "Выход из тюрьмы", "Вы использовали карту и вышли из тюрьмы!");

            int oldPosition = playerPositions[currentPlayerIndex];
            playerPositions[currentPlayerIndex] = (playerPositions[currentPlayerIndex] + steps) % path.size();
            auto [row, col] = path[playerPositions[currentPlayerIndex]];
            players[currentPlayerIndex]->moveTo(boardLayout, row, col);

            if (playerPositions[currentPlayerIndex] < oldPosition && playerPositions[currentPlayerIndex] != 10) {
                playerStates[currentPlayerIndex].money += 200;
                QMessageBox::information(this, "Пересечение Старта", "Вы прошли через Старт и получили 200₼!");
                updatePlayerInfoDisplay();
            }

            qDebug() << "Игрок переместился на позицию" << playerPositions[currentPlayerIndex];
            check_cell_type();
            return;
        } else if (msgBox.clickedButton() == rollButton) {
            if (lastRollWasDouble) {
                playerStates[currentPlayerIndex].inJail = false;
                playerStates[currentPlayerIndex].jailTurns = 0;
                QMessageBox::information(this, "Выход из тюрьмы", "Вы выбросили дубль и выходите из тюрьмы!");

                int oldPosition = playerPositions[currentPlayerIndex];
                playerPositions[currentPlayerIndex] = (playerPositions[currentPlayerIndex] + steps) % path.size();
                auto [row, col] = path[playerPositions[currentPlayerIndex]];
                players[currentPlayerIndex]->moveTo(boardLayout, row, col);

                if (playerPositions[currentPlayerIndex] < oldPosition && playerPositions[currentPlayerIndex] != 10) {
                    playerStates[currentPlayerIndex].money += 200;
                    QMessageBox::information(this, "Пересечение Старта", "Вы прошли через Старт и получили 200₼!");
                    updatePlayerInfoDisplay();
                }

                qDebug() << "Игрок переместился на позицию" << playerPositions[currentPlayerIndex];
                check_cell_type();
                return;
            } else {
                playerStates[currentPlayerIndex].jailTurns--;

                if (playerStates[currentPlayerIndex].jailTurns <= 0) {
                    playerStates[currentPlayerIndex].inJail = false;
                    QMessageBox::information(this, "Тюрьма", "Ваше время в тюрьме истекло, вы свободны!");

                    int oldPosition = playerPositions[currentPlayerIndex];
                    playerPositions[currentPlayerIndex] = (playerPositions[currentPlayerIndex] + steps) % path.size();
                    auto [row, col] = path[playerPositions[currentPlayerIndex]];
                    players[currentPlayerIndex]->moveTo(boardLayout, row, col);

                    if (playerPositions[currentPlayerIndex] < oldPosition && playerPositions[currentPlayerIndex] != 10) {
                        playerStates[currentPlayerIndex].money += 200;
                        QMessageBox::information(this, "Пересечение Старта", "Вы прошли через Старт и получили 200₼!");
                        updatePlayerInfoDisplay();
                    }

                    qDebug() << "Игрок переместился на позицию" << playerPositions[currentPlayerIndex];
                    check_cell_type();
                    return;
                } else {
                    QMessageBox::information(this, "Тюрьма", QString("Вы не выбросили дубль и остаётесь в тюрьме. Осталось ходов: %1").arg(playerStates[currentPlayerIndex].jailTurns));
                    next_player();
                    return;
                }
            }
        } else if (msgBox.clickedButton() == stayButton) {
            playerStates[currentPlayerIndex].jailTurns--;

            if (playerStates[currentPlayerIndex].jailTurns <= 0) {
                playerStates[currentPlayerIndex].inJail = false;
                QMessageBox::information(this, "Тюрьма", "Ваше время в тюрьме истекло, вы свободны!");

                int oldPosition = playerPositions[currentPlayerIndex];
                playerPositions[currentPlayerIndex] = (playerPositions[currentPlayerIndex] + steps) % path.size();
                auto [row, col] = path[playerPositions[currentPlayerIndex]];
                players[currentPlayerIndex]->moveTo(boardLayout, row, col);

                if (playerPositions[currentPlayerIndex] < oldPosition && playerPositions[currentPlayerIndex] != 10) {
                    playerStates[currentPlayerIndex].money += 200;
                    QMessageBox::information(this, "Пересечение Старта", "Вы прошли через Старт и получили 200₼!");
                    updatePlayerInfoDisplay();
                }

                qDebug() << "Игрок переместился на позицию" << playerPositions[currentPlayerIndex];
                check_cell_type();
                return;
            } else {
                QMessageBox::information(this, "Тюрьма", QString("Вы остаётесь в тюрьме. Осталось ходов: %1").arg(playerStates[currentPlayerIndex].jailTurns));
                next_player();
                return;
            }
        }

        return;
    }

    int oldPosition = playerPositions[currentPlayerIndex];
    playerPositions[currentPlayerIndex] = (playerPositions[currentPlayerIndex] + steps) % path.size();
    auto [row, col] = path[playerPositions[currentPlayerIndex]];
    players[currentPlayerIndex]->moveTo(boardLayout, row, col);

    int ind = playerPositions[currentPlayerIndex];

    connect(ui -> monopoly, &QPushButton::clicked, this, [this, ind]() {showHouseManagementDialog(ind);});

    if (playerPositions[currentPlayerIndex] < oldPosition && playerPositions[currentPlayerIndex] != 10) {
        playerStates[currentPlayerIndex].money += 200;
        QMessageBox::information(this, "Пересечение Старта", "Вы прошли через Старт и получили 200₼!");
        updatePlayerInfoDisplay();
    }

  //  qDebug() << "Игрок переместился на позицию" << playerPositions[currentPlayerIndex];
    check_cell_type();
}

void GameWindow::next_player()
{
    if (playerStates[currentPlayerIndex].money < 0) {
        if (!handleBankruptcy()) {
            playerStates[currentPlayerIndex].bankrupt = true;
            QMessageBox::warning(this, "Банкротство",
                                 "Вы обанкротились! Ваша собственность возвращается банку.");

            for (int propIndex : playerStates[currentPlayerIndex].properties) {
                cells[propIndex].owner = -1;
                cells[propIndex].isMortgaged = false;
                cells[propIndex].houseCount = 0;
            }

            playerStates[currentPlayerIndex].properties.clear();
            playerStates[currentPlayerIndex].mortgagedProperties.clear();

            int remainingPlayers = 0;
            int lastPlayerIndex = -1;

            for (int i = 0; i < playerCount; i++) {
                if (!playerStates[i].bankrupt) {
                    remainingPlayers++;
                    lastPlayerIndex = i;
                }
            }

            if (remainingPlayers <= 1) {

                QString winnerColor;
                switch (players[lastPlayerIndex]->getColor()) {
                case Player::Red: winnerColor = "Красный"; break;
                case Player::Blue: winnerColor = "Синий"; break;
                case Player::Green: winnerColor = "Зеленый"; break;
                case Player::Yellow: winnerColor = "Желтый"; break;
                }

                QMessageBox::information(this, "Игра окончена",
                                         QString("%1 игрок победил в игре!").arg(winnerColor));

                emit return_to_menu();
                this->hide();
                return;
            }
        }
    }

    do {
        currentPlayerIndex = (currentPlayerIndex + 1) % playerCount;
    } while (playerStates[currentPlayerIndex].bankrupt);

    updatePlayerTurnLabel();
}

bool GameWindow::handleBankruptcy() {
    int deficit = -playerStates[currentPlayerIndex].money;
    int availableFunds = 0;

    for (int propIndex : playerStates[currentPlayerIndex].properties) {
        if (cells[propIndex].houseCount > 0) {
            availableFunds += (cells[propIndex].housePrice * cells[propIndex].houseCount) / 2;
        }
    }

    for (int propIndex : playerStates[currentPlayerIndex].properties) {
        if (!cells[propIndex].isMortgaged) {
            availableFunds += cells[propIndex].price / 2;
        }
    }

    if (availableFunds >= deficit) {
        QMessageBox::warning(this, "Опасность банкротства",
                             QString("У вас отрицательный баланс: %1₼. Вам нужно продать дома или заложить собственность.").arg(playerStates[currentPlayerIndex].money));


        int neededMoney = deficit;

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
        return true;
    }

    return false;
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

                    // Определяем, был ли выброшен дубль
                    lastRollWasDouble = (final1 == final2);

                    // Убираем обработку выхода из тюрьмы отсюда, она будет в move_player

                    QTimer::singleShot(2200, this, [=]()
                                       {
                                           cube_label_1->setVisible(false);
                                           cube_label_2->setVisible(false);

                                           move_player(final1 + final2);

                                           if (lastRollWasDouble && !playerStates[currentPlayerIndex].inJail) {
                                               QMessageBox::information(this, "Дубль!", "Вы выбросили дубль! Ходите еще раз.");
                                           }
                                       });
                }
            });

    cube_roll_timer->start(100);
}

int GameWindow::GetCellIndex(int pathIndex)
{
    if (pathIndex == 0) return 0;
    if (pathIndex == 10) return 1;
    if (pathIndex == 20) return 2;
    if (pathIndex == 30) return 3;


    if (pathIndex > 0 && pathIndex < 10) return pathIndex + 3;
    if (pathIndex > 10 && pathIndex < 20) return (pathIndex - 10) + 12;
    if (pathIndex > 20 && pathIndex < 30) return (pathIndex - 20) + 21;
    if (pathIndex > 30 && pathIndex < 40) return (pathIndex - 30) + 30;

    return 0;
}



void GameWindow::check_cell_type()
{
    int currentPosition = playerPositions[currentPlayerIndex];
    int cellIndex = GetCellIndex(currentPosition);
    CellInfo& info = cells[cellIndex];

    qDebug() << "Игрок на ячейке:" << info.name << "тип:" << static_cast<int>(info.type);

    switch (info.type)
    {
    case Treasury:
        if (!treasuryCards.isEmpty()) {
            int cardIndex = QRandomGenerator::global()->bounded(treasuryCards.size());
            Card card = treasuryCards[cardIndex];

            if (kaznaWidget) {
                kaznaLabel->setText(card.text);
                kaznaWidget->show();
            }

            processCard(card);

            if (!card.keepCard) {
                treasuryCards.removeAt(cardIndex);
                treasuryCards.append(card);
            }
        }
        break;

    case Chance:
        if (!chanceCards.isEmpty()) {
            int cardIndex = QRandomGenerator::global()->bounded(chanceCards.size());
            Card card = chanceCards[cardIndex];

            QMessageBox::information(this, "Шанс", card.text);

            processCard(card);

            if (!card.keepCard) {
                chanceCards.removeAt(cardIndex);
                chanceCards.append(card);
            }
        }
        break;

    case Property:
        if (info.owner == -1) {
            if (QMessageBox::question(this, "Покупка собственности",
                                      QString("Хотите купить %1 за %2₼?").arg(info.name).arg(info.price),
                                      QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes) {
                buyProperty(currentPosition);
            }
        } else if (info.owner != currentPlayerIndex) {
            if (!info.isMortgaged) {
                payRent(currentPosition);
            } else {
                QMessageBox::information(this, "Заложенная собственность",
                                         "Эта собственность заложена, рента не платится.");
            }
        }
        break;

    case Railroad:
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
    {
        int taxAmount = info.price;
        playerStates[currentPlayerIndex].money -= taxAmount;
        QMessageBox::information(this, "Налог",
                                 QString("Вы заплатили %1₼ налога").arg(taxAmount));
        updatePlayerInfoDisplay();
    }
    break;

    case Corner:
        if (currentPosition == 30) {  // 3 is the position in the cells array, 30 is the position in the path array
            QMessageBox::information(this, "В тюрьму", "Вы отправляетесь в тюрьму!");

            playerStates[currentPlayerIndex].inJail = true;
            playerStates[currentPlayerIndex].jailTurns = 3;

            playerPositions[currentPlayerIndex] = 10;
            auto [row, col] = path[10];
            players[currentPlayerIndex]->moveTo(boardLayout, row, col);
        }
        else if (currentPosition == 10) {
            QMessageBox::information(this, "Посещение тюрьмы", "Вы просто посещаете тюрьму.");
        }
        else if (currentPosition == 20) {
            QMessageBox::information(this, "Бесплатная парковка", "Отдохните на бесплатной парковке.");
        }
        break;
    }

    // Используем переменную lastRollWasDouble которую мы установили в start_cubes_roll

    // Only move to the next player if it's not a double roll or if player is in jail
    if (!lastRollWasDouble || playerStates[currentPlayerIndex].inJail) {
        QTimer::singleShot(1000, this, &GameWindow::next_player);
    } else {
        // If it's a double, update the player turn label to show it's still their turn
        QTimer::singleShot(1000, this, &GameWindow::updatePlayerTurnLabel);
    }
}




void GameWindow::processCard(const Card& card)
{
    PlayerState& currentPlayer = playerStates[currentPlayerIndex];

    switch (card.type) {
    case Money:
        if (card.value > 0) {
            currentPlayer.money += card.value;
            QMessageBox::information(this, "Деньги", QString("Вы получили %1₼").arg(card.value));
        } else if (card.value < 0) {
            currentPlayer.money += card.value;
            QMessageBox::information(this, "Деньги", QString("Вы заплатили %1₼").arg(-card.value));
        }
        break;

    case Movement:
        if (card.targetPosition >= 0) {
            int oldPosition = playerPositions[currentPlayerIndex];
            playerPositions[currentPlayerIndex] = card.targetPosition;
            auto [row, col] = path[card.targetPosition];
            players[currentPlayerIndex]->moveTo(boardLayout, row, col);

            if (card.targetPosition < oldPosition && card.targetPosition != 1) {
                currentPlayer.money += 200;
                QMessageBox::information(this, "Пересечение Старта", "Вы прошли через Старт и получили 200₼!");
            }

            check_cell_type();
        } else if (card.value != 0) {
            int newPosition = (playerPositions[currentPlayerIndex] + card.value) % path.size();
            if (newPosition < 0) newPosition += path.size();

            playerPositions[currentPlayerIndex] = newPosition;
            auto [row, col] = path[newPosition];
            players[currentPlayerIndex]->moveTo(boardLayout, row, col);

            check_cell_type();
        }
        else if (card.targetPosition == -2) {
            int currentPos = playerPositions[currentPlayerIndex];
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

                check_cell_type();
            }
        }
        else if (card.targetPosition == -3) {
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

                check_cell_type();
            }
        }
        else if (card.targetPosition == -4) {
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
            currentPlayer.hasJailCard = true;
            QMessageBox::information(this, "Выход из тюрьмы", "Вы получили карту выхода из тюрьмы. Сохраните ее.");
        } else if (card.targetPosition == 10) {
            // Go to jail card
            currentPlayer.inJail = true;
            currentPlayer.jailTurns = 3;

            playerPositions[currentPlayerIndex] = 10;
            auto [row, col] = path[10];
            players[currentPlayerIndex]->moveTo(boardLayout, row, col);

            QMessageBox::information(this, "Тюрьма", "Вы отправляетесь в тюрьму!");
        } else {
            // This is for any other jail-related card
            QMessageBox::information(this, "Тюрьма", card.text);
        }
        break;

    case PropertyRelated:
        if (card.value < 0) {
            int totalCost = 0;

            for (int propIndex : currentPlayer.properties) {
                if (cells[propIndex].houseCount == 5) {
                    totalCost += 100;
                } else {
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
        QMessageBox::information(this, "Пропуск хода", "Вы пропускаете следующий ход");
        break;
    }

    updatePlayerInfoDisplay();
}

void GameWindow::buyProperty(int cellIndex)
{
    CellInfo& info = cells[GetCellIndex(cellIndex)];
    PlayerState& currentPlayer = playerStates[currentPlayerIndex];

    if (currentPlayer.money >= info.price) {
        currentPlayer.money -= info.price;

        info.owner = currentPlayerIndex;
        currentPlayer.properties.append(GetCellIndex(cellIndex));

        QMessageBox::information(this, "Покупка успешна",
                                 QString("Вы приобрели %1 за %2₼").arg(info.name).arg(info.price));

        updatePlayerInfoDisplay();
    } else {
        QMessageBox::warning(this, "Недостаточно средств",
                             QString("У вас недостаточно денег для покупки %1").arg(info.name));
    }
}

void GameWindow::payRent(int cellIndex)
{
    CellInfo& info = cells[GetCellIndex(cellIndex)];

    if (info.owner == -1 || info.isMortgaged) {
        return;
    }

    int rentAmount = calculateRent(cellIndex);

    playerStates[currentPlayerIndex].money -= rentAmount;

    playerStates[info.owner].money += rentAmount;

    QString ownerColor;
    switch (players[info.owner]->getColor()) {
    case Player::Red: ownerColor = "красному"; break;
    case Player::Blue: ownerColor = "синему"; break;
    case Player::Green: ownerColor = "зеленому"; break;
    case Player::Yellow: ownerColor = "желтому"; break;
    }

    QMessageBox::information(this, "Оплата ренты",
                             QString("Вы заплатили %1₼ ренты %2 игроку").arg(rentAmount).arg(ownerColor));

    updatePlayerInfoDisplay();
}

int GameWindow::calculateRent(int cellIndex)
{
    int actualCellIndex = GetCellIndex(cellIndex);
    CellInfo& info = cells[actualCellIndex];

    if (info.type == Property) {
        int baseRent = info.rent;

        if (info.houseCount > 0) {
            switch (info.houseCount) {
            case 1: return baseRent * 5;
            case 2: return baseRent * 15;
            case 3: return baseRent * 45;
            case 4: return baseRent * 80;
            case 5: return baseRent * 125;
            }
        }

        if (playerOwnsAllPropertiesInGroup(info.owner, info.colorGroup)) {
            return baseRent * 2;
        }

        return baseRent;
    }
    else if (info.type == Railroad) {
        int railroadCount = 0;


        for (int propIndex : playerStates[info.owner].properties) {
            if (cells[propIndex].type == Railroad) {
                railroadCount++;
            }
        }

        return 25 * (1 << (railroadCount - 1));
    }

    return info.rent;
}

bool GameWindow::playerOwnsAllPropertiesInGroup(int playerId, const QString& colorGroup)
{
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
    CellInfo& info = cells[GetCellIndex(cellIndex)];
    PlayerState& currentPlayer = playerStates[currentPlayerIndex];

    if (info.owner == currentPlayerIndex && !info.isMortgaged) {
        info.isMortgaged = true;
        currentPlayer.mortgagedProperties.append(GetCellIndex(cellIndex));

        int mortgageValue = info.price / 2;
        currentPlayer.money += mortgageValue;

        QMessageBox::information(this, "Залог собственности",
                                 QString("Вы заложили %1 и получили %2₼").arg(info.name).arg(mortgageValue));

        updatePlayerInfoDisplay();
    }
}

void GameWindow::unmortgageProperty(int cellIndex)
{
    CellInfo& info = cells[GetCellIndex(cellIndex)];
    PlayerState& currentPlayer = playerStates[currentPlayerIndex];

    if (info.owner == currentPlayerIndex && info.isMortgaged) {
        int unmortgagePrice = (info.price / 2) * 1.1;

        if (currentPlayer.money >= unmortgagePrice) {
            info.isMortgaged = false;
            currentPlayer.money -= unmortgagePrice;

            currentPlayer.mortgagedProperties.removeAll(GetCellIndex(cellIndex));

            QMessageBox::information(this, "Выкуп собственности",
                                     QString("Вы выкупили %1 за %2₼").arg(info.name).arg(unmortgagePrice));

            updatePlayerInfoDisplay();
        } else {
            QMessageBox::warning(this, "Недостаточно средств",
                                 QString("У вас недостаточно денег для выкупа %1").arg(info.name));
        }
    }
}

void GameWindow::updatePlayerInfoDisplay()
{
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
            {"Банковская ошибка в вашу пользу. Получите 200₼"},
            {"Оплатите счет за лечение - 50₼"},
            {"Вы выиграли в лотерею. Получите 100₼"},
            {"С днем рождения! Получите 50₼ от каждого игрока"},
            {"Налог на имущество. Заплатите 25₼ за каждый дом"}
        };
}


void GameWindow::showHouseManagementDialog(int cellIndex)
{
    if (cellIndex < 0 || cellIndex >= path.size()) {
        QMessageBox::warning(this, "Ошибка", "Некорректный индекс ячейки");
        return;
    }

    int actualCellIndex = GetCellIndex(cellIndex);

    if (actualCellIndex < 0 || actualCellIndex >= cells.size()) {
        QMessageBox::warning(this, "Ошибка", "Невозможно определить тип выбранной ячейки");
        return;
    }

    CellInfo& info = cells[actualCellIndex];

    if (info.type != Property) {
        QMessageBox::information(this, "Информация",
                                 "Дома можно строить только на обычной собственности (городах).");

        if (selected_cell_index >= 0 && selected_cell_index < path.size()) {
            auto [prevRow, prevCol] = path[selected_cell_index];
            QLayoutItem *prevItem = boardLayout->itemAtPosition(prevRow, prevCol);
            if (prevItem && prevItem->widget()) {
                prevItem->widget()->setStyleSheet("");
            }
            selected_cell_index = -1;
        }

        return;
    }

    PlayerState& currentPlayer = playerStates[currentPlayerIndex];

    bool canBuild = false;
    bool canSell = false;

    bool hasMonopoly = playerOwnsAllPropertiesInGroup(currentPlayerIndex, info.colorGroup);

    if (info.owner == currentPlayerIndex && !info.isMortgaged && hasMonopoly) {
        canBuild = (info.houseCount < 5) && (currentPlayer.money >= info.housePrice);
        canSell = (info.houseCount > 0);
    }

    if (info.owner != currentPlayerIndex) {
        QMessageBox::information(this, "Чужая собственность",
                                 "Вы не можете строить дома на чужой собственности.");

        if (selected_cell_index >= 0 && selected_cell_index < path.size()) {
            auto [prevRow, prevCol] = path[selected_cell_index];
            QLayoutItem *prevItem = boardLayout->itemAtPosition(prevRow, prevCol);
            if (prevItem && prevItem->widget()) {
                prevItem->widget()->setStyleSheet("");
            }
            selected_cell_index = -1;
        }

        return;
    }

    if (!hasMonopoly) {
        QMessageBox::information(this, "Нет монополии",
                                 "Для строительства домов вы должны владеть всеми собственностями этого цвета.");

        if (selected_cell_index >= 0 && selected_cell_index < path.size()) {
            auto [prevRow, prevCol] = path[selected_cell_index];
            QLayoutItem *prevItem = boardLayout->itemAtPosition(prevRow, prevCol);
            if (prevItem && prevItem->widget()) {
                prevItem->widget()->setStyleSheet("");
            }
            selected_cell_index = -1;
        }

        return;
    }

    if (info.isMortgaged) {
        QMessageBox::information(this, "Заложенная собственность",
                                 "Нельзя строить дома на заложенной собственности.");

        if (selected_cell_index >= 0 && selected_cell_index < path.size()) {
            auto [prevRow, prevCol] = path[selected_cell_index];
            QLayoutItem *prevItem = boardLayout->itemAtPosition(prevRow, prevCol);
            if (prevItem && prevItem->widget()) {
                prevItem->widget()->setStyleSheet("");
            }
            selected_cell_index = -1;
        }

        return;
    }

    QDialog dialog(this);
    dialog.setWindowTitle("Управление домами");
    dialog.setMinimumWidth(350);

    QVBoxLayout *layout = new QVBoxLayout(&dialog);

    QFrame *infoFrame = new QFrame(&dialog);
    infoFrame->setFrameShape(QFrame::Box);
    infoFrame->setLineWidth(1);

    QVBoxLayout *infoLayout = new QVBoxLayout(infoFrame);

    QLabel *nameLabel = new QLabel(QString("<h3>%1</h3>").arg(info.name));
    nameLabel->setAlignment(Qt::AlignCenter);
    infoLayout->addWidget(nameLabel);

    QLabel *colorLabel = new QLabel(QString("Цветовая группа: %1").arg(info.colorGroup));
    infoLayout->addWidget(colorLabel);

    int currentRent = calculateRent(cellIndex);
    QLabel *rentLabel = new QLabel(QString("Текущая рента: %1₼").arg(currentRent));
    infoLayout->addWidget(rentLabel);

    QString houseText;
    if (info.houseCount == 5) {
        houseText = "Отель";
    } else {
        houseText = QString("%1 дом(а)").arg(info.houseCount);
    }

    QLabel *houseLabel = new QLabel(QString("Текущие строения: %1").arg(houseText));
    infoLayout->addWidget(houseLabel);

    QLabel *housePriceLabel = new QLabel(QString("Стоимость строительства дома: %1₼").arg(info.housePrice));
    infoLayout->addWidget(housePriceLabel);

    QLabel *houseSellLabel = new QLabel(QString("Стоимость продажи дома: %1₼").arg(info.housePrice / 2));
    infoLayout->addWidget(houseSellLabel);

    layout->addWidget(infoFrame);

    QGroupBox *rentBox = new QGroupBox("Таблица рент", &dialog);
    QVBoxLayout *rentLayout = new QVBoxLayout(rentBox);

    QLabel *rentTableLabel = new QLabel();
    rentTableLabel->setText(QString(
                                "Базовая рента: %1₼<br>"
                                "С 1 домом: %2₼<br>"
                                "С 2 домами: %3₼<br>"
                                "С 3 домами: %4₼<br>"
                                "С 4 домами: %5₼<br>"
                                "С отелем: %6₼"
                                ).arg(info.rent * (hasMonopoly ? 2 : 1))
                                .arg(info.rent * 5)
                                .arg(info.rent * 15)
                                .arg(info.rent * 45)
                                .arg(info.rent * 80)
                                .arg(info.rent * 125));

    rentLayout->addWidget(rentTableLabel);
    layout->addWidget(rentBox);

    QHBoxLayout *buttonLayout = new QHBoxLayout();

    QPushButton *buildButton = new QPushButton("Построить дом");
    buildButton->setEnabled(canBuild);

    QPushButton *sellButton = new QPushButton("Продать дом");
    sellButton->setEnabled(canSell);

    QPushButton *closeButton = new QPushButton("Закрыть");

    buttonLayout->addWidget(buildButton);
    buttonLayout->addWidget(sellButton);
    buttonLayout->addWidget(closeButton);

    layout->addLayout(buttonLayout);

    connect(buildButton, &QPushButton::clicked, this, [this, cellIndex, &dialog]() {
        buildHouse(cellIndex);
        dialog.accept();

        if (selected_cell_index >= 0 && selected_cell_index < path.size()) {
            auto [prevRow, prevCol] = path[selected_cell_index];
            QLayoutItem *prevItem = boardLayout->itemAtPosition(prevRow, prevCol);
            if (prevItem && prevItem->widget()) {
                prevItem->widget()->setStyleSheet("");
            }
            selected_cell_index = -1;
        }
    });

    connect(sellButton, &QPushButton::clicked, this, [this, cellIndex, &dialog]() {
        sellHouse(cellIndex);
        dialog.accept();

        if (selected_cell_index >= 0 && selected_cell_index < path.size()) {
            auto [prevRow, prevCol] = path[selected_cell_index];
            QLayoutItem *prevItem = boardLayout->itemAtPosition(prevRow, prevCol);
            if (prevItem && prevItem->widget()) {
                prevItem->widget()->setStyleSheet("");
            }
            selected_cell_index = -1;
        }
    });

    connect(closeButton, &QPushButton::clicked, this, [this, &dialog]() {
        dialog.reject();

        if (selected_cell_index >= 0 && selected_cell_index < path.size()) {
            auto [prevRow, prevCol] = path[selected_cell_index];
            QLayoutItem *prevItem = boardLayout->itemAtPosition(prevRow, prevCol);
            if (prevItem && prevItem->widget()) {
                prevItem->widget()->setStyleSheet("");
            }
            selected_cell_index = -1;
        }
    });

    dialog.setWindowFlags(dialog.windowFlags() & ~Qt::WindowContextHelpButtonHint);

    dialog.exec();
}

void GameWindow::buildHouse(int cellIndex)
{
    int actualCellIndex = GetCellIndex(cellIndex);
    CellInfo& info = cells[actualCellIndex];
    PlayerState& currentPlayer = playerStates[currentPlayerIndex];

    if (info.type != Property) {
        QMessageBox::warning(this, "Неподходящая ячейка",
                             "Дома можно строить только на обычной собственности (городах)");
        return;
    }

    if (info.owner != currentPlayerIndex) {
        QMessageBox::warning(this, "Чужая собственность",
                             "Вы не можете строить дома на чужой собственности");
        return;
    }

    if (info.isMortgaged) {
        QMessageBox::warning(this, "Заложенная собственность",
                             "Нельзя строить дома на заложенной собственности");
        return;
    }

    if (!playerOwnsAllPropertiesInGroup(currentPlayerIndex, info.colorGroup)) {
        QMessageBox::warning(this, "Нет монополии",
                             "Для строительства домов вы должны владеть всеми собственностями этого цвета");
        return;
    }

    if (info.houseCount >= 5) {
        QMessageBox::warning(this, "Максимум строений",
                             "На этой собственности уже построен отель");
        return;
    }

    if (currentPlayer.money < info.housePrice) {
        QMessageBox::warning(this, "Недостаточно средств",
                             QString("У вас недостаточно денег для строительства дома на %1").arg(info.name));
        return;
    }

    currentPlayer.money -= info.housePrice;

    auto [row, col] = path[cellIndex];
    QLayoutItem *item = boardLayout->itemAtPosition(row, col);
    CellWidget *cellWidget = nullptr;

    if (item && item->widget()) {
        cellWidget = qobject_cast<CellWidget*>(item->widget());
        if (cellWidget) {
            QHBoxLayout *houseLayout = cellWidget->getHouseLayout();
            QLayoutItem *child;
            while((child = houseLayout->takeAt(0)) != nullptr) {
                if (child->widget()) {
                    child->widget()->deleteLater();
                }
                delete child;
            }
        }
    }

    info.houseCount++;
    if (info.houseCount > 5) {
        info.houseCount = 5;
    }

    if (cellWidget) {
        cellWidget->mutableCellInfo().houseCount = info.houseCount;

        if (info.houseCount == 5) {
            QLabel *hotel = new QLabel(cellWidget);
            QPixmap hotelPixmap("../../resources/colour5.png");
            hotel->setPixmap(hotelPixmap.scaled(20, 15, Qt::KeepAspectRatio, Qt::SmoothTransformation));
            hotel->setFixedSize(20, 15);
            cellWidget->getHouseLayout()->addWidget(hotel);

            QMessageBox::information(this, "Строительство отеля",
                                     QString("Вы построили отель на %1 за %2₼").arg(info.name).arg(info.housePrice));
        } else {
            for (int i = 0; i < info.houseCount; i++) {
                QLabel *house = new QLabel(cellWidget);
                QPixmap housePixmap("../../resources/colour7.png");
                house->setPixmap(housePixmap.scaled(15, 15, Qt::KeepAspectRatio, Qt::SmoothTransformation));
                house->setFixedSize(15, 15);
                cellWidget->getHouseLayout()->addWidget(house);
            }

            QMessageBox::information(this, "Строительство дома",
                                     QString("Вы построили дом #%1 на %2 за %3₼")
                                         .arg(info.houseCount)
                                         .arg(info.name)
                                         .arg(info.housePrice));
        }

        cellWidget->getHouseLayout()->update();
        cellWidget->update();
        cellWidget->updateGeometry();
    }

    if (info.houseCount == 5) {
        currentPlayer.houseCount -= 4;
        currentPlayer.hotelCount++;
    } else {
        currentPlayer.houseCount++;
    }

    updatePlayerInfoDisplay();
}

void GameWindow::sellHouse(int cellIndex)
{
    int actualCellIndex = GetCellIndex(cellIndex);
    CellInfo& info = cells[actualCellIndex];
    PlayerState& currentPlayer = playerStates[currentPlayerIndex];

    if (info.type != Property) {
        QMessageBox::warning(this, "Неподходящая ячейка",
                             "Дома можно продавать только на обычной собственности (городах)");
        return;
    }

    if (info.owner != currentPlayerIndex) {
        QMessageBox::warning(this, "Чужая собственность",
                             "Вы не можете продавать дома с чужой собственности");
        return;
    }

    if (info.houseCount <= 0) {
        QMessageBox::warning(this, "Нет домов",
                             "На этой собственности нет домов для продажи");
        return;
    }

    int previousHouseCount = info.houseCount;

    int salePrice = info.housePrice / 2;

    info.houseCount--;
    if (info.houseCount < 0) info.houseCount = 0;

    auto [row, col] = path[cellIndex];
    QLayoutItem *item = boardLayout->itemAtPosition(row, col);
    CellWidget *cellWidget = nullptr;

    if (item && item->widget()) {
        cellWidget = qobject_cast<CellWidget*>(item->widget());
        if (cellWidget) {
            QHBoxLayout *houseLayout = cellWidget->getHouseLayout();
            QLayoutItem *child;
            while((child = houseLayout->takeAt(0)) != nullptr) {
                if (child->widget()) {
                    child->widget()->deleteLater();
                }
                delete child;
            }

            cellWidget->mutableCellInfo().houseCount = info.houseCount;
        }
    }

    if (cellWidget) {
        if (info.houseCount == 0) {}
        else if (info.houseCount == 4 && previousHouseCount == 5) {
            for (int i = 0; i < 4; i++) {
                QLabel *house = new QLabel(cellWidget);
                QPixmap housePixmap("../../resources/colour7.png");
                house->setPixmap(housePixmap.scaled(15, 15, Qt::KeepAspectRatio, Qt::SmoothTransformation));
                house->setFixedSize(15, 15);
                cellWidget->getHouseLayout()->addWidget(house);
            }
        }
        else {
            for (int i = 0; i < info.houseCount; i++) {
                QLabel *house = new QLabel(cellWidget);
                QPixmap housePixmap("../../resources/colour7.png");
                house->setPixmap(housePixmap.scaled(15, 15, Qt::KeepAspectRatio, Qt::SmoothTransformation));
                house->setFixedSize(15, 15);
                cellWidget->getHouseLayout()->addWidget(house);
            }
        }

        cellWidget->getHouseLayout()->update();
        cellWidget->update();
        cellWidget->updateGeometry();
    }

    currentPlayer.money += salePrice;

    if (previousHouseCount == 5) {
        currentPlayer.hotelCount--;
        currentPlayer.houseCount += 4;
        QMessageBox::information(this, "Продажа отеля",
                                 QString("Вы продали отель на %1 и получили %2₼").arg(info.name).arg(salePrice));
    } else {
        currentPlayer.houseCount--;
        QMessageBox::information(this, "Продажа дома",
                                 QString("Вы продали дом на %1 и получили %2₼").arg(info.name).arg(salePrice));
    }

    updatePlayerInfoDisplay();
}


void GameWindow::on_build_houses_clicked()
{
    if (selected_cell_index < 0 || selected_cell_index >= path.size()) {
        QMessageBox::warning(this, "Выберите собственность",
                             "Сначала выберите собственность, на которой хотите строить дом");
        return;
    }

    int actualCellIndex = GetCellIndex(selected_cell_index);

    if (actualCellIndex < 0 || actualCellIndex >= cells.size()) {
        QMessageBox::warning(this, "Ошибка",
                             "Невозможно определить тип выбранной ячейки");
        return;
    }

    CellInfo& info = cells[actualCellIndex];

    if (info.type != Property) {
        QMessageBox::information(this, "Информация",
                                 "Дома можно строить только на обычной собственности (городах).");

        if (selected_cell_index >= 0 && selected_cell_index < path.size()) {
            auto [prevRow, prevCol] = path[selected_cell_index];
            QLayoutItem *prevItem = boardLayout->itemAtPosition(prevRow, prevCol);
            if (prevItem && prevItem->widget()) {
                prevItem->widget()->setStyleSheet("");
            }
            selected_cell_index = -1;
        }

        return;
    }

    if (info.owner != currentPlayerIndex) {
        QMessageBox::information(this, "Чужая собственность",
                                 "Вы не можете строить дома на чужой собственности.");

        if (selected_cell_index >= 0 && selected_cell_index < path.size()) {
            auto [prevRow, prevCol] = path[selected_cell_index];
            QLayoutItem *prevItem = boardLayout->itemAtPosition(prevRow, prevCol);
            if (prevItem && prevItem->widget()) {
                prevItem->widget()->setStyleSheet("");
            }
            selected_cell_index = -1;

        return;
    }

    if (info.isMortgaged) {
        QMessageBox::information(this, "Заложенная собственность",
                                 "Нельзя строить дома на заложенной собственности.");

        if (selected_cell_index >= 0 && selected_cell_index < path.size()) {
            auto [prevRow, prevCol] = path[selected_cell_index];
            QLayoutItem *prevItem = boardLayout->itemAtPosition(prevRow, prevCol);
            if (prevItem && prevItem->widget()) {
                prevItem->widget()->setStyleSheet("");
            }
            selected_cell_index = -1;
        }

        return;
    }

    if (!playerOwnsAllPropertiesInGroup(currentPlayerIndex, info.colorGroup)) {
        QMessageBox::information(this, "Нет монополии",
                                 "Для строительства домов вы должны владеть всеми собственностями этого цвета.");

        if (selected_cell_index >= 0 && selected_cell_index < path.size()) {
            auto [prevRow, prevCol] = path[selected_cell_index];
            QLayoutItem *prevItem = boardLayout->itemAtPosition(prevRow, prevCol);
            if (prevItem && prevItem->widget()) {
                prevItem->widget()->setStyleSheet("");
            }
            selected_cell_index = -1;
        }

        return;
    }

    showHouseManagementDialog(selected_cell_index);
}
}

void GameWindow::select_cell(int index)
{
    if (selected_cell_index >= 0 && selected_cell_index < path.size())
    {
        auto [prevRow, prevCol] = path[selected_cell_index];
        QLayoutItem *prevItem = boardLayout->itemAtPosition(prevRow, prevCol);

        if (prevItem && prevItem->widget())
        {
            prevItem->widget()->setStyleSheet("");
        }
    }

    selected_cell_index = index;

    if (index >= 0 && index < path.size())
    {
        auto [row, col] = path[index];
        QLayoutItem *item = boardLayout->itemAtPosition(row, col);

        if (item && item->widget())
        {
            item->widget()->setStyleSheet("border: 3px solid red;");
  //          qDebug() << "Выбрана клетка" << index << "с координатами" << row << col;

            int actualCellIndex = GetCellIndex(index);
            if (actualCellIndex >= 0 && actualCellIndex < cells.size()) {
                CellInfo& info = cells[actualCellIndex];
          //      qDebug() << "Тип клетки:" << info.type << "Имя:" << info.name;
            }
        }
    }
}
