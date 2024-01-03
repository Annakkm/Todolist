#ifndef WINDOWEMPLOYEE_H
#define WINDOWEMPLOYEE_H

#include <QDialog>
#include <QGridLayout>
#include <QSqlError>
#include <QString>
#include <QSqlQuery>
#include <QDateEdit>
#include "connectdb.h"

namespace Ui {
class WindowEmployee;
}

class WindowEmployee : public QDialog
{
    Q_OBJECT

public:
    explicit WindowEmployee(QWidget *parent = nullptr);
    ~WindowEmployee();

public slots:
    void selectEmployeeData(const QString &email,const QString &password);
    void queryTask(const int &IdEmp);
private slots:
    void on_btnclose_clicked();
    void onCheckboxClicked(bool checked);



    void on_btnrefresh_clicked();

private:
    Ui::WindowEmployee *ui;
    QGridLayout * gridlayout_;
    ConnectDB db;

    int idEmployee;
    int idAdmin;
   // int taskId;
};

#endif // WINDOWEMPLOYEE_H
