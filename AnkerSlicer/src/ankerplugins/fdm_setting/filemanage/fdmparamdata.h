#ifndef FDMPARAMDATA_H
#define FDMPARAMDATA_H

#include <QObject>
#include <QVariant>
#include "fdmSettingsCommon.h"
#include "shadowtreetemplate.hpp"
#include "qmljsonsetting.hpp"

/*================ class FdmCppParamNode ================*/
class FdmCppParamNode : public QObject, public shadowtree::CppItemNodeApi<FdmCppParamNode>{
    Q_OBJECT

    using QObject::setProperty;
    using QObject::property;
public:
    explicit FdmCppParamNode(){}

    using NodeApi::getNodeName;
    using NodeApi::setNodeName;

    using NodeApi::hasProperty;
    using NodeApi::getProperty;
    using NodeApi::setProperty;
    using NodeApi::insert;

    using NodeApi::checkNodeQobj;
    using NodeApi::newShadowNode; 

    using NodeApi::getParentNode;
    //using NodeApi::setParentNode;

    using NodeApi::addChildNode;
    using NodeApi::findChildNode;
    using NodeApi::allChildrenNode;

    using NodeApi::childrenCount;
    using NodeApi::childByIndex;
    using NodeApi::indexInBrothers;

    using NodeApi::getRootNode;
    using NodeApi::findNode;

     // add by Chunlian for Alice @2022-01-11
    inline FdmQml_Param * getQmlObj(){ return qobject_cast<FdmQml_Param * >( qobjReal());}

    QList<NodeType *>  allParamItem();
    QVariant getValueWorkByUI(QVariant rawtext);

    //add for treeModel
    int childCount()            { return childrenCount(); }
    NodeType *childAt(int row)  { return childByIndex(row); }
    int row()                   { return indexInBrothers(); }


    
public:
    inline QVariant getFdmValue() {return getQobjProperty("fdmValue");}
    QVariant::Type  getFdmType();
    inline bool     getFdmEnable(){return getQobjProperty("fdmEnabled").toBool();}

    bool isCategory() {return !getProperty(fdmsettings::fdmPrinterJsonType).toString().compare("category");}
    bool isParam(){return hasProperty(fdmsettings::fdmPrinterJsonDefaultValue);}
    bool isTheCategoryExpand(){ NodeType * c = getCategory(); return (c && c->getProperty(fdmsettings::fdmNodeExpand).toBool() );}
    NodeType * getCategory(){
        NodeType * findcategory = this;
        while(findcategory && !findcategory->isCategory()){
            findcategory = findcategory->getParentNode();
        }
        return (findcategory && findcategory->isCategory()) ? findcategory : nullptr;
    }

    /**



     */
public slots:
    void nodeValueChange_fromCpp (QVariant rawtext);    
    void nodeValueChange_fromUI  (QVariant rawtext);    
signals:
    void nodeValueChange_toUI (QVariant row);           
    void fdmValueChangeByUI   (QVariant value);         
    void fdmValueChange       (QVariant value);         

private:
    friend class FdmQmlSourceTree;
    void nodeValueChange_fromTree(QVariant logicValue); 
    void nodeValueChange_toTree  (QVariant setValue);   
    bool eventFilter(QObject *watched, QEvent *event) override;

public:
    /*================ dump ================*/
    Q_INVOKABLE void dumpParam(int t=0);
    void dumpParamTree();
};

using FdmParamNode = FdmCppParamNode;
using FdmParamRoot = FdmCppParamNode;


#endif // FDMPARAMDATA_H
