#include "dialogeditor.h"
#include "ui_dialogeditor.h"

DialogEditor::DialogEditor(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogEditor)
{
    ui->setupUi(this);
}

DialogEditor::~DialogEditor()
{
    delete ui;
}
