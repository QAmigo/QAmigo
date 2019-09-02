#ifndef PROTOCALHEADERITEM_H
#define PROTOCALHEADERITEM_H

#include <QStandardItem>

class ProtocalHeaderItem : public QStandardItem
{
public:
    ProtocalHeaderItem(QByteArray header);

    QByteArray getHeader() const;

private:
    QByteArray header;
};

#endif // PROTOCALHEADERITEM_H
