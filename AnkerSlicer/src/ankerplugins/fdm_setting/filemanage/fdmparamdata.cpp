#include "fdmparamdata.h"
#include <QDebug>

//#include "fdmparaminit.h"
#include "common/utilities/tlogger.h"

using namespace fdmsettings;
using namespace shadowtree;
using NodeType = FdmCppParamNode;


QList<NodeType *> FdmCppParamNode::allParamItem(){
    QList<NodeType *> nodeList = getRootNode()->allChildrenNode(Qt::FindChildrenRecursively);
    QList<NodeType *> itemList;

    for(NodeType * it : nodeList){
        if(it->hasProperty(fdmPrinterJsonIcon)){
            itemList.append(it);
            continue;
        }
        if(it->hasProperty(fdmPrinterJsonDefaultValue)){
            itemList.append(it);
            continue;
        }
    }
    return itemList;
}

QVariant::Type FdmCppParamNode::getFdmType() {
    const static QString TypeFloat  ("float");
    const static QString TypeInt    ("int");
    QString typeStr = getProperty(fdmsettings::fdmPrinterJsonType).toString();
    if(typeStr == TypeFloat){
        return QVariant::Double;
    }
    else if(typeStr == TypeInt){
        return QVariant::Int;
    }

    return getProperty(fdmPrinterJsonDefaultValue).type();
}



void FdmCppParamNode::nodeValueChange_fromCpp(QVariant rawtext)
{
    QVariant::Type type = getFdmType();
    
    bool ok = true;
    if (type == QVariant::Type::Int){
       rawtext = int(rawtext.toDouble());
    }
    else {
        ok = rawtext.convert(type);
    }

    if(ok){
        nodeValueChange_toTree(rawtext);
    }
}


void FdmCppParamNode::nodeValueChange_fromUI(QVariant rawtext)
{
    AkUtil::TFunction(QString("{%1:%2->%3}").arg(getNodeName()).arg(getFdmValue().toString()).arg(rawtext.toString()));
    qDebug() << "nodeValueChange_fromUI " << rawtext;
    QVariant::Type type = getFdmType();
     
    bool ok = true;
    if (type == QVariant::Type::Int) {
        rawtext = int(rawtext.toDouble());
    }
    else {
        ok = rawtext.convert(type);
    }

    if(ok){
        emit fdmValueChangeByUI(rawtext);
        nodeValueChange_toTree(rawtext);
    }

}



void FdmCppParamNode::nodeValueChange_fromTree(QVariant logicValue)
{
    
    emit nodeValueChange_toUI(getProperty(fdmNodeRowInModelList));
}


void FdmCppParamNode::nodeValueChange_toTree(QVariant setValue)
{
    
    setQobjProperty("fdmValue", setValue);
}


bool FdmCppParamNode::eventFilter(QObject *watched, QEvent *event){
    //if(watched == (QObject *)&hashData())
    if (event->type() == QEvent::DynamicPropertyChange) {
        auto e = static_cast<QDynamicPropertyChangeEvent*>(event);
        const QByteArray key = e->propertyName();
        QVariant value = getStoreProperty(key);

        if(0){
            static int aa = 0;++aa;
            QString qobjlog;
            {QDebug(&qobjlog) << aa << watched << getNodeName();}
            qobjlog = qobjlog.leftJustified((qobjlog.size() / 16 + 1) * 16);
            qDebug().noquote().nospace() << qobjlog << "eventFilter( " << key.leftJustified(16) << " , " << value << ")";
        }
        if(key == fdmPrinterJsonEnabled){
            if(value.toBool() == false || isTheCategoryExpand()){ //  OK  s2022Äê1ÔÂ26ÈÕ 09:34:16
                setProperty(fdmNodeIsVisibility, value);
            }
            nodeValueChange_fromTree(QVariant());
        }
        if(key == fdmPrinterJsonValue){
            nodeValueChange_fromTree(value);
            emit fdmValueChange(value); // add by Chunlian @2021-12-16
        }
        
        //  fdmValueError: (fdmMinimumValue < fdmMaximumValue) && (fdmValue < fdmMinimumValue || fdmMaximumValue < fdmValue)
        //  fdmValueWarning: (fdmMinimumValueWarning < fdmMaximumValueWarning) && (fdmValue < fdmMinimumValueWarning || fdmMaximumValueWarning < fdmValue)
        //if(key == fdmPrinterJsonValueError || key == fdmPrinterJsonValueWarning){
        //    this->dumpParam();
        //}
    }

    return true;
}

