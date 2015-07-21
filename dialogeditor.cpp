#include "dialogeditor.h"
#include "ui_dialogeditor.h"

DialogEditor::DialogEditor(QWidget *parent) :
    QDialog(parent),
    _ui(new Ui::DialogEditor)
{
    _ui->setupUi(this);
    _npcEditor = nullptr;
    _currentOpenedDialog = nullptr;
    _currentOpenedNode = nullptr;

    _ui->node_table_widget->setColumnWidth(0, 50);
    _ui->node_table_widget->setColumnWidth(1, 300);
    _ui->node_table_widget->setColumnWidth(4, 250);
}

DialogEditor::~DialogEditor()
{
    delete _ui;
}

void DialogEditor::setNpcEditor(NPCEditorHolder *npcEditor)
{
    _npcEditor = npcEditor;
}

NPCEditorHolder *DialogEditor::npcEditorHolder() const
{
    return _npcEditor;
}

void DialogEditor::updateRootListWidget()
{
    _ui->root_list_widget->clear();
    for (unsigned int index = 0;
         index < _npcEditor->dialogLength();
         index++)
    {
        NPCEditorHolder::DialogHolder *dialog = _npcEditor->getDialog(index);

        _ui->root_list_widget->addItem(
            QString::number(dialog->id) + " " + QString(dialog->name.c_str())
        );
    }
}

void DialogEditor::updateNodeListWidget()
{
    if (!_currentOpenedDialog)
        return;

    NPCEditorHolder::DialogNode *startNode = _npcEditor->getNode( _currentOpenedDialog->id, _currentOpenedDialog->startId );

    _ui->node_list_widget->clear();
    _ui->start_node_combo->clear();

    unsigned int startNodeIndex = 0;
    for (unsigned int i=0; i < _npcEditor->rootLength( _currentOpenedDialog->id ); i++)
    {
        NPCEditorHolder::DialogNode *node = _npcEditor->getNodeByIndex( _currentOpenedDialog->id, i );

        QString text(node->text.c_str());

        text.truncate( 20 );

        text = QString::number( node->id ) + tr(" ") + text;

        _ui->node_list_widget->addItem( text );
        _ui->start_node_combo->addItem( text );

        if (startNode)
            if (node->id == startNode->id)
                startNodeIndex = i;
    }
    // Назначаем стартовую ноду
    _ui->start_node_combo->setCurrentIndex( startNodeIndex );
}

void DialogEditor::clear()
{
    _ui->node_id_edit->clear();
    _ui->node_list_widget->clear();
    _ui->node_table_widget->setRowCount(0);
    _ui->root_list_widget->clear();
    _ui->name_edit->clear();
    _ui->id_edit->clear();

    _currentOpenedDialog = nullptr;
    _currentOpenedNode = nullptr;
}

void DialogEditor::loadDialog(unsigned int index)
{
    if (index >= _npcEditor->dialogLength())
    {
        printf("[DialogEditor::loadDialog] Error: Can't get %d dialog. Out of size.\n",
               index);
        return;
    }

    NPCEditorHolder::DialogHolder *dialog = _npcEditor->getDialog(index);

    loadDialog( dialog );
}

void DialogEditor::loadDialog(NPCEditorHolder::DialogHolder *dialog)
{
    saveCurrentDialog();

    // Очистка диалоговых форм
    _ui->start_node_combo->clear();
    _ui->name_edit->clear();
    _ui->id_edit->clear();
    _ui->node_list_widget->clear();

    // Очистка форм нод
    _ui->node_id_edit->clear();
    _ui->node_list_widget->clear();
    _ui->node_text_edit->clear();

    _currentOpenedNode = nullptr;

    // Назначаем этот диалог как выбранный
    _currentOpenedDialog = dialog;

    _ui->name_edit->setText( QString(dialog->name.c_str()) );
    _ui->id_edit->setText( QString::number( dialog->id ) );

    // Загрузка данных о всех нодах и о начальной
    updateNodeListWidget();
}

void DialogEditor::loadNode(unsigned int dialogIndex, unsigned int nodeIndex)
{
    saveCurrentNode();

    NPCEditorHolder::DialogNode *dialogNode = _npcEditor->getNodeByIndex(dialogIndex, nodeIndex );

    loadNode ( dialogNode );
}

