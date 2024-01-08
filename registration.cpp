#include "registration.h"
#include "ui_registration.h"
#include <QMessageBox>
#include <ctime>
#include <QSqlError>
#include <QCryptographicHash>


Registration::Registration(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Registration)
{
    ui->setupUi(this);

    ui->label_idtext->hide();
    ui->lineEdit_id_read_only->hide();
    ui->lineEdit_id->hide();

   int number = getRandomNumber();
    ui->lineEdit_id_read_only->setText(QString::number(number));
}

Registration::~Registration()
{
    delete ui;
}

void Registration::on_btnclose_clicked()
{
    this->close();
}



void Registration::on_btn_link2_clicked()
{
    hide();
    login->setWindowFlags(Qt::FramelessWindowHint);
    login->show();
}

int Registration::getRandomNumber()
{

    std::srand(static_cast<unsigned>(std::time(nullptr)));

    int min = 10000;
    int max = 99999;


    int randomNumber = std::rand() % (max - min + 1) + min;

    return randomNumber;
}


void Registration::on_btnlogin_2_clicked()
{
    if(!db.OpenDatabase()){
        QMessageBox::information(this, "Помилка !", "Перeвірте підключення бази даних");
    }
    else {

        QString full_name = ui->lineEdit_fullname->text();
        QString email = ui->lineEdit_email->text();
        QString phone_number = ui->lineEdit_phoneNumber->text();
        QString password = ui->lineEdit_password->text();
        qDebug()<<"Password: "<<password;
        QString idCompany = ui->lineEdit_id_read_only->text();
        QString AdminId = ui->lineEdit_id->text();

        QString hashedPassword = hashPassword(password);
        qDebug()<<"HashedPassword: "<<hashedPassword;



        if(full_name != "" && email != "" && phone_number != "" && password != "" && (idCompany != "" ||AdminId != ""))
        {
            QSqlQuery query;
            QString commandAdmin = "INSERT INTO login_admin (idCompany, full_name, email, phone_number, password) "
                                   "VALUES (:idCompany, :full_name, :email, :phone_number, :password)";
            query.prepare(commandAdmin);

            QSqlQuery queryW;
            QString commandW = "INSERT INTO login_w (full_name, email, phone_number, password, AdminId)"
                               "VALUES (:full_name, :email, :phone_number, :password, :AdminId)";

            queryW.prepare(commandW);

            QSqlQuery queryif;
            queryif.prepare("SELECT * FROM login_admin WHERE idCompany = :idCompany");
            queryif.bindValue(":idCompany", idCompany);

           // query.bindValue(":idCompany", idCompany);

            if(ui->radioBtnAdmin->isChecked()){
                query.bindValue(":idCompany", idCompany);
                query.bindValue(":full_name", full_name);
                query.bindValue(":email", email);
                query.bindValue(":phone_number", phone_number);
                query.bindValue(":password", hashedPassword);

                if (query.exec()) {
                    qDebug() << "Запит INSERT for admin виконано успішно!";
                    hide();

                    login->setWindowFlags(Qt::FramelessWindowHint);
                    login->show();
                } else {
                    qDebug() << "Помилка під час виконання запиту INSERT for admin";
                }
            }
            else if (ui->radioBtnWorker->isChecked())
            {
                if(AdminId != ""){
                    queryW.bindValue(":full_name", full_name);
                    queryW.bindValue(":email", email);
                    queryW.bindValue(":phone_number", phone_number);
                    queryW.bindValue(":password", hashedPassword);
                    queryW.bindValue(":AdminId", AdminId);

                    if (queryW.exec()) {
                        if(queryif.exec())
                        {
                        qDebug() << "Запит INSERT for employee виконано успішно!";
                        wEmployee = new WindowEmployee();
                        hide();
                        wEmployee->selectEmployeeData(email, hashedPassword);
                        wEmployee->setWindowFlags(Qt::FramelessWindowHint);
                        wEmployee->show();
                        }
                        else{
                            qDebug() << "Помилка під час виконання запиту queryif ";

                        }
                    }
                    else {
                        qDebug() << "Помилка під час виконання запиту INSERT for employee";
                        QMessageBox::information(this, "Помилка!", "Ви ввели невідомий id компанії");
                    }
                }
                else if (AdminId == ""){
                    QMessageBox::information(this, "Помилка!", "Спробуйте ввести інформацію у поля");

                }
            }
            else if ((!ui->radioBtnAdmin->isChecked() || !ui->radioBtnWorker->isChecked()))
            {
                QMessageBox::information(this, "Помилка", "Ви не все заповнили");
            }
        }else {
            QMessageBox::information(this, "Помилка!", "Спробуйте ввести інформацію у поля");
        }

        db.CloseDatabase();
    }
}

QString Registration::hashPassword(const QString &password) {
    QByteArray salt = QCryptographicHash::hash(QByteArray::fromHex("deadbeef"), QCryptographicHash::Sha256);
    QByteArray hashedPassword = QCryptographicHash::hash(password.toUtf8() + salt, QCryptographicHash::Sha256);
    return QString(hashedPassword.toHex());
}


void Registration::openLoginWindow() {
    Login *loginWindow = new Login();
    loginWindow->show();
    hide();
    loginWindow->setWindowFlags(Qt::FramelessWindowHint);
}

