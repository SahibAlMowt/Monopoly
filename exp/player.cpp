#include "player.h"
#include <QPropertyAnimation>
#include <QGridLayout>

#include <QPoint>
#include <QScreen>
#include <QGuiApplication>



Player::Player(const QString &name, const QColor &color, QWidget* parent): QLabel(name, parent), m_currentIndex(0)
{
    setAlignment(Qt::AlignCenter);
    setStyleSheet(QString("background-color: %1; color: white; border-radius: 15px;").arg(color.name()));
    setFixedSize(40, 40);

    m_cellPositions =
    {
        {0, 0}, {0, 1}, {0, 2}, {0, 3}, {0, 4}, {0, 5}, {0, 6}, {0, 7}, {0, 8}, {0, 9},
        {1, 9}, {2, 9}, {3, 9}, {4, 9}, {5, 9}, {6, 9}, {7, 9}, {8, 9}, {9, 9}, {9, 8},
        {9, 7}, {9, 6}, {9, 5}, {9, 4}, {9, 3}, {9, 2}, {9, 1}, {8, 1}, {7, 1}, {6, 1},
        {5, 1}, {4, 1}, {3, 1}, {2, 1}, {1, 1}, {1, 2}, {1, 3}, {1, 4}, {1, 5}, {1, 6},
        {1, 7}, {1, 8}
    };

    moveToCell(m_currentIndex);
}


void Player::moveToCell(int index)
{
    if (index < 0 || index >= m_cellPositions.size())
        return;

    m_currentIndex = index;

    QWidget *parent = parentWidget();
    if (!parent)
        return;

    QGridLayout *layout = qobject_cast<QGridLayout*>(parent->layout());
    if (!layout)
        return;

    auto [row, col] = m_cellPositions[index];

    QScreen *screen = QGuiApplication::primaryScreen();
    QRect screenGeometry = screen->geometry();

    int screenHeight = screenGeometry.height();

    int baseSize = screenHeight / 12;

    QPoint currentPos = this->pos();


    QPoint targetPos = layout->geometry().topLeft() + QPoint(col * baseSize, row * baseSize);

    QPropertyAnimation *animation = new QPropertyAnimation(this, "pos");
    animation->setDuration(500); // продолжительность анимации (в миллисекундах)
    animation->setStartValue(currentPos);
    animation->setEndValue(targetPos);
    animation->start(QAbstractAnimation::DeleteWhenStopped);

    layout->removeWidget(this);
    layout->addWidget(this, row, col, Qt::AlignCenter);
}



int Player::currentIndex() const
{
    return m_currentIndex;
}
