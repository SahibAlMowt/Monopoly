#ifndef PLAYER_H
#define PLAYER_H

#include <QLabel>
#include <QString>
#include <QWidget>

class Player : public QLabel
{
    Q_OBJECT

public:
    Player(const QString &name, const QColor &color, QWidget* parent = nullptr);

    void moveToCell(int index);

    int currentIndex() const;

private:
    int m_currentIndex;
    QVector<QPair<int, int>> m_cellPositions;
};

#endif // PLAYER_H
