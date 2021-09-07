#ifndef USER_H
#define USER_H

#include <QObject>
#include <QDataStream>

class User
{
public:
    User();
    explicit User(int id, QString name, QString password);
    ~User();

    friend QDataStream &operator<<(QDataStream & , const User &);
    friend QDataStream &operator>>(QDataStream & , User &);

    quint32 id() const;
    void setId(const quint32 &id);

    QString userName() const;
    void setUserName(const QString &userName);

    QString password() const;
    void setPassword(const QString &password);

private:
    quint32 m_id;
    QString m_userName;
    QString m_password;
};

#endif // USER_H
