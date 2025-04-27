#include "cell.h"
#include <QVBoxLayout>
#include <QDebug>

CellWidget::CellWidget(const CellInfo &info, QWidget *parent): QFrame(parent), cell_info(info)
{
    setFrameShape(QFrame::Box);
    setLineWidth(2);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setContentsMargins(2, 2, 2, 2);

    QLabel *imageLabel = new QLabel();
    if(info.type == 5)
    {
        imageLabel -> setPixmap(QPixmap(info.imagePath).scaled(90, 60, Qt::KeepAspectRatio, Qt::SmoothTransformation));
 //       qDebug() << "Hello";
    }
    else
    {
        imageLabel -> setPixmap(QPixmap(info.imagePath).scaled(50, 30, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    }

    imageLabel -> setAlignment(Qt::AlignCenter);
    layout -> addWidget(imageLabel);

    QLabel *nameLabel = new QLabel(info.name);
    nameLabel -> setAlignment(Qt::AlignCenter);
    layout -> addWidget(nameLabel);

    if(info.price != 0)
    {
        QLabel *priceLabel = new QLabel(QString::number(info.price) + "₼");
        priceLabel -> setAlignment(Qt::AlignCenter);
        layout -> addWidget(priceLabel);
    }

    houseLayout = new QHBoxLayout();
    houseLayout -> setAlignment(Qt::AlignCenter);
  //  layout->addStretch();
    layout -> addLayout(houseLayout);
}

void CellWidget::build_house()
{
    cell_info.houseCount++;
    if(cell_info.houseCount >= 6)
    {
        cell_info.houseCount--;
  //      qDebug() << cell_info.houseCount;
        return;
    }

    if(cell_info.houseCount == 5)
    {

        QLayoutItem *child;
        while((child = houseLayout -> takeAt(0)) != nullptr)
        {
            if (child->widget())
            {
                child->widget()->deleteLater();
            }
            delete child;
        }
        QLabel *hotel = new QLabel(this);

        QPixmap housePixmap("../../resources/colour5.png");

        hotel->setPixmap(housePixmap.scaled(20, 15, Qt::KeepAspectRatio, Qt::SmoothTransformation));
        hotel->setFixedSize(20, 15);

        houseLayout->addWidget(hotel);
    }
    else if(cell_info.houseCount <= 4)
    {
        QLabel *house = new QLabel(this);

        QPixmap housePixmap("../../resources/colour7.png");
        //  qDebug() << "Pixmap loaded: " << !housePixmap.isNull();

        house->setPixmap(housePixmap.scaled(15, 15, Qt::KeepAspectRatio, Qt::SmoothTransformation));
        house->setFixedSize(15, 15);

        houseLayout->addWidget(house);
    }

    houseLayout->update();

    this->update();
    this->updateGeometry();
}


