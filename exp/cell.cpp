#include "cell.h"
#include <QVBoxLayout>

CellWidget::CellWidget(const QString &name, QWidget *parent): QFrame(parent)
{
    setFrameShape(QFrame::Box);
    setLineWidth(2);
    setFixedSize(80, 80);

    QLabel *label = new QLabel(name);
    label->setAlignment(Qt::AlignCenter);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(label);
}
