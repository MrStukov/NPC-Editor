#ifndef CONDITIONWIDGET_H
#define CONDITIONWIDGET_H

#include <QWidget>
#include <QComboBox>
#include <QVector>
#include "npceditorholder.h"

namespace Ui {
class ConditionWidget;
}

class ConditionWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ConditionWidget(QWidget *parent = 0);
    ~ConditionWidget();

    QVector < NPCEditorHolder::ExistCondition > conditions() const;
    void setConditions( const std::vector < NPCEditorHolder::ExistCondition > &conditions );
private slots:

    void on_add_button_clicked();

    void on_remove_button_clicked();

private:
    Ui::ConditionWidget *ui;
};

#endif // CONDITIONWIDGET_H
