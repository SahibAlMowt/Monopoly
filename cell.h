#ifndef CELLWIDGET_H
#define CELLWIDGET_H

#include <QLabel>
#include <QFrame>
#include <QHBoxLayout>

enum CellTypeO
{
    Property,
    Railroad,
    Treasury,
    Chance,
    Tax,
    Corner,
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
    Money,
    Movement,
    JailRelated,
    PropertyRelated,
    SkipTurn
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

    CellInfo &mutableCellInfo()
    {
        return cell_info;
    }

    QHBoxLayout* getHouseLayout() {
        return houseLayout;
    }

    void build_house();
    void remove_house();

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