void DialogEditor::loadNode(NPCEditorHolder::DialogNode *dialogNode)
{
    saveCurrentNode();

    _currentOpenedNode = dialogNode;

    // Очищаем формы
    _ui->node_id_edit->clear();
    _ui->node_text_edit->clear();

    // Заполняем формы
    _ui->node_id_edit->setText( QString::number( dialogNode->id ));
    _ui->node_text_edit->setPlainText( QString( dialogNode->text.c_str() ) );

    loadNodeAnswers( dialogNode->answers );
}

void DialogEditor::loadNodeAnswers(const std::vector<NPCEditorHolder::DialogAnswer> &answers)
{
    _ui->node_table_widget->setRowCount(0);
    for (std::vector < NPCEditorHolder::DialogAnswer >::const_iterator iterator = answers.begin();
         iterator != answers.end();
         iterator++)
    {
        QTableWidgetItem *idItem = new QTableWidgetItem( QString::number ( iterator->id ) );
        QTableWidgetItem *textItem = new QTableWidgetItem();
        QTableWidgetItem *comboSelectorItem = new QTableWidgetItem();
        QTableWidgetItem *gotoItem = new QTableWidgetItem();

        int rowIndex = _ui->node_table_widget->rowCount();
        _ui->node_table_widget->insertRow( rowIndex );

        gotoItem->setText( QString::number ( iterator->destinationId ) );

        // Заполняем QTableWidgetItem
        _ui->node_table_widget->setItem( rowIndex, 0, idItem );
        _ui->node_table_widget->setItem( rowIndex, 1, textItem );
        _ui->node_table_widget->setItem( rowIndex, 2, comboSelectorItem );
        _ui->node_table_widget->setItem( rowIndex, 3, gotoItem );

        // Создаем кастомные виджеты
        QTextEdit *textWidget = new QTextEdit( _ui->node_table_widget );
        QComboBox *typeWidget = new QComboBox( _ui->node_table_widget );
        ConditionWidget *conditionWidget = new ConditionWidget( _ui->node_table_widget );

        conditionWidget->setConditions( iterator->conditions );

        typeWidget->addItem("none");
        typeWidget->addItem("shop");

        switch (iterator->answerType)
        {
        case NPCEditorHolder::DialogAnswer::Type_Blank:
            typeWidget->setCurrentIndex(0);
            break;
        case NPCEditorHolder::DialogAnswer::Type_Shop:
            typeWidget->setCurrentIndex(1);
            break;
        default:
            typeWidget->setCurrentIndex(0);
            break;
        }

        textWidget->setText( iterator->text.c_str() );

        _ui->node_table_widget->setCellWidget( rowIndex, 1, textWidget );
        _ui->node_table_widget->setCellWidget( rowIndex, 2, typeWidget );
        _ui->node_table_widget->setCellWidget( rowIndex, 4, conditionWidget );

        _ui->node_table_widget->setRowHeight( rowIndex, 120 );
    }
}

void DialogEditor::saveNodeAnswers(std::vector<NPCEditorHolder::DialogAnswer> *answers)
{
    answers->clear();
    for (unsigned int rowIndex = 0; rowIndex < _ui->node_table_widget->rowCount(); rowIndex++)
    {
        NPCEditorHolder::DialogAnswer answer;

        // ID
        QTableWidgetItem *item = _ui->node_table_widget->item( rowIndex, 0);
        answer.id = item->text().toInt();

        // Text
        QTextEdit *textEdit = (QTextEdit *) _ui->node_table_widget->cellWidget( rowIndex, 1 );
        answer.text = textEdit->toPlainText().toStdString();

        // Type
        QComboBox *typeBox = (QComboBox *) _ui->node_table_widget->cellWidget( rowIndex, 2 );
        switch (typeBox->currentIndex())
        {
        case 0:
            answer.answerType = NPCEditorHolder::DialogAnswer::Type_Blank;
            break;
        case 1:
            answer.answerType = NPCEditorHolder::DialogAnswer::Type_Shop;
            break;
        default:
            qDebug() << "[DialogEditor::saveNodeAnswers] Error: Can't identity answer type for index " << rowIndex;
            answer.answerType = NPCEditorHolder::DialogAnswer::Type_Blank;
            break;
        }

        // Goto
        if (_ui->node_table_widget->item( rowIndex, 3 )->text().length() > 0)
            answer.destinationId = _ui->node_table_widget->item( rowIndex, 3 )->text().toInt();
        else
            answer.destinationId = 0;

        // Conditions
        ConditionWidget *conditionWidget = (ConditionWidget *) _ui->node_table_widget->cellWidget( rowIndex, 4 );
        QVector < NPCEditorHolder::ExistCondition > conditions = conditionWidget->conditions();
        answer.conditions = conditions.toStdVector();

        answers->push_back( answer );
    }
}

