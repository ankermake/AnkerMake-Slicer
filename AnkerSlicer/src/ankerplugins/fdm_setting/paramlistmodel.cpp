#include "paramlistmodel.h"
#include <QTimer>
#include <QObject>
//#define TEST
namespace fdmsettings {

ParamListModel::ParamListModel(QObject *parent)
    : QAbstractListModel(parent)
{
}

int ParamListModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    return m_datas.count();
}

QVariant ParamListModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || index.row() < 0 || index.row() >= m_datas.size()) {
        return QVariant();
    }
    FdmParamNode *item =m_datas.at( index.row());
    if(!item) {
        return QVariant();
    }
    
    if(role == datatype::value){
        return item->getFdmValue();
    }
    
    if(role == datatype::visible){
        bool v = item->getFdmEnable() && item->getProperty(roleToString(role)).toBool();
        return QVariant(v);
    }
    QString  str = roleToString(role);
   //item->dumpParam();
    return  item->getProperty(str);
}

QHash<int, QByteArray> ParamListModel::roleNames() const
{
    QHash<int, QByteArray>  d;
    d[datatype::label] = "label";
    d[datatype::value] = "value";
    d[datatype::visible] = "visible";
    d[datatype::level] = "level";
    d[datatype::type] = "type";
    d[datatype::unit] = "unit";
    d[datatype::options] = "options";
    d[datatype::expand] = "expand";
    d[datatype::icon] = "icon";
    d[datatype::description] = "description";
    d[datatype::affects] = "affects";
    d[datatype::affectedBy] = "affectedBy";
    d[datatype::minimumValue] = "minimumValue";
    d[datatype::maximumValue] = "maximumValue";
    d[datatype::minimumValueWarning] = "minimumValueWarning";
    d[datatype::maximumValueWarning] = "maximumValueWarning";

    return  d;
}

void ParamListModel::changeData(int row,QVariant text)
{
     AkUtil::TFunction("");
    if(row > m_datas.size()) {
        return;
    }
    FdmParamNode *item =  m_datas.at(row);
    QVariant value =  item->getProperty(roleToString(datatype::value));

    QVariant type = item->getProperty(roleToString(datatype::type));
    QString typeString = type.toString();
    if(typeString == "bool") {
        if(value.toBool() ==  text.toBool() ){
            return;
        }
    }
    else if(typeString =="enum") {
        if(value.toString()== text.toString()) {
            return;
        }

    
    }else if(typeString =="int") {
        if(value.toInt() == text.toInt()) {
            return;
        }
    }else if(typeString =="float") {
        if(qFuzzyCompare( value.toDouble(), text.toDouble() )) {
            return;
        }
    } else {
        //qDebug() << typeString << value << text;
    }

    item->nodeValueChange_fromUI(text);
}

void ParamListModel::expandIndex(int row)
{
    if(row > m_datas.size()) {
        return;
    }
    setChildNodeVisibility(row,true);
}

void ParamListModel::collapseIndex(int row)
{
    if(row > m_datas.size()) {
        return;
    }
    setChildNodeVisibility(row,false);
}

void ParamListModel::updateList()
{
    beginResetModel();
    endResetModel();
}


bool ParamListModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    return false;
}

Qt::ItemFlags ParamListModel::flags(const QModelIndex &index) const
{
    return  QAbstractListModel::flags(index);
}

void ParamListModel::setListDatas(const QList<FdmParamNode *> &datas)
{
    beginResetModel();
    m_datas.clear();
    m_datas.append(datas);
    for(int row=0; row<datas.size(); ++row){
        datas[row]->insert(fdmNodeRowInModelList, row);
        QObject::connect(datas[row], &FdmParamNode::nodeValueChange_toUI,
                         this, &ParamListModel::onNodeValueChange,
                         Qt::DirectConnection);
    }

    endResetModel();
}

QString ParamListModel::roleToString(int role) const
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
    case (datatype::options):
        return fdmPrinterJsonOptions;
    case (datatype::expand):
        return fdmNodeExpand;
    case(datatype::icon):
        return fdmNodeIcon;
    case(datatype::description):
        return fdmPrinterJsonDescription;
    case(datatype::affects):
        return fdmPrinterAffects;
    case(datatype::affectedBy):
        return fdmPrinterAffectedBy;
    case(datatype::minimumValue):
        return fdmPrinterJsonMinValue;
    case(datatype::maximumValue):
        return fdmPrinterJsonMaxValue;
    case(datatype::minimumValueWarning):
        return fdmPrinterJsonMinValueWarning;
    case(datatype::maximumValueWarning):
        return fdmPrinterJsonMaxValueWarning;
    default:
        return QString();

    }
}

void ParamListModel::setChildNodeVisibility(int row, bool visbility)
{

    FdmParamNode *node =  m_datas.at(row);
    if(node->getProperty(fdmPrinterJsonType) != "category") {
        return;
    }
    node->setProperty(fdmNodeExpand,visbility);
#ifdef TEST
    //  test by Chunlian @2021-12-16
    int top = row, bottom;
    for(bottom = top +1 ; bottom < m_datas.size(); bottom++){
        FdmParamNode *item =  m_datas.at(bottom);
        if(item->getProperty(fdmPrinterJsonType) == "category") {
            break;
        }
        item->setProperty(fdmNodeIsVisibility,visbility);
    }
    bottom--;
    QModelIndex topLeft     = createIndex(top, 0);
    QModelIndex bottomRight = createIndex(bottom, 0);
    emit dataChanged(topLeft, bottomRight, QVector<int>()  << datatype::visible);
    return;
#endif
    for(int i = row + 1 ; i < m_datas.size(); i++)
    {
        FdmParamNode *item =  m_datas.at(i);
        if(item->getProperty(fdmPrinterJsonType) == "category") {
            break;
        }

        
        //  NG  bool visbilityWithEnable = visbility && item->getProperty(fdmPrinterJsonEnble).toBool();
        bool visbilityWithEnable = visbility && item->getFdmEnable();
        item->setProperty(fdmNodeIsVisibility,visbilityWithEnable);
         emit dataChanged(createIndex(i,0),createIndex(i,0),QVector<int>() << datatype::visible);
        //item->dumpParam();
    }
    return;
}

/**
 * @brief ParamListModel::onNodeValueChange
 * @param row
 * TODO:

 */
void ParamListModel::onNodeValueChange(QVariant row){
    int r = row.toInt();
    if(r < 0 || r  >m_datas.size()) {
        return;
    }

    
    //if(m_datas.at(r)->getNodeName().compare("acceleration_"))
    //    qDebug() << "    "  << m_datas[r]->getNodeName() << m_datas.at(r)->getFdmEnable() << row;
    //qDebug() << "    " << __LINE__ << __FUNCTION__ << row;
    emit dataChanged(createIndex(r,0),createIndex(r,0),QVector<int>() << datatype::label << datatype::value << datatype::visible);

}



}
