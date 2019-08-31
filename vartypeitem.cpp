#include "vartypeitem.h"

VarTypeItem::VarTypeItem(QString nameType, VAR_TYPE type, QString name) :
    QListWidgetItem (name + '\t' + nameType),
    type(type),
    name(name)
{

}

int VarTypeItem::getSize()
{
    switch (type) {
    case VAR_TYPE::U8 :
        return 1;
    case VAR_TYPE::I8 :
        return 1;
    case VAR_TYPE::U16 :
        return 2;
    case VAR_TYPE::I16 :
        return 2;
    case VAR_TYPE::U32 :
        return 4;
    case VAR_TYPE::I32 :
        return 4;
    case VAR_TYPE::FLOAT :
        return sizeof(float);
    case VAR_TYPE::DOUBLE :
        return sizeof(double);
    }

    return 0;
}

void VarTypeItem::setBufferValue(const QByteArray &bufferValue)
{
    this->bufferValue = bufferValue;
}

QString VarTypeItem::getName()
{
    return name;
}

void VarTypeItem::fillValue(uint8_t *p, int byteCount, ENDIANESS endianess)
{
    for (int i = 0; i < byteCount; i++) {
        if (endianess == LITTLE)
            *p++ = static_cast<uint8_t>(bufferValue[i]);
        else
            *p++ = static_cast<uint8_t>(bufferValue[byteCount - i - 1]);
    }
}

double VarTypeItem::getDouble(ENDIANESS endianess)
{
    uint8_t *p;
    switch (type) {
    case VAR_TYPE::U8 :
        return static_cast<double>(static_cast<uint8_t>(bufferValue[0]));
    case VAR_TYPE::I8 :
        return static_cast<double>(static_cast<int8_t>(bufferValue[0]));
    case VAR_TYPE::U16 :
        uint16_t u16;
        p = reinterpret_cast<uint8_t *>(&u16);
        fillValue(p, 2, endianess);
        return static_cast<double>(u16);
    case VAR_TYPE::I16 :
        int16_t i16;
        p = reinterpret_cast<uint8_t *>(&i16);
        fillValue(p, 2, endianess);
        return static_cast<double>(i16);
    case VAR_TYPE::U32 :
        uint32_t u32;
        p = reinterpret_cast<uint8_t *>(&u32);
        fillValue(p, 4, endianess);
        return static_cast<double>(u32);
    case VAR_TYPE::I32 :
        int32_t i32;
        p = reinterpret_cast<uint8_t *>(&i32);
        fillValue(p, 4, endianess);
        return static_cast<double>(i32);
    case VAR_TYPE::FLOAT :
        //In Qt, all float should be IEEE defined 32-bit.
        float f;
        p = reinterpret_cast<uint8_t *>(&f);
        fillValue(p, 4, endianess);
        return static_cast<float>(f);
    case VAR_TYPE::DOUBLE :
        //Assume double is 64-bit now.
        double d;
        p = reinterpret_cast<uint8_t *>(&d);
        fillValue(p, 8, endianess);
        return d;
    }

    return 0;
}
