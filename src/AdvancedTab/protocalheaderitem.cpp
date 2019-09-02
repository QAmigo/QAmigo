#include "protocalheaderitem.h"

ProtocalHeaderItem::ProtocalHeaderItem(QByteArray header) :
    QStandardItem (QString("0x") + header.toHex().toUpper()),
    header(header)
{
}

QByteArray ProtocalHeaderItem::getHeader() const
{
    return header;
}
