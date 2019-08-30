#ifndef VARTYPE_H
#define VARTYPE_H

#include <QByteArray>
#include <QString>
#include <QListWidgetItem>

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
    VarTypeItem(QString string, VAR_TYPE type);

public:
    VAR_TYPE type;

public:
    int getSize();
    double getDouble(ENDIANESS endianess);
    void setBufferValue(const QByteArray &bufferValue);

private:
    void fillValue(uint8_t *p, int byteCount, ENDIANESS endianess);

private:
    QByteArray bufferValue;
    double value;
};

#endif // VARTYPE_H
