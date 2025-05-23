#include "windowadmin.h"
#include "ui_windowadmin.h"
#include "login.h"



WindowAdmin::WindowAdmin(QWidget *parent) :
    QDialog(parent), ui(new Ui::WindowAdmin), layout_2(new QVBoxLayout()), currentSelectedButton(nullptr), buttonClicked(false)
{
    ui->setupUi(this);
    if  (!db.OpenDatabase())
    {
        qDebug ()<<"База даних не відкрилася WindowAdmin ";
        QMessageBox::information(this, "Помилка !", "Перeвірте підключення бази даних");
    }
    gridlayout_ = new QGridLayout(ui->scrollAreaWidgetContents);

    ui->widget_tasks->setStyleSheet("background-color: rgb(90, 95, 111);");
    ui->baseside->setStyleSheet("background-color: rgb(90, 95, 111);");

    gridLayout_main = new QGridLayout(ui->scroll_widget_main);

    connect(ui->SearchLine, &QLineEdit::textChanged, this, &WindowAdmin::updateSearchResults);
    ui->labeldel->hide();
}

void WindowAdmin::onRadioButtonClicked()
{
    QRadioButton *clickedRadioButton = qobject_cast<QRadioButton*>(sender());
    if (clickedRadioButton && radioButtonLineEditMap.contains(clickedRadioButton))
    {
        QLineEdit *associatedLineEdit = radioButtonLineEditMap[clickedRadioButton];

        // реалізувати видалення (запит в базу)

        if (associatedLineEdit)
        {
            associatedLineEdit->clear();
            // реалізувати видалення (запит в базу)
        }
    }
}




void WindowAdmin::updateSearchResults(const QString &text) {
    displayEmployeesForAdmin(idCompany, text);
}

void WindowAdmin::loginWithCredentialsAdmin(const QString &email, const QString &password)
{
    QSqlQuery query;
    query.prepare("SELECT idCompany,full_name FROM login_admin WHERE email = :email AND password = :password");
    query.bindValue(":email", email);
    query.bindValue(":password", password);

    if (query.exec() && query.next()) {
        idCompany = query.value("idCompany").toInt();
        QString name = query.value("full_name").toString();

        ui->label_name->setText(name);
        ui->label_email->setText(email);

        qDebug() << "Успішний вхід. IdCompany =" << idCompany;
        qDebug() << "Успішний вхід. name =" << name;

        QString searchText = ui->SearchLine->text();
        displayEmployeesForAdmin(idCompany, searchText);
    } else {
        qDebug() << "Невірний email або пароль";
    }
}



