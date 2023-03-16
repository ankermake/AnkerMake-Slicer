#include "fdmqmlsourcetree.h"
#include "qmljsonsetting.hpp"

#include <QDebug>
#include <QDir>
#include <QTimer>
#include "fdmqmlengine.h"
#include "common/utilities/tlogger.h"

/*================ class FdmQmlTreeApi ================*/
FdmQmlTreeApi::FdmQmlTreeApi(FdmParamRoot *root, bool activateValueChangeEvent):m_fdmParamRoot(root){

    if(activateValueChangeEvent){

        const QList<FdmParamNode *> categoryNodes = __getCategoryNodes();

        for(auto categoryNode : categoryNodes){
            const QString categoryName = categoryNode->getNodeName();
            const QList<FdmParamNode *> nodes = categoryNode->allChildrenNode(Qt::FindChildrenRecursively);
            for(auto node :nodes){
                if(!node->isParam()){ continue;}

                const QString nodeName = node->getNodeName();

                auto my_anyNodeValueChange = [categoryName, nodeName, node, this](QVariant rawtext){
                    FdmSettingItem item;
                    item.name = nodeName;
                    //item.value = node->getFdmValue();
                    item.value = rawtext;
                    item.fdmSettablePerExtruder = node->getProperty(fdmsettings::fdmPrinterJsonTablePerExtruder).toBool();
                    emit anyNodeValueChange(categoryName, item);
                };

                QObject::connect(node, &FdmParamNode::fdmValueChange, my_anyNodeValueChange);


                auto my_anyNodeValueChangeByUI = [categoryName, nodeName, node, this](QVariant rawtext){
                    FdmSettingItem item;
                    item.name = nodeName;
                    //item.value = node->getFdmValue();
                    item.value = rawtext;
                    item.fdmSettablePerExtruder = node->getProperty(fdmsettings::fdmPrinterJsonTablePerExtruder).toBool();
                    emit anyNodeValueChangeByUI(categoryName, item);
                };
                QObject::connect(node, &FdmParamNode::fdmValueChangeByUI, my_anyNodeValueChangeByUI);
            }
        }
    }
}

void FdmQmlTreeApi::setAny(const QList<FdmProfileCategory> &categories){
    for(auto categorie : categories){
        setCategory(categorie);
    }
}

void FdmQmlTreeApi::getAll(QList<FdmProfileCategory> &categories){
    const QList<FdmParamNode *> categoryNodes = __getCategoryNodes();

    categories.clear();
    categories.reserve(categoryNodes.size());

    for(auto categoryNode :categoryNodes){
        FdmProfileCategory category;
        category.name = categoryNode->getNodeName();
        __getCategory(categoryNode, category);
        categories.append(category);
    }
}

void FdmQmlTreeApi::getCategory(FdmProfileCategory &category){
    //FdmParamNode * categoryNode = m_fdmParamRoot->findChildNode(category.name);
    __getCategory(m_fdmParamRoot, category);
}

void FdmQmlTreeApi::setCategory(const FdmProfileCategory &category){
    //FdmParamNode * categoryNode = m_fdmParamRoot->findChildNode(category.name);
    __setCategory(m_fdmParamRoot, category);
}

QVariant::Type FdmQmlTreeApi::getTypeOfKey(const QString &key){
    if(m_fdmParamRoot){
        if(auto node = m_fdmParamRoot->findChildNode(key)){
            //return node->getFdmValue().type();
            return node->getFdmType();
        }
    }
    return QVariant::Invalid;
}

QString FdmQmlTreeApi::getCustomSetting()
{
    const QList<FdmParamNode *> categoryNodes = __getCategoryNodes();
    QStringList settingList;

    for(FdmParamNode * category : categoryNodes){
        const QList<FdmParamNode *> nodes = category->allChildrenNode(Qt::FindChildrenRecursively);

        for(FdmParamNode * node : nodes){
            QString value = node->getProperty(fdmsettings::fdmPrinterJsonValue).toString().trimmed();
            if (value.isEmpty()){continue;}
            if (value.indexOf(' ')>0 ||value.indexOf('\t')>0 ||  value.indexOf('\n')>0){
                value = QString("\"%1\"").arg(value);
            }
            settingList.append(QString(" -s %1=%2 ").arg(node->getNodeName()).arg(value));
        }
    }
    return settingList.join(" ");
}



