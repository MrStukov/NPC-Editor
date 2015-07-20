#include "conditionwidget.h"
#include "ui_conditionwidget.h"

ConditionWidget::ConditionWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ConditionWidget)
{
    ui->setupUi(this);
}

ConditionWidget::~ConditionWidget()
{
    delete ui;
}
