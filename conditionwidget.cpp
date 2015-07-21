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

QVector<NPCEditorHolder::ExistCondition> ConditionWidget::conditions() const
{
    QVector < NPCEditorHolder::ExistCondition > result;

    for (int row = 0; row < ui->tableWidget->rowCount(); row++)
    {
        NPCEditorHolder::ExistCondition condition;

        QComboBox *comboWidget = (QComboBox *) ui->tableWidget->cellWidget( row, 0 );
        switch( comboWidget->currentIndex() )
        {
        case 0:
            condition.type = NPCEditorHolder::ExistCondition::Type_Complete;
            break;
        case 1:
            condition.type = NPCEditorHolder::ExistCondition::Type_Have;
            break;
        default:
            condition.type = NPCEditorHolder::ExistCondition::Type_None;
            break;
        }

        condition.conditionId = ui->tableWidget->item( row, 1 )->text().toInt();

        result.push_back( condition );
    }

    return result;
}

void ConditionWidget::setConditions(const std::vector<NPCEditorHolder::ExistCondition> &conditions)
{
    ui->tableWidget->setRowCount(0);
    for (std::vector < NPCEditorHolder::ExistCondition >::const_iterator iterator = conditions.begin();
         iterator != conditions.end();
         iterator++)
    {
        unsigned int rowIndex = ui->tableWidget->rowCount();

        ui->tableWidget->insertRow( rowIndex );

        QComboBox *selectBox = new QComboBox( ui->tableWidget );
        selectBox->addItem("complete");
        selectBox->addItem("have");

        switch (iterator->type)
        {
        case NPCEditorHolder::ExistCondition::Type_Complete:
            selectBox->setCurrentIndex( 0 );
            break;
        case NPCEditorHolder::ExistCondition::Type_Have:
            selectBox->setCurrentIndex( 1 );
            break;
        default:
            break;
        }

        QTableWidgetItem *idItem = new QTableWidgetItem();
        idItem->setText( QString::number ( iterator->conditionId ) );

        ui->tableWidget->setCellWidget( rowIndex, 0, selectBox );
        ui->tableWidget->setItem( rowIndex, 1, idItem );

        ui->tableWidget->setRowHeight( rowIndex, 20 );
    }
}


void ConditionWidget::on_add_button_clicked()
{
    unsigned int rowIndex = ui->tableWidget->rowCount();

    ui->tableWidget->insertRow( rowIndex );

    QComboBox *selectBox = new QComboBox( ui->tableWidget );
    selectBox->addItem("complete");
    selectBox->addItem("have");

    QTableWidgetItem *idItem = new QTableWidgetItem();
    idItem->setText(0);

    ui->tableWidget->setCellWidget( rowIndex, 0, selectBox );
    ui->tableWidget->setItem( rowIndex, 1, idItem );

    ui->tableWidget->setRowHeight( rowIndex, 20 );
}

void ConditionWidget::on_remove_button_clicked()
{
    unsigned int rowIndex = ui->tableWidget->currentRow();

    ui->tableWidget->removeRow( rowIndex );
}
