#ifndef REGISTRATION_H
#define REGISTRATION_H

#include <QDialog>
#include <QRadioButton>
#include <QLabel>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QWidget>
#include <QSqlQuery>
#include <QDebug>
#include "login.h"
#include "connectdb.h"
#include "windowadmin.h"
#include "windowemployee.h"

namespace Ui {
class Registration;
}


class Registration : public QDialog
{
    Q_OBJECT

public:
    explicit Registration(QWidget *parent = nullptr);
    ~Registration();


private slots:

    void on_btnclose_clicked();
    void on_pushButton_clicked();
    void on_btn_link2_clicked();

    int getRandomNumber();

    void on_btnlogin_2_clicked();

private:
    Ui::Registration *ui;
    Login * login = new Login();
    WindowAdmin * wAdmin ;
    WindowEmployee * wEmployee;
    ConnectDB db;
};

#endif // REGISTRATION_H