void FdmQmlTreeApi::__getCategory(FdmParamNode *categoryNode, FdmProfileCategory &category)
{
    if(categoryNode && categoryNode->isCategory()){
        const QList<FdmParamNode *> nodes = categoryNode->allChildrenNode(Qt::FindChildrenRecursively);
        for(auto node :nodes){
            const QString  key   = node->getNodeName();
            const QVariant value = node->getFdmValue();
            category.set(key, value);
        }
    }
}

void FdmQmlTreeApi::__setCategory(FdmParamNode *rootNode, const FdmProfileCategory &category)
{
    if(rootNode){ //  && categoryNode->isCategory()
        const QList<FdmSettingItem> items = category.getSettings();
        for(const FdmSettingItem & item :items){
            if(item.name == "anker_param_ai_camera" || item.name == "international_language"){
                continue; 
            }
            if(FdmParamNode *node = rootNode->findChildNode(item.name, Qt::FindChildrenRecursively)){
                node->nodeValueChange_fromCpp(item.value);
            }
        }
    }
}

const QList<FdmParamNode *> FdmQmlTreeApi::__getCategoryNodes()
{
    {
        FdmParamNode * settings = m_fdmParamRoot->findChildNode("settings");
        const QList<FdmParamNode *> categoryNodes = settings->allChildrenNode(Qt::FindDirectChildrenOnly);
        return categoryNodes;
    }
    if(0){
        QList<FdmParamNode *> categoryNodes;
        QList<FdmParamNode *> all = m_fdmParamRoot->allChildrenNode(Qt::FindChildrenRecursively);
        for(FdmParamNode * node : all){
            if(node->isCategory()){
                categoryNodes.append(node);
            }
        }
        return categoryNodes;
    }
}





/*================ class FdmQmlSourceTree ================*/
void FdmQmlSourceTree::init()
{
    
    QUrl qmlFile = QUrl::fromLocalFile(":/qml/back_logic/FdmJsonObjTree_All.qml");  //  @2023-02-23 by ChunLian

    AkUtil::TFunction(qmlFile.toString());

    {
        QObject * root = FdmQmlEngine::instance()->loadQmlFile(qmlFile);
        m_qmlSourceTree_Right = FdmParamNode::newShadowNode(root);
        m_fdmQmlTree_Right = new FdmQmlTreeApi(m_qmlSourceTree_Right, true);
        QObject::connect(root, &QObject::destroyed, m_fdmQmlTree_Right, &QObject::deleteLater);

        if(true){
            QString dbgStr; QDebug debug(&dbgStr); debug.nospace().noquote();
            debug << "m_qmlSourceTree_Right:" << m_qmlSourceTree_Right ;
            qDebug() << dbgStr;
            AkUtil::TInfo(dbgStr);
        }
    }

    {
        QObject * root = FdmQmlEngine::instance()->loadQmlFile(qmlFile);
        m_qmlSourceTree_Preference = FdmParamNode::newShadowNode(root);
        m_fdmQmlTree_Preference = new FdmQmlTreeApi(m_qmlSourceTree_Preference, true);
        QObject::connect(root, &QObject::destroyed, m_fdmQmlTree_Preference, &QObject::deleteLater);

        if(true){
            QString dbgStr; QDebug debug(&dbgStr); debug.nospace().noquote();
            debug << "m_qmlSourceTree_Preference:" << m_qmlSourceTree_Preference ;
            qDebug() << dbgStr;
            AkUtil::TInfo(dbgStr);
        }
    }

    afterInit();  // add  @2022-06-30 by CL
    //m_qmlSourceTree_Right->dumpParamTree();

    //  test @2022-01-19 by CL
    //if(0){
    //    QObject::connect(m_fdmQmlTree_Right, &FdmQmlTreeApi::anyNodeValueChange,[](const QString &categoryName, const QString &nodeName, const QVariant value){
    //        qDebug() << __LINE__ <<  __FUNCTION__ << categoryName << nodeName << value;
    //    });
    //}

}