void FdmCppParamNode::dumpParam(int t)
{
    QString _temp;
    QString *dbgStr = &_temp;
    {
        QDebug debug(dbgStr); debug.nospace().noquote();
        QString _table    = QByteArray(t, ' ');
        QString _nodename = "NodeName: " + this->getNodeName();                             _nodename = _nodename.leftJustified(32);
        debug << (_table + _nodename).leftJustified(72);

        // this qobjMeta()
        QString _children = "Children: " + QString::number(qobjReal()->children().size());              _children = _children.leftJustified(12); debug << _children;
        QString _metaData = "meta: " + QString::number(qobjReal()->metaObject()->propertyCount());      _metaData = _metaData.leftJustified(12); debug << _metaData;
        QString _dynamic  = "dyna: " + QString::number(qobjReal()->dynamicPropertyNames().size());      _dynamic  = _dynamic .leftJustified(12); debug << _dynamic;
        QString _property = "hash: " + QString::number(storeData().size());                              _property = _property.leftJustified(12); debug << _property;

        int width = 16;
        for(const auto &  keyIt : storeData().keys()){
            if(keyIt.length() > width){
                width = keyIt.length() / 4 * 4;
            }
        }width +=  4;

        if(Qobj_MetaPropertyCount(qobjReal()) > 0){
            static QList<QByteArray> skip = { "parent", "data", "resources", "children", "x", "y", "z", "width", "height", "opacity", "enabled", "visible", "visibleChildren", "states", "transitions", "state", "childrenRect", "anchors", "left", "right", "horizontalCenter", "top", "bottom", "verticalCenter", "baseline", "baselineOffset", "clip", "focus", "activeFocus", "activeFocusOnTab", "rotation", "scale", "transformOrigin", "transformOriginPoint", "transform", "smooth", "antialiasing", "implicitWidth", "implicitHeight", "containmentMask", "layer"};
            debug << "\n" << QByteArray(t+8, ' ') << _metaData;
            QList<QByteArray> keys = Qobj_MetaPropertyKeys(qobjReal());
            for(const auto & keyIt : keys){
                if(skip.contains(keyIt)){continue;}
                QByteArray  key   = keyIt;
                QVariant    value = getQobjProperty(key);

                debug << "\n" << QByteArray(t+10, ' ');
                debug << key.leftJustified(width) << value;
            }
        }

        //  start fit dump bug @2022-10-08 by CL
        if(false && Qobj_DynamicPropertyCount(qobjReal()) > 0){
            debug << "\n" << QByteArray(t+8, ' ') << _dynamic;
            int count = Qobj_DynamicPropertyCount(qobjReal());
            QList<QByteArray> keys = Qobj_DynamicPropertyKeys(qobjReal());
            for(int i=0; i < count; ++i){
                QByteArray  key   = keys[i];
                QVariant    value = getQobjProperty(key);

                debug << "\n" << QByteArray(t+10, ' ');
                debug << key.leftJustified(width) << value;
            }
        }

        if(storeData().size() > 0){
            debug << "\n" << QByteArray(t+8, ' ') << _property;

            static QList<QString> skip = {}; //{ fdmPrinterJsonValue, fdmPrinterJsonValueWork, fdmPrinterJsonDefaultValue, fdmPrinterJsonDefaultValueWork };

            for(auto keyIt : storeData().keys()){
                if(skip.contains(keyIt)){continue;}
                QByteArray  key   = keyIt;
                QVariant    value = storeData().get(keyIt);

                debug << "\n" << QByteArray(t+10, ' ');
                debug << key.leftJustified(width) << value;
            }

            debug << "\n\n";
        }
        //  end fit dump bug @2022-10-08 by CL
    }
    qDebug().noquote().nospace() << *dbgStr;
}

void FdmCppParamNode::dumpParamTree(){
    std::function<void(NodeType *, int)>func;
    func = [&](NodeType * node, int t){
        node->dumpParam(t);
        if(node->childrenCount() > 0){
            for(NodeType * childNode : node->allChildrenNode()){
                func(childNode, t+4);
            }
        }
    };
    func(this, 0);
}
