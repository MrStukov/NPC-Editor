#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QMessageBox>
#include "npceditorholder.h"
#include "dialogeditor.h"

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

    void on_listWidget_activated(const QModelIndex &index);

    void on_delete_button_clicked();

    void on_actionDialog_manager_triggered();

    void dialogsUpdateSlot();
private:
    // Метод обновления listWidet
    void updateListWidget();

    // Обновление набора диалогов
    void updateDialogs();

    // Метод загрузки данных в формы
    void loadNPCWith( unsigned int index );

    // Метод обновления данных из форм
    void updateData();

    // Метод очистки форм
    void clearForm();

    Ui::MainWindow *_ui;
    DialogEditor *_dialogEditor;

    NPCEditorHolder _npcEditor;
    int _currentNPCIndex;
};

#endif // MAINWINDOW_H