void FdmQmlSourceTree::afterInit()
{
    {   
        calcEffects(m_qmlSourceTree_Right);
        //calcEffects(m_qmlSourceTree_Preference);

        
    }

    {   
        updataLimitValue(m_qmlSourceTree_Right);
        //updataLimitValue(m_qmlSourceTree_Preference);
    }

#if 0  
    if(0){   
        if(FdmParamNode* languageNode = m_qmlSourceTree_Right->findChildNode("international_language"))
            QObject::connect(languageNode, &FdmParamNode::fdmValueChange, [this](QVariant value){
                if(value.type() == QVariant::Bool){
                    this->setLanguage(value.toBool());
                }
            });
    }

    {   
        QSettings settings;
        auto LanguageList = settings.value("LanguageList").toStringList();
        auto CurrentLanguage = settings.value("CurrentLanguage").toInt();

        qDebug() <<"LanguageList" << LanguageList << CurrentLanguage;
        setLanguage(CurrentLanguage);
    }
#endif
}


void FdmQmlSourceTree::setLanguage(int index)
{
    auto trLanguage = [](FdmParamRoot * trRoot, FdmParamRoot * srcRoot){
        const QString fdmLabel("fdmLabel");
        const QString fdmDescr("fdmDescription");
        const QString fdmOptions("fdmOptions");

        const QList<FdmParamNode *> allChildren = srcRoot->allChildrenNode(Qt::FindChildrenRecursively);
        for(FdmParamNode* node : allChildren){
            if(!node){continue;}
            const QString nodeName = node->getNodeName();
            FdmParamNode* trNode = trRoot->findChildNode(nodeName);
            if(!trNode){continue;}
            bool hasTr = false;

            //node->dumpParam();

            
            if (trNode->hasQobjProperty(fdmOptions)) {
                QVariant oldMap = node->getQobjProperty(fdmOptions);
                QVariant trMap = trNode->getQobjProperty(fdmOptions);
                if (oldMap != trMap) {
                    hasTr = true;
                    node->setQobjProperty(fdmOptions, trMap);
                }
            }

            
            if(trNode->hasQobjProperty(fdmLabel)){
                QVariant oldStr = node  ->getQobjProperty(fdmLabel);
                QVariant trStr  = trNode->getQobjProperty(fdmLabel);
                if(oldStr != trStr){
                    hasTr = true;
                    node->setQobjProperty(fdmLabel, trStr);
                    //qDebug() << fdmLabel << "old: " << oldStr << ", new: " << trStr << ", after: " << node->getQobjProperty(fdmLabel);
                }
            }
            
            if(trNode->hasQobjProperty(fdmDescr)){
                QVariant oldStr = node  ->getQobjProperty(fdmDescr);
                QVariant trStr  = trNode->getQobjProperty(fdmDescr);
                if(oldStr != trStr){
                    hasTr = true;
                    node->setQobjProperty(fdmDescr, trStr);
                    //qDebug() << fdmDescr << "old: " << oldStr << ", new: " << trStr << ", after: " << node->getQobjProperty(fdmDescr);
                }
            }

            //node->dumpParam(2);

            if(hasTr){
                node->nodeValueChange_fromTree(QVariant());
            }
        }
    };

    QUrl qmlFile_en = QUrl::fromLocalFile(":/qml/back_logic/FdmJsonObjTree_All.qml");       //  @2023-02-23 by ChunLian
    QUrl qmlFile_zh = QUrl::fromLocalFile(":/qml/back_logic/FdmJsonObjTree_All_zh.qml");    //  @2023-02-23 by ChunLian
    QUrl qmlFile_jp = QUrl::fromLocalFile(":/qml/back_logic/FdmJsonObjTree_All_jp.qml");    //  @2023-02-23 by ChunLian

    QUrl qmlFile;
    switch (index) {
        case 1: qmlFile = qmlFile_zh;break;
        case 2: qmlFile = qmlFile_jp;break;
        case 0:
        default:
            qmlFile = qmlFile_en;
            break;
    }

    
    
    //QObject * qmlRoot = FdmQmlEngine::instance()->loadQmlFile(qmlFile);
    FdmQmlEngine engin;
    QObject* qmlRoot = engin.loadQmlFile(qmlFile);
    FdmParamRoot * trRoot = FdmParamNode::newShadowNode(qmlRoot);

    
    trLanguage(trRoot, m_qmlSourceTree_Right);
    trLanguage(trRoot, m_qmlSourceTree_Preference);
    //gengxin(m_qmlSourceTree_Right);

    
    qmlRoot->deleteLater();

    {   //  add  @2022-06-30 by CL
        updataEffects(m_qmlSourceTree_Right);
        //updataEffects(m_qmlSourceTree_Preference);
    }
}

