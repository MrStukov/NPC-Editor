#ifndef DIALOGEDITOR_H
#define DIALOGEDITOR_H

#include <QDialog>

namespace Ui {
class DialogEditor;
}

class DialogEditor : public QDialog
{
    Q_OBJECT

public:
    explicit DialogEditor(QWidget *parent = 0);
    ~DialogEditor();

private:
    Ui::DialogEditor *ui;
};

#endif // DIALOGEDITOR_H