void WindowAdmin::displayEmployeesForAdmin(int adminIdCompany, const QString &seachText)
{
    int row = 0;
    int col = 0;

    QLayoutItem *child;
    while ((child = gridlayout_->takeAt(0)) != nullptr) {
        delete child->widget();
        delete child;
    }

    QSqlQuery query;
    query.prepare("SELECT * FROM login_w WHERE AdminId = :AdminId");
    query.bindValue(":AdminId", adminIdCompany);
    query.exec();

    while (query.next()) {
        QString employeeName = query.value("full_name").toString();
        QString email = query.value("email").toString();
        if (employeeName.toLower().startsWith(seachText.toLower())) {

            QPushButton *button = new QPushButton();
            button->setText(QString("%1\n%2").arg(employeeName).arg(email));
            button->setStyleSheet("text-align: left;");

            connect(button, &QPushButton::clicked, this, [this, employeeName, email, button]()
                    {
                        onEmployeeButtonClicked(employeeName, email);
                    });

            gridlayout_->setSpacing(0);
            gridlayout_->setContentsMargins(0, 0, 0, 0);
            gridlayout_->addWidget(button, row, col);
            col++;
            if (col == 1) {
                col = 0;
                row++;
            }
        }
    }
    ui->scrollAreaWidgetContents->setLayout(gridlayout_);
}
void WindowAdmin::onEmployeeButtonClicked(QString employeeName, QString email) // метод натиснення на кнопку
{

    selectedAdminId = idCompany;
    qDebug()<<"Admin: "<<selectedAdminId;

    selectedEmployeeId = getEmployeeId(employeeName, email);
    qDebug()<<"Employee: "<<selectedEmployeeId;


    selectedEmployeeName = employeeName;
    selectedEmployeeEmail = email;

    QPushButton *clickedButton = qobject_cast<QPushButton*>(sender());

    if (clickedButton && !clickedButton->isEnabled()) {
        return;

    }
    qDebug() << "Натискання на кнопку для:" << employeeName << email;

    QSqlQuery queryTasks;
    queryTasks.prepare("SELECT description, deadline, status FROM tasks WHERE assigned_to_employee_id = :employeeId");
    queryTasks.bindValue(":employeeId", selectedEmployeeId);
    QStringList tasks = getTasksForEmployee(selectedEmployeeId);
    qDebug()<<"Tasks: "<< tasks;

    QList<QDate> dates = getDateForEmployee(selectedEmployeeId);
    qDebug()<<"Dates: "<< dates;

    QStringList statuses = getStatusesForEmployee(selectedEmployeeId);
    qDebug()<<"Statuses: "<< statuses;

    if (clickedButton && !buttonClicked) {
        handleButtonClick(static_cast<QPushButton*>(sender()));

        createLineEdits(6);
        ui->widget_tasks->show();
        ui->widget_tasks->setStyleSheet("border-radius: 10px;"
                                        "background-color: rgb(90, 95, 111);");
    }

    if (queryTasks.exec()) {
        int i = 0;

        while (queryTasks.next() && i < 6) {
            QString description = queryTasks.value("description").toString();
            QDate deadline = queryTasks.value("deadline").toDate();

            qDebug()<<"Опис: "<< description;
            qDebug()<<"Дата: "<< deadline;
            qDebug()<<"lineedits size = "<<lineEdits.size()<< ", tasks = " << tasks.size() ;
            qDebug()<<"Status: "<< statuses;

                lineEdits.at(i)->setText(tasks.at(i));
                setDateTime(dateEdits.at(i), dates.at(i));
                qDebug()<<"checkBoxes: "<< checkBoxes;

                if (i < statuses.size()) {
                    checkBoxes.at(i)->setChecked(statuses.at(i).toLower() == "done");
                }
            i++;
        }
    } else {
        qDebug() << "Помилка під час виконання запиту SELECT:" << queryTasks.lastError().text();
    }
    selectedEmployeeId = getEmployeeId(employeeName, email);
}



void WindowAdmin::loadDeadlines(int employeeId)
{
    QList<QDate> deadlines = getDeadlines(employeeId);

    qDebug()<< deadlines.size()<<", " << dateEdits.size();

    if (deadlines.size() != dateEdits.size()) {
        qDebug() << "Помилка: Розмір списку дедлайнів не відповідає розміру dateEdits";
        return;
    }

    for (int i = 0; i < dateEdits.size(); ++i) {
        setDateTime(dateEdits.at(i), deadlines.at(i));
    }
}

QStringList WindowAdmin::getStatusesForEmployee(int employeeId)
{
    QStringList statuses;

    QSqlQuery queryStatuses;
    queryStatuses.prepare("SELECT status FROM tasks WHERE assigned_to_employee_id = :employeeId");
    queryStatuses.bindValue(":employeeId", employeeId);

    if (queryStatuses.exec())
    {
        while (queryStatuses.next())
        {
            statuses.append(queryStatuses.value("status").toString());
        }
    }
    else
    {
        qDebug() << "Error fetching task statuses:" << queryStatuses.lastError().text();
    }

    return statuses;
}


void WindowAdmin::handleButtonClick(QPushButton *clickedButton)
{
    if (clickedButton->property("active").toBool()) {

        clickedButton->setStyleSheet(""
                                     "background-color: rgb(30, 101, 172);;"
                                     "color: white;"
                                     "text-align: left;");
        clickedButton->setProperty("active", false);
        currentSelectedButton = nullptr;
    } else {
        clickedButton->setStyleSheet("background-color: rgb(30, 101, 172);"
                                     "color: white;"
                                     "text-align: left;");
        clickedButton->setProperty("active", true);

        if (currentSelectedButton && currentSelectedButton != clickedButton) {
            currentSelectedButton->setStyleSheet(""
                                                 "background-color: rgb(90, 95, 111);;"
                                                 "color: black;"
                                                 "text-align: left;");
            currentSelectedButton->setProperty("active", false);
        }

        currentSelectedButton = clickedButton;
    }
}