void FdmQmlSourceTree::setAiCheck(bool check)
{
    auto setAiCheck = [this](FdmParamRoot *root, bool check){
        if(!root){return;}
        if(auto node = root->findNode("anker_param_ai_camera")){
            node->setQobjProperty("fdmValue", check);
        }
    };

    setAiCheck(m_qmlSourceTree_Right, check);
    setAiCheck(m_qmlSourceTree_Preference, check);
}


void FdmQmlSourceTree::calcEffects(FdmParamRoot* srcRoot)
{

    QHash<QString, QStringList> affectedByIdsMap;   
    QHash<QString, QStringList> affectsMapTemp;     
    QList<FdmParamNode*> allChildrenNode = srcRoot->allChildrenNode(Qt::FindChildrenRecursively);
    for (int i = 0; i < allChildrenNode.size(); i++) {
        //allChildrenNode[i]->dumpParam();
        QVariant ids = allChildrenNode[i]->getQobjProperty("fdmAffectedById");
        QString nodeName = allChildrenNode[i]->getNodeName();

        if (ids.type() != QVariant::String)
        {
            continue;
        }
        QStringList affectedByIds = ids.toString().split(",");
        if (!affectedByIdsMap.contains(nodeName))
        {
            affectedByIdsMap[nodeName] = QStringList();
        }

        
        for (int j = 0; j < affectedByIds.size(); j++)
        {
            QString affectedById = affectedByIds[j];
            if (!affectedByIdsMap[nodeName].contains(affectedById))
            {
                affectedByIdsMap[nodeName].append(affectedById);

                
                if (!affectsMapTemp.contains(affectedById)){ affectsMapTemp[affectedById] = QStringList(); }
                affectsMapTemp[affectedById].append(nodeName);
            }
        }
    }

    srcRoot->setStoreProperty(fdmsettings::fdmPrinterAffects_Map, QVariant::fromValue(affectsMapTemp));
    srcRoot->setStoreProperty(fdmsettings::fdmPrinterAffectedBy_Map, QVariant::fromValue(affectedByIdsMap));
    //srcRoot->dumpParamTree();
}


