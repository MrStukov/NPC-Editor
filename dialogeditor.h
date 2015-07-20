#ifndef DIALOGEDITOR_H
#define DIALOGEDITOR_H

#include <QDialog>
#include <QDebug>
#include "npceditorholder.h"

namespace Ui {
class DialogEditor;
}

class DialogEditor : public QDialog
{
    Q_OBJECT

public:
    explicit DialogEditor(QWidget *parent = 0);
    ~DialogEditor();

    void setNpcEditor( NPCEditorHolder *npcEditor );
    NPCEditorHolder *npcEditorHolder() const;

    // Обновление listWidget
    void updateRootListWidget();

private slots:
    void on_root_list_widget_activated(const QModelIndex &index);

    void on_node_list_widget_activated(const QModelIndex &index);

    void on_new_button_clicked();

    void on_delete_button_clicked();

private:
    // Загрузка
    void loadDialog( unsigned int index );
    void loadDialog( NPCEditorHolder::DialogHolder *holder );
    void loadNode( unsigned int dialogIndex, unsigned int nodeIndex );

    // Сохранение
    void saveCurrentNode();
    void saveCurrentDialog();

    Ui::DialogEditor *_ui;
    NPCEditorHolder *_npcEditor;

    // Выбранный в данный момент диалог
    NPCEditorHolder::DialogHolder *_currentOpenedDialog;
    NPCEditorHolder::DialogNode *_currentOpenedNode;
};

#endif // DIALOGEDITOR_H
