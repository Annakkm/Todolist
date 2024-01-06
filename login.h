#ifndef LOGIN_H
#define LOGIN_H

#include <QDialog>
#include <qdebug.h>
#include <QString>
#include "connectdb.h"
#include "windowadmin.h"
#include "windowemployee.h"

class Mainwindow;

namespace Ui {
class Login;
}

class Login : public QDialog
{
    Q_OBJECT

public:
    explicit Login(QWidget *parent = nullptr);
    ~Login();
public slots:
    void openRegistrationWindow();
private slots:

    //void on_btnEnter_clicked();
    void on_btnclose_clicked();
    void on_btnlogin_2_clicked();

    void on_btnlinkreg_clicked();

private:
    Ui::Login *ui;
    ConnectDB db;
    WindowAdmin * wAdmin;
    WindowEmployee * wEmployee;
    QString hashPassword(const QString &password);

};

#endif // LOGIN_H