void FdmQmlSourceTree::updataEffects(FdmParamRoot *srcRoot)
{
    QVariant affectsMapInTree = srcRoot->getStoreProperty(fdmsettings::fdmPrinterAffects_Map);
    if(!affectsMapInTree.canConvert<QHash<QString, QStringList>>()){return;}
    const auto affectsMap = affectsMapInTree.value<QHash<QString, QStringList>>();

    QVariant affectedByMapInTree = srcRoot->getStoreProperty(fdmsettings::fdmPrinterAffectedBy_Map);
    if(!affectedByMapInTree.canConvert<QHash<QString, QStringList>>()){return;}
    const auto affectedByMap = affectedByMapInTree.value<QHash<QString, QStringList>>();

    QList<FdmParamNode*> allChildrenNode = srcRoot->allChildrenNode(Qt::FindChildrenRecursively);
    for (int i = 0; i < allChildrenNode.size(); i++) {
        FdmParamNode* node = allChildrenNode[i];
        QString nodeName = node->getNodeName();

        if(affectsMap.contains(nodeName)){
            QStringList labelList;
            QStringList list = affectsMap[nodeName];
            for(const QString & id: list ){
                if(auto * node = srcRoot->findNode(id)){
                    labelList.append(node->getStoreProperty(fdmsettings::fdmPrinterJsonLabel).toString());
                }
            }
            
            QString str = labelList.join("<br>"); 
            node->setStoreProperty(fdmsettings::fdmPrinterAffects, str);
        }
        else{
            node->setStoreProperty(fdmsettings::fdmPrinterAffects, QString());
        }

        if(affectedByMap.contains(nodeName)){
            QStringList labelList;
            QStringList list = affectedByMap[nodeName];
            for(const QString & id: list ){
                if(auto * node = srcRoot->findNode(id)){
                    labelList.append(node->getStoreProperty(fdmsettings::fdmPrinterJsonLabel).toString());
                }
            }
            QString str = labelList.join("<br>"); 
            node->setStoreProperty(fdmsettings::fdmPrinterAffectedBy, str);
        }
        else{
            node->setStoreProperty(fdmsettings::fdmPrinterAffectedBy, QString()); 
        }
    }
}

void FdmQmlSourceTree::updataLimitValue(FdmParamRoot *srcRoot)
{
    QList<FdmParamNode*> allChildrenNode = srcRoot->allChildrenNode(Qt::FindChildrenRecursively);
    const QString TypeDouble("float");
    const QString TypeInt("int");

    for (int i = 0; i < allChildrenNode.size(); i++) {
        FdmParamNode* node = allChildrenNode[i];
        const QString type = node->getStoreProperty(fdmsettings::fdmPrinterJsonType).toString();

        if(type == TypeDouble || type == TypeInt)   ;else{continue;}

        {   
            QVariant limitVar = node->getStoreProperty(fdmsettings::fdmPrinterJsonMinValue);
            if(!limitVar.isValid()){
                double limit = 0;
                if(type == TypeDouble)
                    limit = -std::numeric_limits<double>::infinity();
                else if(type == TypeInt)
                    limit = std::numeric_limits<int>::min();
                node->setStoreProperty(fdmsettings::fdmPrinterJsonMinValue, limit);
            }
        }

        {   
            QVariant limitVar = node->getStoreProperty(fdmsettings::fdmPrinterJsonMaxValue);
            if(!limitVar.isValid()){
                double limit = 0;
                if(type == TypeDouble)
                    limit = +std::numeric_limits<double>::infinity();
                else if(type == TypeInt)
                    limit = std::numeric_limits<int>::max();
                node->setStoreProperty(fdmsettings::fdmPrinterJsonMaxValue, limit);
            }
        }

        {   
            QVariant limitVar = node->getStoreProperty(fdmsettings::fdmPrinterJsonMinValueWarning);
            if(!limitVar.isValid()){
                QVariant limitMinValue = node->getStoreProperty(fdmsettings::fdmPrinterJsonMinValue);
                if(limitMinValue.isValid()){
                    node->setStoreProperty(fdmsettings::fdmPrinterJsonMinValueWarning, limitMinValue);
                }else{
                    double limit = 0;
                    if(type == TypeDouble)
                        limit = -std::numeric_limits<double>::infinity();
                    else if(type == TypeInt)
                        limit = std::numeric_limits<int>::min();
                    node->setStoreProperty(fdmsettings::fdmPrinterJsonMinValueWarning, limit);
                }
            }
        }

        {   
            QVariant limitVar = node->getStoreProperty(fdmsettings::fdmPrinterJsonMaxValueWarning);
            if(!limitVar.isValid()){
                QVariant limitMaxValue = node->getStoreProperty(fdmsettings::fdmPrinterJsonMaxValue);
                if(limitMaxValue.isValid()){
                    node->setStoreProperty(fdmsettings::fdmPrinterJsonMaxValueWarning, limitMaxValue);
                }else{
                    double limit = 0;
                    if(type == TypeDouble)
                        limit = +std::numeric_limits<double>::infinity();
                    else if(type == TypeInt)
                        limit = std::numeric_limits<int>::max();
                    node->setStoreProperty(fdmsettings::fdmPrinterJsonMaxValueWarning, limit);
                }
            }
        }
    }
}