QList<QDate> WindowAdmin::getDeadlines(int employeeId)
{
    QList<QDate> deadlines;

    QSqlQuery queryDeadlines;
    queryDeadlines.prepare("SELECT deadline FROM tasks WHERE assigned_to_employee_id = :employeeId");
    queryDeadlines.bindValue(":employeeId", employeeId);

    if (queryDeadlines.exec()) {
        while (queryDeadlines.next()) {
            deadlines.append(queryDeadlines.value("deadline").toDate());
        }
    } else {
        qDebug() << "Помилка отримання дедлайнів:" << queryDeadlines.lastError().text();
    }

    return deadlines;
}



void WindowAdmin::setDateTime(QDateEdit* dateEdit, const QDate& deadlineDate)
{
    if (dateEdit) {
        if (deadlineDate.isValid()) {
            dateEdit->setDate(deadlineDate);
        } else {
            dateEdit->setDate(QDate());
        }
    }
}

QList<QDate> WindowAdmin::getDateForEmployee(int employeeId)
{
    QList<QDate> dates;

    QSqlQuery queryDate;
    queryDate.prepare("SELECT deadline FROM tasks WHERE assigned_to_employee_id = :employeeId");
    queryDate.bindValue(":employeeId", employeeId);

    if (queryDate.exec()) {
        while (queryDate.next()) {
            dates.append(queryDate.value("deadline").toDate());
        }
    } else {
        qDebug() << "Error fetching tasks:" << queryDate.lastError().text();
    }

    return dates;
}

QStringList WindowAdmin::getTasksForEmployee(int employeeId)
{
    QStringList tasks;

    QSqlQuery queryTasks;
    queryTasks.prepare("SELECT description FROM tasks WHERE assigned_to_employee_id = :employeeId");
    queryTasks.bindValue(":employeeId", employeeId);

    if (queryTasks.exec()) {
        while (queryTasks.next()) {
            tasks.append(queryTasks.value("description").toString());
        }
    } else {
        qDebug() << "Error fetching tasks:" << queryTasks.lastError().text();
    }

    return tasks;
}

int WindowAdmin::getEmployeeId(const QString& employeeName, const QString& email)
{
    QSqlQuery query;
    query.prepare("SELECT id FROM login_w WHERE full_name = :employeeName AND email = :email");
    query.bindValue(":employeeName", employeeName);
    query.bindValue(":email", email);

    if (query.exec() && query.next()) {
        return query.value("id").toInt();
    } else {
        qDebug() << "Error fetching employee ID:" << query.lastError().text();
        return -1;
    }
}

int WindowAdmin::getAdminId(const QString& adminName, const QString& email)
{
    QSqlQuery queryA;
    queryA.prepare("SELECT idCompany FROM login_admin WHERE full_name = :adminName AND email = :email");
    queryA.bindValue(":adminName", adminName);
    queryA.bindValue(":email", email);

    if (queryA.exec() && queryA.next()) {
        return queryA.value("idCompany").toInt();
    } else {
        qDebug() << "Error Admin ID:" << queryA.lastError().text();
        return -1;
    }
}

void WindowAdmin::clearLineEdits()
{
    for (QLineEdit* lineEdit : lineEdits) {
        delete lineEdit;
    }
    lineEdits.clear();
}

void WindowAdmin::clearRadioButton()
{
    for (QRadioButton* radioButton : radiobuttons) {
        delete radioButton;
    }
    radiobuttons.clear();
}

void WindowAdmin::clearCheckBox()
{
    for (QCheckBox* checkBox : checkBoxes) {
        delete checkBox;
    }
    checkBoxes.clear();
}

void WindowAdmin::clearDateEdits()
{
    for (QDateEdit* dateEdit : dateEdits) {
        delete dateEdit;
    }
    dateEdits.clear();
}

void WindowAdmin::onLabelClicked(const QString &link)
{
    qDebug() << "Ви натиснули на посилання:" << link;
}

