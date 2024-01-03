#include "login.h"
#include "ui_login.h"
#include "connectdb.h"
#include <QString>
#include <QMessageBox>
#include <QSqlQueryModel>
#include <QSqlQuery>
#include <QCryptographicHash>

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
// ... ваш код ...


QString Login::hashPassword(const QString &password) {
    QByteArray salt = QCryptographicHash::hash(QByteArray::fromHex("deadbeef"), QCryptographicHash::Sha256);
    QByteArray hashedPassword = QCryptographicHash::hash(password.toUtf8() + salt, QCryptographicHash::Sha256);
    qDebug()<<salt;
    qDebug()<<hashedPassword;
    return QString(hashedPassword.toHex());
}



void Login::on_btnclose_clicked()
{
    db.CloseDatabase();
    QSqlDatabase::removeDatabase(QSqlDatabase::defaultConnection);

    this->close();

}

void Login::on_btnlogin_2_clicked()
{

    if (!db.OpenDatabase()) {
        QMessageBox::information(this, "Помилка!", "Перевірте підключення бази даних");
    } else {
        QString email = ui->lineEdit_email->text();
        QString password = ui->lineEdit_password->text();
        qDebug() << "Password insert: " << password;

        // Хешуйте введений пароль з використанням солі для порівняння
        QString hashedInputPassword = hashPassword(password);
        qDebug() << "Hashed Input Password: " << hashedInputPassword;

        if (email != "" && password != "") {
            QSqlQuery queryAdmin;
            QString commandAdmin = "SELECT * FROM login_admin WHERE email = :email";
            queryAdmin.prepare(commandAdmin);
            queryAdmin.bindValue(":email", email);

            if (queryAdmin.exec()) {
                if (queryAdmin.next()) {
                    QString hashedPasswordFromDB = queryAdmin.value("password").toString();
                    qDebug() << "Hashed Password DB: " << hashedPasswordFromDB;

                    if (ui->radioBtnAdmin->isChecked()) {
                        queryAdmin.bindValue(":password", hashedPasswordFromDB);

                        qDebug() << "Запит Select for admin виконано успішно!";
                        wAdmin = new WindowAdmin();
                        wAdmin->loginWithCredentials(email, hashedPasswordFromDB);
                        hide();
                        wAdmin->setWindowFlags(Qt::FramelessWindowHint);

                        if (hashedInputPassword == hashedPasswordFromDB) {
                            qDebug() << "Email і пароль вірні";
                            wAdmin->show();
                        } else {
                            qDebug() << "Невірний пароль";
                        }
                    }
                }
                else{
                    qDebug() << "Помилка під час виконання запиту queryEmployee.next";
                }
            }else{
                qDebug() << "Помилка під час виконання запиту queryEmployee.exec";
            }

            QSqlQuery queryEmployee;
            QString commandEmployee = "SELECT * FROM login_w WHERE email = :email";
            queryEmployee.prepare(commandEmployee);
            queryEmployee.bindValue(":email", email);

            if (queryEmployee.exec()) {
                if (queryEmployee.next()) {
                    QString hashedPasswordFromDB = queryEmployee.value("password").toString();
                    qDebug() << "Hashed Password DB: " << hashedPasswordFromDB;

                    if (ui->radioBtnWorker->isChecked()) {
                        queryEmployee.bindValue(":password", hashedPasswordFromDB);

                        if (hashedInputPassword == hashedPasswordFromDB) {
                            qDebug() << "Email і пароль вірні";
                            wEmployee = new WindowEmployee();
                            wEmployee->selectEmployeeData(email, hashedPasswordFromDB);
                            hide();
                            wEmployee->setWindowFlags(Qt::FramelessWindowHint);

                            wEmployee->show();
                        } else {
                            qDebug() << "Невірний пароль";
                        }

                    }
                }else{
                    qDebug() << "Помилка під час виконання запиту queryEmployee.next";
                }
            }else{
                qDebug() << "Помилка під час виконання запиту queryEmployee.exec";
            }

            if ((!ui->radioBtnWorker->isChecked()) && (!ui->radioBtnAdmin->isChecked())) {
                QMessageBox::information(this, "Error!", "Виберіть всі пункти");
            }
        }
    }
}



void Login::on_btn_link2_clicked()
{

}


/*void Login::on_btnEnter_clicked()
{
    QString email = ui->lineEdit_email->text();
    QString password = ui->lineEdit_password->text();
    qDebug()<<"password: "<<password;
    QSqlQuery query;
    query.prepare("SELECT * FROM login_w WHERE email = :email");
    query.bindValue(":email", email);

    if (query.exec() && query.next()) {
        QString hashedPasswordFromDB = query.value("password").toString();
        QString saltFromDB = query.value("salt").toString();
        qDebug()<<"Hashed Password :"<< hashedPasswordFromDB;
        // Хешуйте введений пароль з використанням солі для порівняння
        QString hashedInputPassword = hashPassword(password);
        qDebug()<<"hashedInputPassword :"<< hashedInputPassword;

        if (hashedInputPassword == hashedPasswordFromDB) {
            qDebug() << "Email і пароль вірні";
        } else {
            qDebug() << "Невірний пароль";
        }
    }
}*/
