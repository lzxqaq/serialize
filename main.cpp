#include <QCoreApplication>
#include <QFile>
#include <QDateTime>
#include <QDebug>
#include <QTextCodec>

#include "data/user.h"
#include "dao/userdao.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    int i = 0;
    QVector<User> users;
    for (i = 0; i < 1000000; i++)
    {
        User user(i, "name" + QString::number(i), "field");
        users.append(user);
    }
    UserDao dao;
    QDateTime begin;
    QDateTime end;

    {
        begin = QDateTime::currentDateTime();

        dao.insert(users);

        end = QDateTime::currentDateTime();

        qDebug() << "插入 " << users.size() << "个数据，耗时（毫秒） " << begin.msecsTo(end);

    }

    {
        User user(i, "name" + QString::number(i), "field");

        begin = QDateTime::currentDateTime();

        dao.insert(user);

        end = QDateTime::currentDateTime();
        qDebug() << "插入单个数据，耗时（毫秒） " << begin.msecsTo(end);
    }

    QVector<User> selectUsers;
    {
        begin = QDateTime::currentDateTime();

        selectUsers = dao.selectAll();

        end = QDateTime::currentDateTime();

        qDebug() << "查询所有数据，耗时（毫秒） " << begin.msecsTo(end);

    }

    {
        begin = QDateTime::currentDateTime();

        dao.select(50);

        end = QDateTime::currentDateTime();

        qDebug() << "查询单个数据，耗时（毫秒） " << begin.msecsTo(end);

    }

    return a.exec();
}