QDate WindowAdmin::getDeadlineForTask(int employeeId, const QString &taskDescription)
{
    QSqlQuery queryDeadline;
    queryDeadline.prepare("SELECT deadline FROM tasks WHERE assigned_to_employee_id = :employeeId AND description = :description");
    queryDeadline.bindValue(":employeeId", employeeId);
    queryDeadline.bindValue(":description", taskDescription);

    if (queryDeadline.exec() && queryDeadline.next())
    {
        return queryDeadline.value("deadline").toDate();
    }
    else
    {
        qDebug() << "Помилка отримання дедлайну для завдання:" << queryDeadline.lastError().text();
        return QDate(); // Повернути невалідну дату у випадку помилки
    }
}

void WindowAdmin::createLineEdits(int number)
{
    clearLineEdits();
    clearDateEdits();
    clearCheckBox();
    clearRadioButton();
    clearLayout(layout_2);

    QList<QString> existingTasks = getTasksForEmployee(selectedEmployeeId);

    for (int i = 0; i < number; ++i)
    {
        QHBoxLayout *rowLayout = new QHBoxLayout();

        QLineEdit *lEdit = new QLineEdit();
        QDateEdit *dateEdit = new QDateEdit();
        QCheckBox *checkbox = new QCheckBox();
        QRadioButton *radioButton = new QRadioButton();

        // Загальний стиль для всіх елементів
        lEdit->setStyleSheet("background: rgb(30, 101, 172);"
                             "border: 2px solid rgb(255, 255, 255);"
                             "color: rgb(255, 255, 255);"
                             "border-radius: 14px;"
                             "height: 24px;"
                             "padding-left: 20px;");

        lEdit->setPlaceholderText(QString("Введіть завдання %1").arg(i + 1));
        rowLayout->addWidget(lEdit);

        // Загальний стиль для QDateEdit
        dateEdit->setStyleSheet("QDateEdit {"
                                "background: rgb(30, 101, 172);"
                                "padding: 3px;"
                                "color: rgb(255, 255, 255);"
                                "font-size: 10;"
                                "border: none;"
                                "outline: none;"
                                "border-radius: 9px;"
                                "}"
                                "QDateEdit::down-arrow {"
                                "    image: url(:/new/prefix4/img/calendar_white.png);"
                                "    width: 30px;"
                                "    height: 16px;"
                                "    border: none;"
                                "}");
        QString dateString = "01012024";

        dateEdit->setDate(QDate::fromString(dateString, "ddMMyyyy"));

        rowLayout->addWidget(dateEdit);
        rowLayout->addWidget(checkbox);
        connect(radioButton, &QRadioButton::clicked, this, &WindowAdmin::onRadioButtonClicked);

        ui->gridLayout_2->addWidget(radioButton);

        radioButtonLineEditMap[radioButton] = lEdit;

        layout_2->addLayout(rowLayout);

        lineEdits.append(lEdit);
        dateEdits.append(dateEdit);
        checkBoxes.append(checkbox);
        radiobuttons.append(radioButton);

        if (i < existingTasks.size())
        {
            QString taskDescription = existingTasks.at(i);
            QDate taskDeadline = getDeadlineForTask(selectedEmployeeId, taskDescription);

            int daysUntilDeadline = QDate::currentDate().daysTo(taskDeadline);

            if (daysUntilDeadline <= 7)
            {
                if (!isTaskAlreadyExists(taskDescription))
                {
                    // Змінити стиль для елементів, якщо дедлайн близько
                    lEdit->setStyleSheet("background: rgb(255, 0, 0);"
                                         "border: 2px solid rgb(255, 255, 255);"
                                         "color: rgb(255, 255, 255);"
                                         "border-radius: 14px;"
                                         "height: 24px;"
                                         "padding-left: 20px;");
                }
            }
        }
    }
    ui->widget_tasks->setLayout(layout_2);
    ui->widget_tasks->show();
    ui->labeldel->show();
}


bool WindowAdmin::isTaskAlreadyExists(const QString& taskDescription)
{
    QSqlQuery query;
    query.prepare("SELECT COUNT(*) FROM tasks WHERE description = :description AND assigned_to_employee_id = :assigned_to_employee_id");
    query.bindValue(":description", taskDescription);
    query.bindValue(":assigned_to_employee_id", selectedEmployeeId);

    if (query.exec() && query.next()) {
        int taskCount = query.value(0).toInt();
        return taskCount > 0;

    } else {
        qDebug() << "Error checking if task already exists:" << query.lastError().text();
        return false;
    }
}

