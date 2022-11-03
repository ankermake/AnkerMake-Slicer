#ifndef SHADOWTREETEMPLATE_H
#define SHADOWTREETEMPLATE_H

#include <QObject>
#include <QMetaProperty>
#include <QEvent>

namespace shadowtree {


/*================ class QObject metaObject Property ================*/
inline int Qobj_MetaPropertyCount(QObject *qobj){
    return qobj->metaObject()->propertyCount();
}

inline QList<QByteArray> Qobj_MetaPropertyKeys(QObject *qobj){
    int count = qobj->metaObject()->propertyCount();
    QList<QByteArray> keys;
    for(int i=0; i<count; ++i){
        QByteArray key = qobj->metaObject()->property(i).name();
        keys.append(key);
    }
    return keys;
}

/*================ class QObject Dynamic Property ================*/
inline int Qobj_DynamicPropertyCount(QObject *qobj){
    return qobj->dynamicPropertyNames().size();
}

inline QList<QByteArray> Qobj_DynamicPropertyKeys(QObject *qobj){
    return qobj->dynamicPropertyNames();
}


class ClStoreData : private QObject{
    Q_OBJECT
public:
    ClStoreData(QObject * master);      
    using QObject::installEventFilter;  

    bool     has(const QString & key)const;
    QVariant get(const QString & key)const;
    void     set(const QString & key, QVariant value);

    int      size() const;
    void     remove(const QString &key);
    QList<QByteArray> keys();

//signals:
//    void dataChange(QObject * master, const QString & key, QVariant value);
//private:
//    bool event(QEvent *event) override;
};


class ClInjPropertyData
{
public:
    using ThisType = ClInjPropertyData;
    explicit ClInjPropertyData(QObject * qobjReal=nullptr);
    explicit ClInjPropertyData(const ThisType & o);
    void operator= (const ThisType& o);
    bool operator==(const ThisType& o);
private:
    friend class ClItemProperty;
    QObject     * m_qobjReal {nullptr};  
    ClStoreData * m_storeData{nullptr};  
    QObject     * m_shadow   {nullptr};  
private:
    void       __new(QObject * qobjReal);
    ThisType * __get(QObject * qobjReal);
    void       __set(const ThisType & o);
    bool       __com(const ThisType & o);

};


/**
 * @brief The ClItemProperty class
 *



 */
class ClItemProperty{
protected:
    ClInjPropertyData inj;
public:
    explicit ClItemProperty(QObject * qobjReal=nullptr)    :inj(qobjReal){}
    explicit ClItemProperty(const ClInjPropertyData& data) :inj(data)    {}

    //  setFdmProperty
    inline ClStoreData & storeData(){return *inj.m_storeData;}
    inline bool     hasStoreProperty(const QString & cppKey)                    { return storeData().has(cppKey);}
    inline QVariant getStoreProperty(const QString & cppKey)                    { return storeData().get(cppKey); }
    inline void     setStoreProperty(const QString & cppKey, QVariant fdmValue) { storeData().set(cppKey, fdmValue); }

    //  setQmlProperty
    inline QObject *qobjReal(){return inj.m_qobjReal;}
    inline bool     hasQobjProperty(const QString & qmlKey)                     { return getQobjProperty(qmlKey).type() != QVariant::Invalid;}
    inline QVariant getQobjProperty(const QString & qmlKey)                     { return qobjReal()->property(qmlKey.toLocal8Bit()); }
    inline void     setQobjProperty(const QString & qmlKey, QVariant qtValue)   { qobjReal()->setProperty(qmlKey.toLocal8Bit(), qtValue); }

    void dumpMetaShadow(int t=0, QString * log=nullptr);

    inline QObject *__shadow(){return inj.m_shadow;}
    inline QObject *__setShadow(QObject * shadowNode){return inj.m_shadow = shadowNode;}
};


/*================ class CppItemNodeApi<NodeType> ================*/
/**


 */
template<typename T>
class CppItemNodeApi : public ClItemProperty{
protected:
    using ClItemProperty::ClItemProperty;
    using NodeType = T;
    using NodeList = QList<NodeType *>;
    using NodeApi  = CppItemNodeApi<NodeType>;
public:
    //  node name : objectName;
    inline QString  getNodeName(){return qobjReal()->objectName();}
    inline void     setNodeName(const QString & nodeName){qobjReal()->setObjectName(nodeName);}

    //  property :   m_data.m_hashData;
    inline bool     hasProperty(const QString & propertyName) { return hasStoreProperty(propertyName); }
    inline QVariant getProperty(const QString & propertyName) { return getStoreProperty(propertyName); }
    inline void     setProperty(const QString & propertyName, QVariant propertyValue) { setStoreProperty(propertyName, propertyValue); }
    inline void     insert     (const QString & propertyName, QVariant propertyValue) { setStoreProperty(propertyName, propertyValue); }

