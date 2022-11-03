#ifndef FDMQMLSOURCETREE_H
#define FDMQMLSOURCETREE_H

#include "fdmparamdata.h"
#include "../profilemanage/fdmprofilecategory.h"

/*================ class FdmQmlTreeApi ================*/


class FdmQmlTreeApi : public QObject{
    Q_OBJECT
public:
    void setAny(const QList<FdmProfileCategory> & categories);
    
    void getAll(QList<FdmProfileCategory> & categories);
    
    void getCategory(FdmProfileCategory & category);
    
    void setCategory(const FdmProfileCategory & category);
    QVariant::Type getTypeOfKey(const QString &key);


    //get setting for slice
    QString getCustomSetting();

signals:
    void anyNodeValueChange(const QString & categoryName, const FdmSettingItem & item);
    void anyNodeValueChangeByUI(const QString & categoryName, const FdmSettingItem & item);

private:
    friend class FdmQmlSourceTree;
    FdmQmlTreeApi(FdmParamRoot *root, bool activateValueChangeEvent);
    static void __getCategory(FdmParamNode *categoryNode,       FdmProfileCategory & category);
    static void __setCategory(FdmParamNode *rootNode    , const FdmProfileCategory & category);
    const QList<FdmParamNode *> __getCategoryNodes();
    FdmParamRoot * m_fdmParamRoot;
};


/*================ class FdmQmlSourceTree ================*/
class FdmQmlSourceTree
{
    using ThisType = FdmQmlSourceTree;
public:
    static FdmQmlSourceTree& instance()
    {
        static FdmQmlSourceTree staticInstance;
        return staticInstance;
    }

    explicit FdmQmlSourceTree(){}

    //~FdmQmlSourceTree(){}
    void init();
    
    QVariantMap refreshLanguageArg;   // OnlyAtAtartup or Realtime   add @2022-10-11 by CL
    void setLanguage(int index);
    void setAiCheck(bool check);

    /// getter
    inline FdmParamNode * qmlSourceTree_Right(){return (FdmParamNode *)m_qmlSourceTree_Right; }
    inline FdmParamNode * qmlSourceTree_Preference(){return (FdmParamNode *)m_qmlSourceTree_Preference; }

    //for Alice FdmParamSettingsWidget
    QList<FdmParamNode *> loadParamList(const QString &file); 

    //for Alice FdmParamSettingsWidget
    QList<FdmParamNode *> loadParamListForPreference(const QString &file); 

    //  for Andrew
    inline FdmQmlTreeApi &getFdmQmlTreeApi_Right() { return *m_fdmQmlTree_Right;}
    inline FdmQmlTreeApi &getFdmQmlTreeApi_Preference() { return *m_fdmQmlTree_Preference;}


//    template <typename Receiver, typename Func2>
//    void connectEvent4Sign(Receiver receiver, Func2 method, Qt::ConnectionType type = Qt::AutoConnection){
//        QObject::connect(m_data, &ClParamDataObj::sign_event, receiver, method, type);
//    }

    ///  member variable
private:
    FdmParamRoot * m_qmlSourceTree_Right{nullptr};
    FdmParamRoot * m_qmlSourceTree_Preference{nullptr};

    FdmQmlTreeApi * m_fdmQmlTree_Right{nullptr};
    FdmQmlTreeApi * m_fdmQmlTree_Preference{nullptr};

    //  add @2022-06-30 by CL
    void afterInit();
    void calcEffects(FdmParamRoot *srcRoot);
    void updataEffects(FdmParamRoot *srcRoot);
    void updataLimitValue(FdmParamRoot *srcRoot);
};


#endif // FDMQMLSOURCETREE_H
