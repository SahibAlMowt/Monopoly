#ifndef CELLWIDGET_H
#define CELLWIDGET_H

#include <QLabel>
#include <QFrame>

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
};

enum class CellType
{
    Corner,
    Horizontal,
    Vertical
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

private:

    CellInfo cell_info;
    QLabel *imageLabel;
    QLabel *nameLabel;
    QLabel *priceLabel;

};

#endif // CELLWIDGET_H
