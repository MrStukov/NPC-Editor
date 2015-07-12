#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "npcfileeditorholder.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_actionNew_triggered();

    void on_actionOpen_triggered();

    void on_actionSave_triggered();

    void on_actionSave_As_triggered();

    void on_actionExit_triggered();

    void on_listWidget_clicked(const QModelIndex &index);

    void on_addNpc_button_clicked();

private:
    // Метод обновления listWidet
    void updateListWidget();

    // Метод загрузки данных в формы
    void loadNPCWith( unsigned int index );

    // Метод обновления данных из форм
    void updateData();

    Ui::MainWindow *_ui;
    NPCFileEditorHolder _npcEditor;
    int _currentNPCIndex;
};

#endif // MAINWINDOW_H