#include "filemanage/fdmsettingvisibilityconfig.h"
QList<FdmParamNode *> FdmQmlSourceTree::loadParamList(const QString &file)
{
    //for expert.cfg expert.cfg advanced.cft
    fdmsettings::FdmSettingVisibilityConfig config;
    return  config.loadList(m_qmlSourceTree_Right,file);
}

QList<FdmParamNode *> FdmQmlSourceTree::loadParamListForPreference(const QString &file)
{
    fdmsettings::FdmSettingVisibilityConfig config;
    return  config.loadList(m_qmlSourceTree_Preference,file);
}

#if 0 
//#include <QSettings>
//void FdmQmlSourceTree::saveCustomIni(const QString &saveFile){
//    if(!m_qmlSourceTree_Right){return;}

//    QSettings ini(saveFile, QSettings::IniFormat);

//    QList<FdmParamNode *> all = m_qmlSourceTree_Right->allChildrenNode(Qt::FindChildrenRecursively);
//    for(FdmParamNode * group : all){
//        if(group->getProperty(fdmPrinterJsonType) != "category"){ continue;}
//        QList<FdmParamNode *> nodes = group->allChildrenNode(Qt::FindChildrenRecursively);

//        ini.beginGroup(group->getNodeName());
//        for(FdmParamNode * node : nodes){
//            ini.setValue(node->getNodeName(), node->getProperty(fdmPrinterJsonValue));
//        }
//        ini.endGroup();
//    }
//}

//


//#include <QSettings>
//void FdmQmlSourceTree::saveCustomIni(const QString &saveFile){
//    if(!m_qmlSourceTree_Right){return;}

//    QSettings ini(saveFile, QSettings::IniFormat);

//    QList<FdmParamNode *> all = m_qmlSourceTree_Right->allChildrenNode(Qt::FindChildrenRecursively);
//    for(FdmParamNode * group : all){
//        if(group->getProperty(fdmPrinterJsonType) != "category"){ continue;}
//        QList<FdmParamNode *> nodes = group->allChildrenNode(Qt::FindChildrenRecursively);

//        ini.beginGroup(group->getNodeName());
//        for(FdmParamNode * node : nodes){
//            ini.setValue(node->getNodeName(), node->getProperty(fdmPrinterJsonValue));
//        }
//        ini.endGroup();
//    }
//}

//void FdmQmlSourceTree::resetCustomIni(const QString &iniFile){
//    if(!m_qmlSourceTree_Right){return;}

//    QSettings ini(iniFile, QSettings::IniFormat);
//    QStringList rawkeys = ini.allKeys();
//    QMap<QString, QString> keymap;
//    for(const QString & rawkey : rawkeys){
//        keymap.insert(rawkey, rawkey.split('/').last());
//    }

//    for(const QString & rawkey : rawkeys){
//        QString nodeName = keymap[rawkey]; //
//        FdmParamNode * node = m_qmlSourceTree_Right->findChildNode(nodeName);
//        if(!node){continue;}
//        QVariant newValue = ini.value(rawkey);
//        node->nodeValueChange_M2D(newValue);
//        //emit node->nodeValueChange_D2M(newValue);
//    }
////}
#endif


