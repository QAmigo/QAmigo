#ifndef VARTYPE_H
#define VARTYPE_H

#include <QByteArray>
#include <QString>
#include <QtCharts>
#include <QLineSeries>

typedef enum {
    U8,
    I8,
    U16,
    I16,
    U32,
    I32,
    Float,
    Double,
} VAR_TYPE;

typedef enum {
    BIG,
    LITTLE,
} ENDIANESS;

class VarType
{
public:
    VarType(VAR_TYPE type);

public:
    int getSize();
    double getDouble(ENDIANESS endianess);
    void setBufferValue(const QByteArray &bufferValue);
    QString getName();

    QLineSeries *getSeries() const;
    void setSeries(QLineSeries *value);

    VAR_TYPE getType() const;

private:
    void fillValue(uint8_t *p, int byteCount, ENDIANESS endianess);

private:
    QByteArray bufferValue;
    double value;
    QString name;
    VAR_TYPE type;
};

#endif // VARTYPE_H
