#ifndef BASETABLE_H
#define BASETABLE_H

#include <QDialog>

namespace Ui {
class BaseTable;
}

class BaseTable : public QDialog
{
    Q_OBJECT

public:
    explicit BaseTable(QWidget *parent = nullptr);
    ~BaseTable();

private slots:

private:
    Ui::BaseTable *ui;
};

#endif // BASETABLE_H
