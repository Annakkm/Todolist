#include "login.h"
#include "ui_login.h"
#include "connectdb.h"
#include <QString>
#include <QMessageBox>
#include <QSqlQueryModel>
#include <QSqlQuery>


Login::Login(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Login)
{
    ui->setupUi(this);
}

Login::~Login()
{
    delete ui;

}

void Login::on_btnEnter_clicked()
{
    QString email = ui->lineEdit_email->text();
    QString password = ui->lineEdit_password->text();

    if (email != "" && password != ""){
        QSqlQuery query;
        if(query.exec("select * from login_w where email = '" + email + "' and password = '" + password + "'")){
            int count = 0;

            while(query.next()){
                count++;
            }
            if (count == 1){
                qDebug()<<"email and password is correct";
            }
        }
    }
}


void Login::on_btnclose_clicked()
{
    db.CloseDatabase();
    QSqlDatabase::removeDatabase(QSqlDatabase::defaultConnection);

    this->close();

}

void Login::on_btnlogin_2_clicked()
{

    if(!db.OpenDatabase()){
        QMessageBox::information(this, "Помилка !", "Перeвірте підключення бази даних");
    }
    else {
        QString email = ui->lineEdit_email->text();
        QString password = ui->lineEdit_password->text();


        if (email != "" && password != "")
        {
            QSqlQuery queryAdmin;
            QString commandAdmin = "SELECT * FROM login_admin WHERE email = :email AND password = :password";
            queryAdmin.prepare(commandAdmin);

            QSqlQuery queryEmployee;
            QString commandEmployee = "SELECT * FROM login_w WHERE email = :email AND password = :password";
            queryEmployee.prepare(commandEmployee);

            if(ui->radioBtnAdmin->isChecked()){
                queryAdmin.bindValue(":email" , email);
                queryAdmin.bindValue(":password" , password);

                if (queryAdmin.exec() && queryAdmin.next()){
                    qDebug() << "Запит Select for admin виконано успішно!";
                    wAdmin = new WindowAdmin();
                    wAdmin->loginWithCredentials(email, password);
                    hide();
                    wAdmin->setWindowFlags(Qt::FramelessWindowHint);

                    wAdmin->show();

                }else {
                    qDebug() << "Помилка під час виконання запиту Select for admin";
                    QMessageBox::information(this, "Error!", "Ви ввели не правильні дані");
                }

            }else if (ui->radioBtnWorker->isChecked())
            {
                queryEmployee.bindValue(":email" , email);
                queryEmployee.bindValue(":password" , password);

                if (queryEmployee.exec()&& queryEmployee.next()){
                    qDebug() << "Запит Select for employee виконано успішно!";
                    wEmployee = new WindowEmployee();
                    wEmployee->selectEmployeeData(email, password);
                    hide();
                    wEmployee->setWindowFlags(Qt::FramelessWindowHint);

                    wEmployee->show();

                }else{
                    qDebug() << "Помилка під час виконання запиту Select for employee";
                    QMessageBox::information(this, "Error!", "Ви ввели не правильні дані");

                }
            }else if ((!ui->radioBtnWorker->isChecked()) && (!ui->radioBtnAdmin->isChecked()) ) {
                QMessageBox::information(this, "Error!", "Виберіть всі пункти");
            }
        }
    }
}