    static bool       checkNodeQobj(NodeType * nodePtr){return (nodePtr && nodePtr->qobjReal());}
    static NodeType * newShadowNode(QObject *qobjReal){
        if(!qobjReal){return nullptr;}

        
        NodeType * self = qobject_cast<NodeType *>(qobjReal);
        if(self) return self;

        
        ClItemProperty pro(qobjReal);
        NodeType * shadow = dynamic_cast<NodeType *>(pro.__shadow());
        if(shadow) return shadow;

        
        NodeType * newshadow = new NodeType();
        QObject::connect(qobjReal, &QObject::destroyed, newshadow, &QObject::deleteLater);

        pro.__setShadow(newshadow);
        *(ClItemProperty*)(newshadow) = pro;
        pro.storeData().installEventFilter(newshadow);  
        return newshadow;
    }


    //  parent children : FdmParamNode *m_parentNode; QHash m_childreNode,,
    NodeType * getParentNode()                                                   {return qobjReal()->parent() ? newShadowNode(qobjReal()->parent()) : nullptr;}
    void       setParentNode(NodeType * parentNode){if(checkNodeQobj(parentNode)){ qobjReal()->setParent(parentNode->qobjReal());}}
    void       addChildNode (NodeType * childNode ){if(checkNodeQobj(childNode )){ childNode->qobjReal()->setParent(this->qobjReal());}}

    NodeType * findChildNode(const QString &childNodeName, Qt::FindChildOptions options = Qt::FindChildrenRecursively){
        if(childNodeName.isEmpty()){return nullptr;}
        QList<QObject *> c = qobjReal()->template findChildren<QObject *>(childNodeName, options);
        if(c.size() == 1){ return newShadowNode(c[0]); }
        if(c.size() >  1){ Q_ASSERT(0); }  
        return nullptr;
    }
    QList<NodeType *>   allChildrenNode(Qt::FindChildOptions options = Qt::FindDirectChildrenOnly){
        QList<NodeType *> nodes;
        QObjectList children = qobjReal()->children();
        for(QObject * obj : children){
            NodeType *node = newShadowNode(obj);
            nodes.append(node);
            if(options == Qt::FindChildrenRecursively && obj->children().size() > 0){
                nodes.append(node->allChildrenNode(Qt::FindChildrenRecursively));
            }
        }
        return nodes;
    }

    //  childrenCount : childByIndex indexInBrothers
    int        childrenCount()        { return qobjReal()->children().size(); }
    NodeType * childByIndex(int index){ return (0 <= index && index < childrenCount()) ? newShadowNode(qobjReal()->children()[index]) : nullptr ;}
    int        indexInBrothers()      { return qobjReal()->parent() ? qobjReal()->parent()->children().indexOf( qobjReal() ): 0; }

    NodeType * getRootNode(){
        QObject * parent = qobjReal();
        while(parent->parent()){
            parent = parent->parent();
        }
        return newShadowNode(parent);
    }
    
    NodeType * findNode(const QString &childNodeName){return getRootNode()->findChildNode(childNodeName, Qt::FindChildrenRecursively);}
};

}

Q_DECLARE_METATYPE(shadowtree::ClInjPropertyData)
Q_DECLARE_METATYPE(shadowtree::ClInjPropertyData*)
Q_DECLARE_METATYPE(shadowtree::ClItemProperty)
Q_DECLARE_METATYPE(shadowtree::ClItemProperty*)


/*================ class MyParamNode_Demo1 ================*/
class RealQObject : public QObject{
    //Q_OBJECT
public:
    using QObject::QObject;
};

class MyParamNode_Demo1 : public RealQObject, public shadowtree::CppItemNodeApi<MyParamNode_Demo1>{
    //Q_OBJECT
public:
    explicit MyParamNode_Demo1()                    :NodeApi(this) {}
    explicit MyParamNode_Demo1(NodeType *parentNode):NodeApi(this){
        setParentNode(parentNode);
    }
};


/*================ class MyParamNode_Demo2 ================*/
class MyParamNode_RealObject : public RealQObject, public shadowtree::ClItemProperty{
    Q_OBJECT
public:
    explicit MyParamNode_RealObject(RealQObject * parent)
        :RealQObject(parent),shadowtree::ClItemProperty(this) {}
};

using ShadowQObject = QObject;
class MyParamNode_ShadowNode : public ShadowQObject, public shadowtree::CppItemNodeApi<MyParamNode_ShadowNode>{
    Q_OBJECT
public:
    MyParamNode_ShadowNode(){}
    MyParamNode_ShadowNode * newShadowNode(MyParamNode_RealObject * qobjReal){
        MyParamNode_ShadowNode * shadowNode;
        shadowNode = NodeApi::newShadowNode(qobjReal);
        return shadowNode;
    }
};

/*================ class MyParam_LikeMap_Demo3 ================*/
class MyParam_LikeMap_Demo3 : public shadowtree::ClStoreData{
public:
    using LikeMapApi = shadowtree::ClStoreData;
    using LikeMap = MyParam_LikeMap_Demo3;
    explicit MyParam_LikeMap_Demo3():LikeMapApi(nullptr){}
};


#endif // SHADOWTREETEMPLATE_H
