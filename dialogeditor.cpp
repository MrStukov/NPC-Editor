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

void DialogEditor::loadDialog(unsigned int index)
{
    if (index >= _npcEditor->dialogLength())
    {
        printf("[DialogEditor::loadDialog] Error: Can't get %d dialog. Out of size.\n",
               index);
        return;
    }

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

    // Загрузка в формы данных о диалоге

    NPCEditorHolder::DialogHolder *dialog = _npcEditor->getDialog(index);

    // Назначаем этот диалог как выбранный
    _currentOpenedDialog = dialog;

    _ui->name_edit->setText( QString(dialog->name.c_str()) );
    _ui->id_edit->setText( QString::number( dialog->id ) );

    // Загрузка данных о всех нодах и о начальной
    NPCEditorHolder::DialogNode *startNode = _npcEditor->getNode( dialog->id, dialog->startId );

    unsigned int startNodeIndex = 0;
    for (unsigned int i=0; i < _npcEditor->rootLength( dialog->id ); i++)
    {
        NPCEditorHolder::DialogNode *node = _npcEditor->getNodeByIndex( dialog->id, i );

        QString text(node->text.c_str());

        text.truncate( 20 );

        text = QString::number( node->id ) + tr(" ") + text;

        _ui->node_list_widget->addItem( text );
        _ui->start_node_combo->addItem( text );

        if (node->id == startNode->id)
            startNodeIndex = i;
    }

    // Назначаем стартовую ноду
    _ui->start_node_combo->setCurrentIndex( startNodeIndex );
}

void DialogEditor::loadDialog(NPCEditorHolder::DialogHolder *dialog)
{
    if (index >= _npcEditor->dialogLength())
    {
        printf("[DialogEditor::loadDialog] Error: Can't get %d dialog. Out of size.\n",
               index);
        return;
    }

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
    NPCEditorHolder::DialogNode *startNode = _npcEditor->getNode( dialog->id, dialog->startId );

    unsigned int startNodeIndex = 0;
    for (unsigned int i=0; i < _npcEditor->rootLength( dialog->id ); i++)
    {
        NPCEditorHolder::DialogNode *node = _npcEditor->getNodeByIndex( dialog->id, i );

        QString text(node->text.c_str());

        text.truncate( 20 );

        text = QString::number( node->id ) + tr(" ") + text;

        _ui->node_list_widget->addItem( text );
        _ui->start_node_combo->addItem( text );

        if (node->id == startNode->id)
            startNodeIndex = i;
    }

    // Назначаем стартовую ноду
    _ui->start_node_combo->setCurrentIndex( startNodeIndex );
}

void DialogEditor::loadNode(unsigned int dialogIndex, unsigned int nodeIndex)
{
    saveCurrentNode();

    NPCEditorHolder::DialogNode *dialogNode = _npcEditor->getNodeByIndex(dialogIndex, nodeIndex );

    _currentOpenedNode = dialogNode;

    // Очищаем формы
    _ui->node_id_edit->clear();
    _ui->node_text_edit->clear();

    if (!dialogNode)
        return;

    // Заполняем формы
    _ui->node_id_edit->setText( QString::number( dialogNode->id ) );
    _ui->node_text_edit->setPlainText( QString( dialogNode->text.c_str() ) );
}

void DialogEditor::saveCurrentNode()
{
    if (!_currentOpenedNode)
        return;

    _currentOpenedNode->text = _ui->node_text_edit->toPlainText().toStdString();
}

void DialogEditor::saveCurrentDialog()
{
    saveCurrentNode();

    if (!_currentOpenedDialog)
        return;

    _currentOpenedDialog->name = _ui->name_edit->text().toStdString();

    NPCEditorHolder::DialogNode *node = _npcEditor->getNodeByIndex(
        _currentOpenedDialog->id,
        _ui->start_node_combo->currentIndex()
    );

    _currentOpenedDialog->startId = node->id;
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

}
