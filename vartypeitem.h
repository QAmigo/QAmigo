#ifndef VARTYPE_H
#define VARTYPE_H

#include <QByteArray>
#include <QString>
#include <QListWidgetItem>

#include <decodeditem.h>

typedef enum {
    U8,
    I8,
    U16,
    I16,
    U32,
    I32,
    FLOAT,
    DOUBLE,
} VAR_TYPE;

typedef enum {
    BIG,
    LITTLE,
} ENDIANESS;

class VarTypeItem : public QListWidgetItem
{
public:
    VarTypeItem(QString nameType, VAR_TYPE type, QString name);

public:
    VAR_TYPE type;

public:
    int getSize();
    double getDouble(ENDIANESS endianess);
    void setBufferValue(const QByteArray &bufferValue);
    QString getName();

private:
    void fillValue(uint8_t *p, int byteCount, ENDIANESS endianess);

private:
    QByteArray bufferValue;
    double value;
    DecodedItem *item;
    QString name;
};

#endif // VARTYPE_H
