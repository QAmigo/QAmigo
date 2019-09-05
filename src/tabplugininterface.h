#ifndef TABPLUGININTERFACE_H
#define TABPLUGININTERFACE_H

#include <QWidget>

class TabPluginInterface : public QWidget
{
public:
    virtual ~TabPluginInterface() {}
    virtual QLayout *getLayout() = 0;
    virtual QString getName() const = 0;
    virtual void onFrameUpdated(int id, QList<double> listValues) = 0;
    virtual void setConnection(QIODevice *connection) = 0;
};

QT_BEGIN_NAMESPACE

#define TabPluginInterface_iid "org.UESTC.SongQiang.TabPluginInterface"

Q_DECLARE_INTERFACE(TabPluginInterface, TabPluginInterface_iid)

QT_END_NAMESPACE

#endif // TABPLUGININTERFACE_H
