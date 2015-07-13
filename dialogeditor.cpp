#include "dialogeditor.h"
#include "ui_dialogeditor.h"

DialogEditor::DialogEditor(QWidget *parent) :
    QDialog(parent),
    _ui(new Ui::DialogEditor)
{
    _ui->setupUi(this);
    _npcEditor = nullptr;
}

DialogEditor::~DialogEditor()
{
    delete _ui;
}

void DialogEditor::setNpcEditor(NPCFileEditorHolder *npcEditor)
{
    _npcEditor = npcEditor;
}

NPCFileEditorHolder *DialogEditor::npcEditorHolder() const
{
    return _npcEditor;
}

void DialogEditor::updateListWidget()
{
    _ui->listWidget->clear();
    for (unsigned int index = 0;
         index < _npcEditor->dialogLength();
         index++)
    {
        NPCFileEditorHolder::DialogHolder *dialog = _npcEditor->getDialog(index);

        _ui->listWidget->addItem(
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

    NPCFileEditorHolder::DialogHolder *dialog = _npcEditor->getDialog(index);

    _ui->name_edit->setText( QString(dialog->name.c_str()) );
    _ui->id_edit->setText( QString::number( dialog->id ) );
}

void DialogEditor::on_listWidget_activated(const QModelIndex &index)
{
    loadDialog( index.row() );
}
