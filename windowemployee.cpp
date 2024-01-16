#include "windowemployee.h"
#include "ui_windowemployee.h"

WindowEmployee::WindowEmployee(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::WindowEmployee)
{
    ui->setupUi(this);

    if  (!db.OpenDatabase())
    {
        qDebug ()<<"База даних не відкрилася WindowEmployee ";
        QMessageBox::information(this, "Помилка !", "Перeвірте підключення бази даних");
    }

    ui->dateEdit_1->setReadOnly(true);
    ui->dateEdit_2->setReadOnly(true);
    ui->dateEdit_3->setReadOnly(true);
    ui->dateEdit_4->setReadOnly(true);
    ui->dateEdit_5->setReadOnly(true);
    ui->dateEdit_6->setReadOnly(true);

    ui->lineEdit_1->setReadOnly(true);
    ui->lineEdit_2->setReadOnly(true);
    ui->lineEdit_3->setReadOnly(true);
    ui->lineEdit_4->setReadOnly(true);
    ui->lineEdit_5->setReadOnly(true);
    ui->lineEdit_6->setReadOnly(true);
    QString dateString = "01012024";

    ui->dateEdit_1->setDate(QDate::fromString(dateString, "ddMMyyyy"));

    ui->dateEdit_2->setDate(QDate::fromString(dateString, "ddMMyyyy"));
    ui->dateEdit_3->setDate(QDate::fromString(dateString, "ddMMyyyy"));
    ui->dateEdit_4->setDate(QDate::fromString(dateString, "ddMMyyyy"));
    ui->dateEdit_5->setDate(QDate::fromString(dateString, "ddMMyyyy"));
    ui->dateEdit_6->setDate(QDate::fromString(dateString, "ddMMyyyy"));

}

WindowEmployee::~WindowEmployee()
{
    delete ui;
}

void WindowEmployee::on_btnclose_clicked()
{
    db.CloseDatabase();
    QSqlDatabase::removeDatabase(QSqlDatabase::defaultConnection);
    this->close();
}

void WindowEmployee::onCheckboxClicked(bool checked)
{
    QCheckBox* checkbox = qobject_cast<QCheckBox*>(sender());
    int taskId = -1;

    if (checkbox) {
        taskId = checkbox->property("task_id").toInt();
        qDebug() << "TaskID = " << taskId;

        QSqlQuery updateQuery;
        QString updateCommand;

        if (checked) {
            // Якщо чекбокс встановлено, оновіть статус на 'done'
            updateCommand = "UPDATE tasks SET status = 'done' WHERE task_id = :task_id";
        } else {
            // Якщо чекбокс скасовано, оновіть статус на 'set'
            updateCommand = "UPDATE tasks SET status = 'set' WHERE task_id = :task_id";
        }

        updateQuery.prepare(updateCommand);
        updateQuery.bindValue(":task_id", taskId);

        if (updateQuery.exec()) {
            qDebug() << "Статус завдання оновлено на '" << (checked ? "done" : "set") << "' для завдання з ідентифікатором" << taskId;
        } else {
            qDebug() << "Помилка при оновленні статусу завдання:" << updateQuery.lastError().text();
        }
    }
}





void WindowEmployee::selectEmployeeData(const QString &email,const QString &password)
{
    qDebug()<<"selectEmployeeData enter";
    qDebug()<<"Email: "<< email;
    qDebug()<<"Password: "<< password;

    QSqlQuery query;
    QString command = "SELECT * FROM login_w WHERE email = :email and password = :password";
    query.prepare(command);
    query.bindValue(":email", email);
    query.bindValue(":password", password);

    if (query.exec()) {
        // Отримати перший запис, якщо вона доступна
        if (query.next()) {
            idEmployee = query.value(0).toInt();
            QString fullname = query.value(1).toString();

            qDebug() << "Успішний вхід. id =" << idEmployee;
            qDebug() << "Успішний вхід. fullname =" << fullname;
            qDebug() << "Успішний вхід. email =" << email;

            ui->employeeName->setText(fullname);
            ui->employeeEmail->setText(email);
        } else {
            qDebug() << "Записи для заданої електронної пошти та пароля не знайдено.";
        }
    } else {
        qDebug() << "Не вдалося виконати запит. Помилка:" << query.lastError().text();
    }

    queryTask(idEmployee);

}

void WindowEmployee::queryTask(const int &IdEmp)
{
    qDebug()<<"IdEmp: "<<IdEmp;
    QSqlQuery queryTask;
    QString command ="SELECT task_id, description, deadline, assigned_by_admin_id, status FROM tasks WHERE assigned_to_employee_id = :assigned_to_employee_id";
    queryTask.prepare(command);
    queryTask.bindValue(":assigned_to_employee_id", IdEmp);
    if(queryTask.exec()){
        int i = 0;

        while(queryTask.next() && i <= 6){
            int taskId = queryTask.value("task_id").toInt();
            QString description = queryTask.value("description").toString();
            QDate date = queryTask.value("deadline").toDate();
            QString status = queryTask.value("status").toString();
            qDebug()<<"desc = "<<description;

            QString lineEditName = QString("lineEdit_%1").arg(i + 1);
            QString dateEditName = QString("dateEdit_%1").arg(i + 1);
            QString checkBoxName = QString("checkBox_%1").arg(i + 1);


            QLineEdit* currentLineEdit = findChild<QLineEdit*>(lineEditName);
            QDateEdit* currentDateEdit = findChild<QDateEdit*>(dateEditName);
            QCheckBox* currentCheckBox = findChild<QCheckBox*>(checkBoxName);

            // Встановіть значення тільки для поточного lineEdit
            if (currentLineEdit && currentDateEdit && currentCheckBox) {
                currentLineEdit->setText(description);
                currentDateEdit->setDate(date);

                currentCheckBox->setProperty("task_id", taskId);

                connect(currentCheckBox, SIGNAL(clicked(bool)), this, SLOT(onCheckboxClicked(bool)));

            } else {
                qDebug() << "QLineEdit not found for index:" << i;
            }

            if (currentCheckBox) {
                currentCheckBox->setChecked(status.toLower() == "done");
            } else {
                qDebug() << "QCheckBox не знайдено для index:" << i;
            }

            i++;
        }
    }
    else {
        qDebug() << "Помилка під час виконання запиту SELECT:" << queryTask.lastError().text();
    }
}

/*void WindowAdmin::loginWithCredentialsEmployee(const QString &email, const QString &password)
{
    QSqlQuery query;
    query.prepare("SELECT AdminId,full_name FROM login_w WHERE email = :email AND password = :password");
    query.bindValue(":email", email);
    query.bindValue(":password", password);

    if (query.exec() && query.next()) {
        idCompany = query.value("AdminId").toInt();
        QString name = query.value("full_name").toString();

        ui->label_name->setText(name);
        ui->label_email->setText(email);

        qDebug() << "Успішний вхід. IdCompany =" << idCompany;
        qDebug() << "Успішний вхід. name =" << name;

        QString searchText = ui->SearchLine->text();
    } else {
        qDebug() << "Невірний email або пароль";
    }
}*/

void WindowEmployee::on_btnrefresh_clicked()
{
    qDebug()<<idEmployee;

    queryTask(idEmployee);
}

