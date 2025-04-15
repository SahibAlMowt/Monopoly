#ifndef CELLWIDGET_H
#define CELLWIDGET_H

#include <QLabel>
#include <QFrame>

class CellWidget : public QFrame
{
    Q_OBJECT

public:

    explicit CellWidget(const QString &name, QWidget *parent = nullptr);

};

#endif // CELLWIDGET_H
