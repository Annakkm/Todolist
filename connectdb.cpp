#include "connectdb.h"

ConnectDB::ConnectDB()
{

}

bool ConnectDB::OpenDatabase()
{
    db =QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName("localhost");
    db.setPassword("mysql1mysql");
    db.setUserName("Ann");
    db.setDatabaseName("todolist");

    if(!db.open()){
        qDebug()<<"Not Connected db";

        return false;
    }
    else {
        qDebug()<<"Connected db";

        return true;
    }
}

void ConnectDB::CloseDatabase()
{
    db.close();
    QSqlDatabase::removeDatabase(QSqlDatabase::defaultConnection);
}


