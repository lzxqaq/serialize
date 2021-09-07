#ifndef USERDAO_H
#define USERDAO_H

#include <QObject>
#include "data/user.h"

class UserDao
{
public:
    User select(quint32 id);
    QVector<User> selectAll();

    bool insert(const User &user);
    bool insert(const QVector<User> &users);

    bool update(const User &user);
    bool update(const QVector<User> &users);

    bool remove(const User &user);
    bool remove(const QVector<User> &users);
};

#endif // USERDAO_H
