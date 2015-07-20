#ifndef CONDITIONWIDGET_H
#define CONDITIONWIDGET_H

#include <QWidget>

namespace Ui {
class ConditionWidget;
}

class ConditionWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ConditionWidget(QWidget *parent = 0);
    ~ConditionWidget();

private:
    Ui::ConditionWidget *ui;
};

#endif // CONDITIONWIDGET_H
