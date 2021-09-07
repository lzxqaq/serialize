#include "userdao.h"
#include <QFile>
#include <QDebug>


const QString fileName = "user.dat";


bool UserDao::insert(const User &user)
{
    QFile  file(fileName);
    if  (!file.open(QFile::Append)) {
        qDebug() << QString::fromLocal8Bit("\n文件打开失败");
        return false;
    }

    QDataStream stream(&file);
    stream << user;
    file.close();
    return true;
}

bool UserDao::insert(const QVector<User> &users)
{
    QFile  file(fileName);
    if  (!file.open(QFile::Append)) {
        qDebug() << QString::fromLocal8Bit("\n文件打开失败");
        return false;
    }

    QDataStream stream(&file);
    foreach (auto user, users)
    {
        stream << user;
    }
    file.close();
    return true;
}

User UserDao::select(quint32 id)
{
    User user;
    QFile  file(fileName);
    if  (!file.open(QFile::ReadOnly)) {
        qDebug() << QString::fromLocal8Bit("\n文件打开失败");
        return user;
    }

    QDataStream stream(&file);

    bool flag = false;

    while (!stream.atEnd())
    {
        stream >> user;
        if (id == user.id())
        {
            flag = true;
            break;
        }
    }
    file.close();

    if (flag)
    {
        return user;
    }
    else
    {
        return User();
    }
}

QVector<User> UserDao::selectAll()
{
    QVector<User> users;
    QFile  file(fileName);
    if  (!file.open(QFile::ReadOnly)) {
        qDebug() << QString::fromLocal8Bit("\n文件打开失败");
        return users;
    }

    QDataStream stream(&file);

    stream.resetStatus();
    while (!stream.atEnd())
    {
        User user;
        stream >> user;
        users.append(user);
    }
    file.close();

    return users;
}