void DialogEditor::saveCurrentNode()
{
    if (!_currentOpenedNode)
        return;

    _currentOpenedNode->text = _ui->node_text_edit->toPlainText().toStdString();

    saveNodeAnswers( &_currentOpenedNode->answers );

    // Обновление в listWidget
    int currentNodeIndex = _npcEditor->getNodeIndex( _currentOpenedNode );

    _ui->node_list_widget->item( currentNodeIndex )->setText(
        QString::number(_currentOpenedNode->id) + tr(" ") + QString(_currentOpenedNode->text.c_str())
    );
}

void DialogEditor::saveCurrentDialog()
{
    saveCurrentNode();

    if (!_currentOpenedDialog)
        return;

    _currentOpenedDialog->name = _ui->name_edit->text().toStdString();

    if (_ui->start_node_combo->currentIndex() > -1)
    {
        NPCEditorHolder::DialogNode *node = _npcEditor->getNodeByIndex(
            _currentOpenedDialog->id,
            _ui->start_node_combo->currentIndex()
        );

        _currentOpenedDialog->startId = node->id;
    }

    // Обновление данных в list widget
    int dialogIndex = _npcEditor->getDialogIndex( _currentOpenedDialog );
    _ui->root_list_widget->item( dialogIndex )->setText(
        QString::number(_currentOpenedDialog->id) + " " + QString(_currentOpenedDialog->name.c_str())
    );

    emit dialogsUpdated();
}

void DialogEditor::on_root_list_widget_activated(const QModelIndex &index)
{
    loadDialog( index.row() );
}

void DialogEditor::on_node_list_widget_activated(const QModelIndex &index)
{
    if (_currentOpenedDialog)
        loadNode( _currentOpenedDialog->id, index.row());
}

void DialogEditor::on_new_button_clicked()
{
    NPCEditorHolder::DialogHolder *newDialog = _npcEditor->createDialog();

    loadDialog( newDialog );

    updateRootListWidget();
}

void DialogEditor::on_delete_button_clicked()
{
    if (!_currentOpenedDialog)
        return;

    _npcEditor->removeDialog( _currentOpenedDialog->id );

    _currentOpenedDialog = nullptr;

    updateRootListWidget();
}

void DialogEditor::on_add_node_button_clicked()
{
    if (!_currentOpenedDialog)
        return;

    NPCEditorHolder::DialogNode *newNode = _npcEditor->createNode( _currentOpenedDialog->id );

    loadNode(newNode);

    updateNodeListWidget();
}

void DialogEditor::on_delete_node_button_clicked()
{
    if (!_currentOpenedNode)
        return;

    _npcEditor->removeNode( _currentOpenedNode->dialogId, _currentOpenedNode->id );

    _currentOpenedNode = nullptr;

    updateNodeListWidget();
}

void DialogEditor::on_answer_add_button_clicked()
{
    if (!_currentOpenedNode)
        return;

    _npcEditor->addAnswer( _currentOpenedNode );

    loadNodeAnswers( _currentOpenedNode->answers );
}

void DialogEditor::on_answer_delete_button_clicked()
{
    if (!_currentOpenedNode)
        return;

    _npcEditor->removeAnswer( _currentOpenedNode, _ui->node_table_widget->currentIndex().row() );

    loadNodeAnswers( _currentOpenedNode->answers );
}

void DialogEditor::leaveEvent(QEvent *event)
{
    saveCurrentDialog();
    QWidget::leaveEvent( event );
}
