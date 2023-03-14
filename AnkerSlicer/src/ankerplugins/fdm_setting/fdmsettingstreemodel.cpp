#include "fdmsettingstreemodel.h"

namespace fdmsettings{
FdmSettingsTreeModel::FdmSettingsTreeModel(QObject *parent)
    : QAbstractItemModel(parent)
{
}

FdmSettingsTreeModel::~FdmSettingsTreeModel()
{

}

//QVariant FdmSettingsTreeModel::headerData(int section, Qt::Orientation orientation, int role) const
//{
//    // FIXME: Implement me!
//    return  QVariant();
//}

QModelIndex FdmSettingsTreeModel::index(int row, int column, const QModelIndex &parent) const
{
    if(hasIndex(row,column,parent)) {
        return QModelIndex();
    }
    FdmParamNode *parentNode;
    if(!parent.isValid()) {
        parentNode = m_root;
    }else {
        parentNode =indexToNode(parent);
    }
    FdmParamNode *childItem = parentNode->childAt(row);
    if(childItem) {
        return createIndex(row,1,childItem);
    }
    return QModelIndex();
}

QModelIndex FdmSettingsTreeModel::parent(const QModelIndex &index) const
{
    if(!index.isValid()) {
        return QModelIndex();
    }
    FdmParamNode *child = indexToNode(index);
    if(!child) {
        return QModelIndex();
    }
    FdmParamNode *parent =child->getParentNode();
    if(parent) {
        return createIndex(parent->row(),1,parent);
    }
    return QModelIndex();
}

int FdmSettingsTreeModel::rowCount(const QModelIndex &parent) const
{
    FdmParamNode * parentNode;
    if (!parent.isValid()) {
        parentNode = m_root;
    } else {
        parentNode = indexToNode(parent);
        if(!parentNode) {
            return 0;
        }
    }
    return parentNode->childCount();
}

int FdmSettingsTreeModel::columnCount(const QModelIndex &parent) const
{
    // FIXME: Implement me!
    return 1;
}

//bool FdmSettingsTreeModel::hasChildren(const QModelIndex &parent) const
//{
//    // FIXME: Implement me!
//    return true;
//}

//bool FdmSettingsTreeModel::canFetchMore(const QModelIndex &parent) const
//{
//    // FIXME: Implement me!
//    return false;
//}

//void FdmSettingsTreeModel::fetchMore(const QModelIndex &parent)
//{
//    // FIXME: Implement me!
//}

QVariant FdmSettingsTreeModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();
    auto node = indexToNode(index);

   // qDebug() << " data = " <<node->getProperty(roleToString(role));
    //node->dumpParam();
    return  node->getProperty(roleToString(role));
}

QHash<int, QByteArray> FdmSettingsTreeModel::roleNames() const
{
    QHash<int, QByteArray>  d;
    d[datatype::label] = "label";
    d[datatype::value] = "value";
    d[datatype::visible] = "visible";
    d[datatype::level] = "level";
    d[datatype::type] = "type";
    d[datatype::unit] = "unit";

    return  d;
}

void FdmSettingsTreeModel::setRootNode(FdmParamNode *root)
{
    m_root = root;
}

QString FdmSettingsTreeModel::roleToString(int role) const
{
    QHash<int, QByteArray> roles = roleNames();
    if(roles.isEmpty()) {
        return QString();
    }
    QString str = roles.value(role);
    if(str.isEmpty()) {
        return QString();
    }
    switch (role) {
    case (datatype::label):
        return fdmPrinterJsonLabel;
    case (datatype::value):
        return fdmPrinterJsonValue;
    case (datatype::visible):
        return fdmNodeIsVisibility;
    case (datatype::level):
        return fdmNodeLevel;
    case (datatype::type):
        return fdmPrinterJsonType;
    case (datatype::unit):
        return fdmPrinterJsonUnit;
    default:
        return QString();
    }
}

FdmParamNode *FdmSettingsTreeModel::indexToNode(const QModelIndex &index) const
{
    if(!index.isValid()) {
        return  nullptr;
    }
   FdmParamNode *node = static_cast<FdmParamNode *>(index.internalPointer());
   if(node) {
       return node;
   }
   return nullptr;
}

QModelIndex FdmSettingsTreeModel::nodeToIndex(FdmParamNode *node) const
{
    return QModelIndex();
}
}
