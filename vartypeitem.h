#ifndef VARTYPE_H
#define VARTYPE_H

#include <QString>
#include <QListWidgetItem>

typedef enum {
    U8,
    I8,
    U16,
    I16,
    U32,
    I32,
    U64,
    I64,
    FLOAT,
    DOUBLE,
} VAR_TYPE;

class VarTypeItem : public QListWidgetItem
{
public:
    VarTypeItem(QString string, VAR_TYPE type);

public:
    VAR_TYPE type;
};

#endif // VARTYPE_H
