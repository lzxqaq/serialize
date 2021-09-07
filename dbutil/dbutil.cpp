#include "dbutil.h"
#include "ConnectionPool"
#include "dbutilconfig.h"

DBUtil::DBUtil()
{
    QSqlDatabase db = ConnectionPool().getConnection()->database();
    m_query = new QSqlQuery(db);
}

int DBUtil::insert(const QString &sql, const QVariantMap &params) {
    int id = -1;

    executeSql(sql, params, [&id](QSqlQuery *query) {
        id = query->lastInsertId().toInt(); // 插入行的主键
    });

    return id;
}

bool DBUtil::insertBatch(const QString &sql, const QList<QVariantMap> &params)
{
    bool flag = false;

    executeBatchSql(sql, params, [&flag](QSqlQuery *query) {
        flag = query->lastError().type() == QSqlError::NoError;
    });
    return flag;
}

bool DBUtil::update(const QString &sql, const QVariantMap &params) {
    bool result;

    executeSql(sql, params, [&result](QSqlQuery *query) {
        result = query->lastError().type() == QSqlError::NoError;
    });

    return result;
}

bool DBUtil::updateBatch(const QString &sql, const QList<QVariantMap> &params)
{
    bool result;

    executeBatchSql(sql, params, [&result](QSqlQuery *query) {
        result = query->lastError().type() == QSqlError::NoError;
    });

    return result;
}

int DBUtil::selectInt(const QString &sql, const QVariantMap &params) {
    return selectVariant(sql, params).toInt();
}

qint64 DBUtil::selectInt64(const QString &sql, const QVariantMap &params) {
    return selectVariant(sql, params).toLongLong();
}

QString DBUtil::selectString(const QString &sql, const QVariantMap &params) {
    return selectVariant(sql, params).toString();
}

QDate DBUtil::selectDate(const QString &sql, const QVariantMap &params) {
    return selectVariant(sql, params).toDate();
}

QDateTime DBUtil::selectDateTime(const QString &sql, const QVariantMap &params) {
    return selectVariant(sql, params).toDateTime();
}

QVariant DBUtil::selectVariant(const QString &sql, const QVariantMap &params) {
    QVariant result;

    executeSql(sql, params, [&result](QSqlQuery *query) {
        if (query->next()) {
            result = query->value(0);
        }
    });

    return result;
}

bool DBUtil::transaction()
{
    QSqlDatabase db = ConnectionPool().getConnection()->database();
    return db.transaction();
}

bool DBUtil::commit()
{
    QSqlDatabase db = ConnectionPool().getConnection()->database();
    return db.commit();
}

bool DBUtil::roolback()
{
    QSqlDatabase db = ConnectionPool().getConnection()->database();
    return db.rollback();
}

QString DBUtil::lastError()
{
    QString result;
    if (m_query->lastError().type() != QSqlError::NoError)
    {
        result = m_query->lastError().text().trimmed();
    }
    return result;
}

QStringList DBUtil::selectStrings(const QString &sql, const QVariantMap &params)
{
    QStringList strings;

    executeSql(sql, params, [&strings](QSqlQuery *query) {
        while (query->next()) {
            strings.append(query->value(0).toString());
        }
    });

    return strings;
}

QVariantMap DBUtil::selectMap(const QString &sql, const QVariantMap &params)
{
    return selectMaps(sql, params).value(0);
}

QList<QVariantMap> DBUtil::selectMaps(const QString &sql, const QVariantMap &params)
{
    QList<QVariantMap> maps;

    executeSql(sql, params, [&maps, this](QSqlQuery *query) {
        maps = queryToMaps(query);
    });

    return maps;
}

void DBUtil::bindValues(QSqlQuery *query, const QVariantMap &params)
{
    for (QVariantMap::const_iterator i=params.constBegin(); i!=params.constEnd(); ++i)
    {
        query->bindValue(":" + i.key(), i.value());
    }
}

void DBUtil::bindBatchValues(QSqlQuery *query, const QList<QVariantMap> &params)
{
    foreach (auto param, params)
    {
        for (QVariantMap::const_iterator i=param.constBegin(); i!=param.constEnd(); ++i)
        {
            query->bindValue(":" + i.key(), i.value());
        }
    }
}

QStringList DBUtil::getFieldNames(const QSqlQuery &query)
{
    QSqlRecord record = query.record();
    QStringList names;
    int count = record.count();

    for (int i = 0; i < count; ++i) {
        names << record.fieldName(i);
    }

    return names;
}

QList<QVariantMap > DBUtil::queryToMaps(QSqlQuery *query)
{
    QList<QVariantMap > rowMaps;
    QStringList fieldNames = getFieldNames(*query);

    while (query->next()) {
        QVariantMap rowMap;

        foreach (const QString &fieldName, fieldNames)
        {
            rowMap.insert(fieldName, query->value(fieldName));
        }

        rowMaps.append(rowMap);
    }

    return rowMaps;
}

void DBUtil::debug(const QSqlQuery &query, const QVariantMap &params)
{
    if (DbUtilConfig::instance().getDebug())
    {
        if (query.lastError().type() != QSqlError::NoError)
        {
            qDebug().noquote() << "==> SQL Error: " << query.lastError().text().trimmed();
        }

        qDebug().noquote() << "==> SQL Query:" << query.lastQuery();

        if (params.size() > 0) {
            qDebug().noquote() << "==> SQL Params: " << params;
        }
    }
}

void DBUtil::debug(const QSqlQuery &query)
{
    if (DbUtilConfig::instance().getDebug())
    {
        if (query.lastError().type() != QSqlError::NoError)
        {
            qDebug().noquote() << "==> SQL Error: " << query.lastError().text().trimmed();
        }

        qDebug().noquote() << "==> SQL Query:" << query.lastQuery();
    }
}


template<typename T>
QList<T> DBUtil::selectBeans(const T &mapToBean(const QVariantMap &), const QString &sql, const QVariantMap &params)
{
    QList<T> beans;

    // 每一个 map 都映射成一个 bean 对象
    foreach (const QVariantMap &row, selectMaps(sql, params)) {
        beans.append(mapToBean(row));
    }

    return beans;
}

template<typename T>
void DBUtil::executeSql(const QString &sql, const QVariantMap &params, const T &t)
{

    m_query->setForwardOnly(true);//结果集仅向前，可以更有效地利用内存，它还将提高某些数据库的性能
    m_query->prepare(sql);
    bindValues(m_query, params);

    if (m_query->exec()) {
        t(m_query);
    }
    debug(*m_query, params);
}

template<typename T>
void DBUtil::executeBatchSql(const QString &sql, const QList<QVariantMap> &params, const T &t)
{

    m_query->setForwardOnly(true);//结果集仅向前，可以更有效地利用内存，它还将提高某些数据库的性能
    m_query->prepare(sql);
    bindBatchValues(m_query, params);

    if (m_query->execBatch()) {
        t(m_query);
    }
    debug(*m_query);
}
