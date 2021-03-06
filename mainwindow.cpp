#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    _ui(new Ui::MainWindow)
{
    _ui->setupUi(this);

    _dialogEditor = new DialogEditor(this);
    _dialogEditor->setNpcEditor( &_npcEditor );
    connect(_dialogEditor, SIGNAL(dialogsUpdated()), this, SLOT(dialogsUpdateSlot()));

    _currentNPCIndex = -1;
}

MainWindow::~MainWindow()
{
    delete _ui;
}

void MainWindow::on_actionNew_triggered()
{
    _npcEditor.clear();
    updateListWidget();
    updateDialogs();
}

void MainWindow::on_actionOpen_triggered()
{
    QString path = QFileDialog::getOpenFileName(this,
                    tr("Open file"), QString(), tr("NPC Files (*.xml)"));
    if (path.length() <= 0)
        return;

    if (!_npcEditor.openFile( path.toStdString() ))
        QMessageBox::warning(this, tr("Opening"), tr("Can't open file.") );

    updateListWidget();
    updateDialogs();
    _dialogEditor->clear();
    _dialogEditor->updateRootListWidget();
}

void MainWindow::on_actionSave_triggered()
{
    updateData();
    if (!_npcEditor.save())
        QMessageBox::warning(this, tr("Saving"), tr("Can't save file.") );

    updateListWidget();
    updateDialogs();
}

void MainWindow::on_actionSave_As_triggered()
{
    updateData();
    QString path = QFileDialog::getSaveFileName(this,
                    tr("Save file"), QString(), tr("NPC Files (*.xml)"));

    if (!_npcEditor.save( path.toStdString() ))
        QMessageBox::warning(this, tr("Saving"), tr("Can't save file.") );

    updateListWidget();
    updateDialogs();
}

void MainWindow::on_actionExit_triggered()
{
    close();
}

void MainWindow::updateListWidget()
{
    _ui->listWidget->clear();
    for (unsigned int index = 0; index < _npcEditor.npcLength(); index++)
    {
        NPCEditorHolder::NPCHolder *npc = _npcEditor.getNPC( index );
        _ui->listWidget->addItem( QString::number( npc->id ) + tr(" ") + QString(npc->name.c_str()) );
    }
}

void MainWindow::updateDialogs()
{
    _ui->dialog_combo->clear();
    for (unsigned int index = 0;
         index < _npcEditor.dialogLength();
         index++)
        _ui->dialog_combo->addItem(
            QString::number( _npcEditor.getDialog( index )->id ) + " " + QString( _npcEditor.getDialog( index )->name.c_str())
        );
}

void MainWindow::on_listWidget_clicked(const QModelIndex &index)
{

}

void MainWindow::loadNPCWith(unsigned int index)
{
    if (_currentNPCIndex != -1)
        updateData();

    _currentNPCIndex = index;
    NPCEditorHolder::NPCHolder *npc = _npcEditor.getNPC(index);
    _ui->name_line->setText( QString(npc->name.c_str()) );
    _ui->id_line->setText( QString::number( npc->id ) );
    _ui->dialog_combo->setCurrentIndex( npc->dialogId );
}

void MainWindow::updateData()
{
    if (_currentNPCIndex == -1)
        return;

    NPCEditorHolder::NPCHolder *npc = _npcEditor.getNPC( _currentNPCIndex );
    npc->name = _ui->name_line->text().toStdString();

    if (_ui->dialog_combo->currentIndex() > -1)
        npc->dialogId = _ui->dialog_combo->currentIndex();
    else
        npc->dialogId = 0;

    _ui->listWidget->item( _currentNPCIndex )->setText(
        QString::number( npc->id ) + tr(" ") + QString(npc->name.c_str())
    );
}

void MainWindow::clearForm()
{
    _ui->name_line->clear();
    _ui->id_line->clear();
    _currentNPCIndex = -1;
}

void MainWindow::on_addNpc_button_clicked()
{
    NPCEditorHolder::NPCHolder *npc = _npcEditor.createNPC();
    updateListWidget();

    int index = _npcEditor.getNPCIndex( npc );
    if (index != -1)
        loadNPCWith( index );
}

void MainWindow::on_listWidget_activated(const QModelIndex &index)
{
    loadNPCWith( index.row() );
}

void MainWindow::on_delete_button_clicked()
{
    if (_currentNPCIndex == -1)
        return;

    NPCEditorHolder::NPCHolder *npc = _npcEditor.getNPC( _currentNPCIndex );
    _npcEditor.removeNPC( npc->id );

    clearForm();
    updateListWidget();
}

void MainWindow::on_actionDialog_manager_triggered()
{
    _dialogEditor->show();
}

void MainWindow::dialogsUpdateSlot()
{
    updateDialogs();
}
