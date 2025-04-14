#include "game_widget.h"
#include "game.h"
#include "board.h"
#include "cell.h"
#include "player.h"
#include "ui_mainwindow.h"

#include <QPainter>

GameWidget::GameWidget(GameController* controller, QWidget *parent)
    : QWidget(parent), controller(controller) {
    setMinimumSize(400, 400);
}

void GameWidget::paintEvent(QPaintEvent*) {
    QPainter painter(this);
    int cellSize = width() / 5;
    Board* board = controller->getBoard();

    for (int i = 0; i < board->size(); ++i) {
        int row = i / 5;
        int col = i % 5;
        QRect rect(col * cellSize, row * cellSize, cellSize, cellSize);
        painter.drawRect(rect);

        QString name = board->getCell(i)->getName();
        painter.drawText(rect, Qt::AlignCenter, name);
    }

    // Отметим игроков
    const auto& players = controller->getPlayers();
    for (int i = 0; i < players.size(); ++i) {
        Player* p = players[i];
        int pos = p->getPosition();
        int row = pos / 5;
        int col = pos % 5;
        QRect rect(col * cellSize, row * cellSize, cellSize, cellSize);

        painter.setBrush(i == 0 ? Qt::blue : Qt::red);
        painter.drawEllipse(rect.topLeft() + QPoint(10 + 10 * i, 10), 10, 10);
    }
}
