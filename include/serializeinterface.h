#ifndef SERIALIZE_H
#define SERIALIZE_H

#include <QObject>
#include <QDataStream>

class SerializeInterface
{
public:

    /**
    * @brief 用来把类对象进行二进制方式序列化的函数。
    * @param[in] fileName 文件名。
    * @return 执行结果
    */
    virtual bool serializeBinary(const QString &fileName) const = 0;

    /**
     * @brief 用来把类对象进行二进制方式序列化的函数。本接口的实现需要设置字节顺序
     * @param[in] stream 文件流对象。
     * @return 执行结果
     */
    virtual bool serializeBinary(QDataStream& stream) const = 0;

    /**
    * @brief 用来把类对象进行二进制方式反序列化的函数。
    * @param[in] fileName 文件名。
    * @return 执行结果
    */
    virtual bool deSerializeBinary(const QString& fileName) = 0;
    /**
    * @brief 用来把类对象进行二进制方式序列化的函数。本接口的实现需要与序列化保存设置相同的字节顺序
    * @param[in] stream 文件流对象。
    * @return 执行结果
    */
    virtual bool deSerializeBinary(QDataStream& ds) = 0;
};

#endif // SERIALIZE_H