int WindowAdmin::getTasksId(const int &employeeID, const int &adminId)
{
    QSqlQuery queryTask;
    QString command = "SELECT * FROM tasks WHERE assigned_to_employee_id = :employeeID AND assigned_by_admin_id = :adminId";
    queryTask.prepare(command);
    queryTask.bindValue(":employeeID", employeeID);
    queryTask.bindValue(":adminId", adminId);

    if (queryTask.exec() && queryTask.next()) {
        return queryTask.value("task_id").toInt();
    } else {
        qDebug() << "Error Task ID:" << queryTask.lastError().text();
        return -1;
    }
}
void WindowAdmin::on_btn_save_clicked()
{

    if (lineEdits.isEmpty() || dateEdits.isEmpty()) {
        qDebug() << "Пусті списки lineEdits або dateEdits";
        // ТУТ МОЖНА ВИВЕСТИ ЗАМІСТЬ ТИХ КНОПОК ТЕКСТ LABEL, ЩО НЕ ПРАЦІВНИКІВ ПОКИ ЩО НЕМАЄ
        return;
    }

    QString statusTask = "set";

    if (lineEdits.size() != dateEdits.size()) {
        qDebug() << "Різний розмір списків lineEdits та dateEdits";
        return;
    }

    for (int i = 0; i < lineEdits.size(); ++i)
    {
        qDebug()<<"\n";

        QString enteredText = lineEdits[i]->text();
        QDate selectedDate = dateEdits[i]->date();
        QDate defaultDate(2024, 1, 1);

        if(enteredText != "" && selectedDate != defaultDate){

            qDebug() << "Введені дані:" << enteredText << selectedDate;

            if (!isTaskAlreadyExists(enteredText))
            {
                QSqlQuery queryData;
                QString command = "INSERT INTO tasks (description, deadline, assigned_to_employee_id, assigned_by_admin_id, status) "
                                  "VALUES (:description, :deadline, :assigned_to_employee_id, :assigned_by_admin_id, :status)";
                queryData.prepare(command);
                queryData.bindValue(":description", enteredText);
                queryData.bindValue(":deadline", selectedDate);
                queryData.bindValue(":assigned_to_employee_id", selectedEmployeeId);
                queryData.bindValue(":assigned_by_admin_id", selectedAdminId);
                queryData.bindValue(":status", statusTask);

                if (queryData.exec()) {
                    qDebug() << "Запит INSERT виконано успішно!";
                } else {
                    qDebug() << "Помилка під час виконання запиту INSERT:" << queryData.lastError().text();
                }

            }

        }
    }
}

void WindowAdmin::on_btn_update_clicked()
{
    int TaskId = getTasksId(selectedEmployeeId, selectedAdminId);
    qDebug()<< TaskId;

    for (int i = 0; i < lineEdits.size(); ++i)
    {

        QString newText = lineEdits[i]->text();
        QDate newDate = dateEdits[i]->date();

        QSqlQuery queryUpdate;
        QString command = "UPDATE tasks SET description = :newText, deadline = :newDate WHERE assigned_to_employee_id = :selectedEmployeeId AND task_id = :TaskId";


        if (newText != "" && !isTaskAlreadyExists(newText))
        {
            qDebug() << "Введені дані:" << newText << newDate;

            queryUpdate.prepare(command);
            queryUpdate.bindValue(":newText", newText);
            queryUpdate.bindValue(":newDate", newDate);
            queryUpdate.bindValue(":selectedEmployeeId", selectedEmployeeId);
            queryUpdate.bindValue(":TaskId", TaskId);

            if (queryUpdate.exec())
            {
                qDebug() << "Запит update виконано успішно для тексту!";
            } else
            {
                qDebug() << "Помилка під час виконання запиту update:" << queryUpdate.lastError().text();
            }
            /// головна помилка : воно зберігає але не так як треба замість іншого тексту зберігає

        }


    }
}

void WindowAdmin::clearLayout(QLayout* layout)
{
    QLayoutItem* item;
    while ((item = layout->takeAt(0)) != nullptr)
    {
        delete item->widget();
        delete item;
    }
}

WindowAdmin::~WindowAdmin()
{
    db.CloseDatabase();
    delete ui;
}


void WindowAdmin::on_btnclose_clicked()
{
    this->close();
    Login * l = new Login();
    l->setWindowFlags(Qt::FramelessWindowHint);
    l->show();

}


