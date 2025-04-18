#ifndef CELLWIDGET_H
#define CELLWIDGET_H

#include <QLabel>
#include <QFrame>

struct CellInfo
{
    QString name;
    int price;
    QString imagePath;
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

private:

    QLabel *imageLabel;
    QLabel *nameLabel;
    QLabel *priceLabel;

};

#endif // CELLWIDGET_H
