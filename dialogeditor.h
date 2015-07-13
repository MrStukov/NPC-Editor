#ifndef DIALOGEDITOR_H
#define DIALOGEDITOR_H

#include <QDialog>
#include "npcfileeditorholder.h"

namespace Ui {
class DialogEditor;
}

class DialogEditor : public QDialog
{
    Q_OBJECT

public:
    explicit DialogEditor(QWidget *parent = 0);
    ~DialogEditor();

    void setNpcEditor( NPCFileEditorHolder *npcEditor );
    NPCFileEditorHolder *npcEditorHolder() const;

    // Обновление listWidget
    void updateListWidget();
private slots:
    void on_listWidget_activated(const QModelIndex &index);

private:

    // Загрузка диалога
    void loadDialog( unsigned int index );

    Ui::DialogEditor *_ui;
    NPCFileEditorHolder *_npcEditor;
};

#endif // DIALOGEDITOR_H
