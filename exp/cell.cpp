#include "cell.h"
#include <QVBoxLayout>

CellWidget::CellWidget(const CellInfo &info, CellType type, QWidget *parent): QFrame(parent)
{
    setFrameShape(QFrame::Box);
    setLineWidth(2);

    switch (type)
    {
        case CellType::Corner:
            setFixedSize(100, 100);
            break;

        case CellType::Horizontal:
            setFixedSize(80, 100);
            break;

        case CellType::Vertical:
            setFixedSize(100, 80);
            break;
    }

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setContentsMargins(2, 2, 2, 2);

    QLabel *imageLabel = new QLabel();
    imageLabel -> setPixmap(QPixmap(info.imagePath).scaled(50, 30, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    imageLabel -> setAlignment(Qt::AlignCenter);
    layout -> addWidget(imageLabel);

    QLabel *nameLabel = new QLabel(info.name);
    nameLabel -> setAlignment(Qt::AlignCenter);
    layout -> addWidget(nameLabel);

    QLabel *priceLabel = new QLabel(QString::number(info.price) + "₼");
    priceLabel -> setAlignment(Qt::AlignCenter);
    layout -> addWidget(priceLabel);
}
