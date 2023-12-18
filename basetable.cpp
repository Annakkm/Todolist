#include "basetable.h"
#include "ui_basetable.h"

BaseTable::BaseTable(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::BaseTable)
{
    ui->setupUi(this);
}

BaseTable::~BaseTable()
{
    delete ui;
}



