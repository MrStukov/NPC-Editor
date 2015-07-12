#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    _ui(new Ui::MainWindow)
{
    _ui->setupUi(this);

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
}

void MainWindow::on_actionOpen_triggered()
{
    QString path; // TODO: Добавить получение этого пути через диалог

    if (!_npcEditor.openFile( path.toStdString() ))
    {
        // TODO: Добавить окно с ошибкой
    }

    updateListWidget();
}

void MainWindow::on_actionSave_triggered()
{
    if (!_npcEditor.save())
    {
        // TODO: Добавить окно с ошибкой
    }
}

void MainWindow::on_actionSave_As_triggered()
{
    QString path; // TODO: Добавить получение путии для сохранения

    if (!_npcEditor.save( path.toStdString() ))
    {
        // TODO: Добавить окно с ошибкой
    }
}

void MainWindow::on_actionExit_triggered()
{
    close();
}

void MainWindow::updateListWidget()
{
    _ui->listWidget->clear();
    for (unsigned int index = 0; index < _npcEditor.length(); index++)
    {
        NPCFileEditorHolder::NPCHolder *npc = _npcEditor.getNPC( index );
        _ui->listWidget->addItem( QString::number( npc->id ) + npc->name );
    }
}


void MainWindow::on_listWidget_clicked(const QModelIndex &index)
{

}

void MainWindow::loadNPCWith(unsigned int index)
{
    if (_currentNPCIndex != -1)
        updateData();

    _currentNPCIndex = index;
    NPCFileEditorHolder::NPCHolder *npc = _npcEditor.getNPC(index);
    _ui->name_line->setText( npc->name );
    _ui->id_line->setText( QString::number( npc->id ) );
}

void MainWindow::updateData()
{
    NPCFileEditorHolder::NPCHolder *npc = _npcEditor.getNPC( _currentNPCIndex );
    npc->name = _ui->name_line->text().toStdString();
}

void MainWindow::on_addNpc_button_clicked()
{
    NPCFileEditorHolder::NPCHolder *npc = _npcEditor.create();
    updateListWidget();

    int index = _npcEditor.getNPCIndex( npc );
    if (index != -1)
        loadNPCWith( index );
}
