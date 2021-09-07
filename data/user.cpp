#include "user.h"
#include <QFile>
#include <QDataStream>
#include <QtDebug>

User::User()
{

}

User::User(int id, QString name, QString password)
    : m_id(id)
    , m_userName(name)
    , m_password(password)
{

}

User::~User()
{

}

QDataStream& operator<<(QDataStream &out, const User &user)
{
    out << user.m_id
        << user.m_userName
        << user.m_password;
    return out;
}

QDataStream& operator>>(QDataStream &in, User &user)
{
    in >> user.m_id
       >> user.m_userName
       >> user.m_password;
    return in;
}


quint32 User::id() const
{
    return m_id;
}

void User::setId(const quint32 &id)
{
    m_id = id;
}

QString User::userName() const
{
    return m_userName;
}

void User::setUserName(const QString &userName)
{
    m_userName = userName;
}

QString User::password() const
{
    return m_password;
}

void User::setPassword(const QString &password)
{
    m_password = password;
}
