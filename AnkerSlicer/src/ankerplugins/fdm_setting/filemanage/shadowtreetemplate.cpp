#include "shadowtreetemplate.hpp"
#include <QDebug>

using namespace shadowtree;



/*================ class ClStoreData ================*/
ClStoreData::ClStoreData(QObject *master):QObject(nullptr){
    if(master){
        QObject::connect(master, &QObject::destroyed, this, &QObject::deleteLater);
    }
}

bool ClStoreData::has(const QString &key) const{
    return QObject::dynamicPropertyNames().contains(key.toLocal8Bit());
    //return get(key).type() != QVariant::Invalid;
}
QVariant ClStoreData::get(const QString &key) const{
    return QObject::property(key.toLocal8Bit());
}
void ClStoreData::set(const QString &key, QVariant value){
    QObject::setProperty(key.toLocal8Bit(), value);
}

int ClStoreData::size() const {
    return QObject::dynamicPropertyNames().size();
}

void ClStoreData::remove(const QString &key){
    set(key, QVariant());
}

QList<QByteArray> ClStoreData::keys(){
    return QObject::dynamicPropertyNames();
}

//bool ClShadowHash::event(QEvent *event){
//    if(false && m_master){
//        if (event->type() == QEvent::DynamicPropertyChange) {
//            auto e = static_cast<QDynamicPropertyChangeEvent*>(event);
//            const QByteArray key = e->propertyName();
//            QVariant value = property(key);

//            if(key == "value"){
//                emit dataChange(m_master, key, value);
//            }
//        }
//    }
//    return QObject::event(event);
//}

/*================ class ClShadowHash ================*/
ClInjPropertyData::ClInjPropertyData(QObject *qobjReal)      { __new(qobjReal); }
ClInjPropertyData::ClInjPropertyData(const ThisType &o)      { __set(o);}
void ClInjPropertyData::operator= (const ThisType &o)        { __set(o);}
bool ClInjPropertyData::operator==(const ThisType &o)        { return __com(o);}

static int aa=0, bb=0, cc= 0;

void ClInjPropertyData::__new(QObject *qobjReal){
    if(nullptr == qobjReal){ return ;}

    ThisType *inj = __get(qobjReal);

    inj->m_qobjReal  = qobjReal;
    inj->m_storeData = inj->m_storeData ? inj->m_storeData : new ClStoreData(m_qobjReal);

    if(this != inj){
        __set(*inj);
    }
}


ClInjPropertyData::ThisType *ClInjPropertyData::__get(QObject *qobjReal)
{
    //if(ThisType * self = dynamic_cast<ThisType *>(qobjReal)){
    //    return self;
    //}
    QVariant injPro = qobjReal->property("#INJ_PROPERTY#");
    if(ThisType * inj = injPro.value<ThisType *>()){
        return inj;
    }
    qobjReal->setProperty("#INJ_PROPERTY#", QVariant::fromValue(this));
    return this;
}


void ClInjPropertyData::__set(const ThisType &o){
    ThisType * inj = __get(o.m_qobjReal);
    inj->m_qobjReal  = this->m_qobjReal  = o.m_qobjReal;
    inj->m_storeData = this->m_storeData = o.m_storeData;
    inj->m_shadow    = this->m_shadow    = o.m_shadow;
}

bool ClInjPropertyData::__com(const ThisType &o){
    return  this->m_qobjReal  == o.m_qobjReal  &&
            this->m_storeData == o.m_storeData &&
            this->m_shadow    == o.m_shadow;
}



/*================ class CppItemNodeWrapper<NodeType> ================*/

void ClItemProperty::dumpMetaShadow(int t, QString *log){
    //qDebug() << __LINE__ << __FUNCTION__;
    QString _temp;
    QString *dbgStr = log ? log : &_temp;
    {
        QDebug debug(dbgStr); debug.nospace().noquote();
        QString _table    = QByteArray(t, ' ');
        QString _nodename = "NodeName: " + qobjReal()->objectName();                             _nodename = _nodename.leftJustified(32);
        debug << (_table + _nodename).leftJustified(72);

        // this qobjMeta()
        QString _children = "Children: " + QString::number(qobjReal()->children().size());              _children = _children.leftJustified(12); debug << _children;
        QString _metaData = "real : " + QString::number(qobjReal()->metaObject()->propertyCount());     _metaData = _metaData.leftJustified(12); debug << _metaData;
        QString _dynamic  = "dyna : " + QString::number(qobjReal()->dynamicPropertyNames().size());     _dynamic  = _dynamic .leftJustified(12); debug << _dynamic;
        QString _property = "store: " + QString::number(storeData().size());                            _property = _property.leftJustified(12); debug << _property;

        int width = 16;
        for(auto keyIt : storeData().keys()){
            if(keyIt.length() > width){
                width = keyIt.length() / 4 * 4;
            }
        }width +=  4;

        if(Qobj_MetaPropertyCount(qobjReal()) > 0){
            static QList<QByteArray> skip = { "parent", "data", "resources", "children", "x", "y", "z", "width", "height", "opacity", "enabled", "visible", "visibleChildren", "states", "transitions", "state", "childrenRect", "anchors", "left", "right", "horizontalCenter", "top", "bottom", "verticalCenter", "baseline", "baselineOffset", "clip", "focus", "activeFocus", "activeFocusOnTab", "rotation", "scale", "transformOrigin", "transformOriginPoint", "transform", "smooth", "antialiasing", "implicitWidth", "implicitHeight", "containmentMask", "layer"};
            debug << "\n" << QByteArray(t+8, ' ') << _metaData;
            QList<QByteArray> keys = Qobj_MetaPropertyKeys(qobjReal());
            for(auto ketIt : keys){
                if(skip.contains(ketIt)){continue;}
                QString  key   = ketIt;
                QVariant value = getQobjProperty(key);

                debug << "\n" << QByteArray(t+10, ' ');
                debug << key.leftJustified(width) << value;
            }
        }

        if(Qobj_DynamicPropertyCount(qobjReal()) > 0){
            debug << "\n" << QByteArray(t+8, ' ') << _dynamic;
            int count = Qobj_DynamicPropertyCount(qobjReal());
            QList<QByteArray> keys = Qobj_DynamicPropertyKeys(qobjReal());
            for(int i=0; i < count; ++i){
                QString  key   = keys[i];
                QVariant value = getQobjProperty(key);

                debug << "\n" << QByteArray(t+10, ' ');
                debug << key.leftJustified(width) << value;
            }
        }

        if(storeData().size() > 0){
            debug << "\n" << QByteArray(t+8, ' ') << _property;
            for(auto keyIt : storeData().keys()){
                QString  key   = keyIt;
                QVariant value = storeData().get(keyIt);

                debug << "\n" << QByteArray(t+10, ' ');
                debug << key.leftJustified(width) << value;
            }
        }

        debug << "\n\n";
    }

    qDebug().noquote().nospace() << *dbgStr;
}


