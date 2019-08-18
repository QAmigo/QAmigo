#include "vartypeitem.h"

VarTypeItem::VarTypeItem(QString string, VAR_TYPE type) :
    QListWidgetItem (string),
    type(type)
{

}
