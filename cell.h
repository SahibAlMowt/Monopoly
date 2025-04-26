#ifndef CELLWIDGET_H
#define CELLWIDGET_H

#include <QLabel>
#include <QFrame>
#include <QHBoxLayout>

enum CellTypeO
{
    Property,    // Покупаемое имущество (города)
    Railroad,    // Вокзал
    Treasury,    // Казна
    Chance,      // Шанс
    Tax,         // Налог
    Corner,      // Угловые клетки (Старт, Тюрьма, Бесплатная парковка, Идти в тюрьму)
};

struct CellInfo
{
    QString name;
    int price;
    QString imagePath;
    CellTypeO type;
    int owner;
    bool isMortgaged;
    int cardIndex;
    QString colorGroup;
    int rent;
    int housePrice;
    int houseCount = 0;
};


enum class CellType
{
    Corner,
    Horizontal,
    Vertical
};

enum CardType {
    Money,          // Изменение денежного баланса
    Movement,       // Перемещение игрока
    JailRelated,    // Связано с тюрьмой (попадание или освобождение)
    PropertyRelated, // Действие, связанное с собственностью
    SkipTurn        // Пропуск хода
};


class CellWidget : public QFrame
{
    Q_OBJECT

public:

    explicit CellWidget(const CellInfo &info, QWidget *parent = nullptr);

    const CellInfo &getCellInfo() const
    {
        return cell_info;
    }

    void build_house();

private:

    CellInfo cell_info;
    QLabel *imageLabel;
    QLabel *nameLabel;
    QLabel *priceLabel;

    QHBoxLayout *houseLayout;

signals:

    void clicked();

protected:

    void mousePressEvent(QMouseEvent *event) override
    {
        emit clicked();
        QFrame::mousePressEvent(event);
    }

};

#endif // CELLWIDGET_H
