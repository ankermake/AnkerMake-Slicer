#ifndef QMLJSONSETTING_H
#define QMLJSONSETTING_H

#include "shadowtreetemplate.hpp"
#include "fdmSettingsCommon.h"
#include <QQuickItem>
#include <QQmlApplicationEngine>

/*================ class ================*/
class FdmQml_ItemBase;
class FdmQml_File;
class FdmQml_Root;
class FdmQml_Settings;
class FdmQml_Children;
class FdmQml_Category;
class FdmQml_Param;


/*================ class FdmQml_ItemBase ================*/
class FdmQml_ItemBase : public QQuickItem, public shadowtree::CppItemNodeApi<FdmQml_ItemBase>
{
    Q_OBJECT
    Q_PROPERTY(QString objectName               READ objectName                 WRITE setObjectName)

    //Q_PROPERTY(QString fdmLabel                 READ fdmLabel                   WRITE setFdmLabel                   NOTIFY fdmLabelChanged)
    //Q_PROPERTY(QString fdmDescription           READ fdmDescription             WRITE setFdmDescription             NOTIFY fdmDescriptionChanged)
    //Q_PROPERTY(QString fdmType                  READ fdmType                    WRITE setFdmType                    NOTIFY fdmTypeChanged)
    //Q_PROPERTY(QString fdmIcon                  READ fdmIcon                    WRITE setFdmIcon                    NOTIFY fdmIconChanged)

    //Q_PROPERTY(QVariant fdmValue                READ fdmValue                   WRITE setFdmValue                   NOTIFY fdmValueChanged)
    //Q_PROPERTY(QVariant fdmDefaultValue         READ fdmDefaultValue            WRITE setFdmDefaultValue            NOTIFY fdmDefaultValueChanged)
    //Q_PROPERTY(QVariant fdmResolve              READ fdmResolve                 WRITE setFdmResolve                 NOTIFY fdmResolveChanged)

    //Q_PROPERTY(QString fdmLabel                 READ fdmLabel                   WRITE setFdmLabel                   NOTIFY fdmLabelChanged)
    //Q_PROPERTY(QString fdmDescription           READ fdmDescription             WRITE setFdmDescription             NOTIFY fdmDescriptionChanged)
    //Q_PROPERTY(QString fdmType                  READ fdmType                    WRITE setFdmType                    NOTIFY fdmTypeChanged)
    //Q_PROPERTY(QString fdmUnit                  READ fdmUnit                    WRITE setFdmUnit                    NOTIFY fdmUnitChanged)

    //Q_PROPERTY(double fdmMinimumValue           READ fdmMinimumValue            WRITE setFdmMinimumValue            NOTIFY fdmMinimumValueChanged)
    //Q_PROPERTY(double fdmMaximumValue           READ fdmMaximumValue            WRITE setFdmMaximumValue            NOTIFY fdmMaximumValueChanged)
    //Q_PROPERTY(double fdmMinimumValueWarning    READ fdmMinimumValueWarning     WRITE setFdmMinimumValueWarning     NOTIFY fdmMinimumValueWarningChanged)
    //Q_PROPERTY(double fdmMaximumValueWarning    READ fdmMaximumValueWarning     WRITE setFdmMaximumValueWarning     NOTIFY fdmMaximumValueWarningChanged)
    //Q_PROPERTY(bool fdmValueWarning             READ fdmValueWarning            WRITE setFdmValueWarning            NOTIFY fdmValueWarningChanged)
    //Q_PROPERTY(bool fdmValueError               READ fdmValueError              WRITE setFdmValueError              NOTIFY fdmValueErrorChanged)

    //Q_PROPERTY(bool fdmEnabled                  READ fdmEnabled                 WRITE setFdmEnabled                 NOTIFY fdmEnabledChanged)
    //Q_PROPERTY(bool fdmSettablePerMesh          READ fdmSettablePerMesh         WRITE setFdmSettablePerMesh         NOTIFY fdmSettablePerMeshChanged)
    //Q_PROPERTY(bool fdmSettablePerExtruder      READ fdmSettablePerExtruder     WRITE setFdmSettablePerExtruder     NOTIFY fdmSettablePerExtruderChanged)
    //Q_PROPERTY(bool fdmSettablePerMeshgroup     READ fdmSettablePerMeshgroup    WRITE setFdmSettablePerMeshgroup    NOTIFY fdmSettablePerMeshgroupChanged)
    //Q_PROPERTY(bool fdmSettableGlobally         READ fdmSettableGlobally        WRITE setFdmSettableGlobally        NOTIFY fdmSettableGloballyChanged)

    //Q_PROPERTY(int fdmLimitToExtruder           READ fdmLimitToExtruder         WRITE setFdmLimitToExtruder         NOTIFY fdmLimitToExtruderChanged)
    //Q_PROPERTY(QVariantMap fdmOptions           READ fdmOptions                 WRITE setFdmOptions                 NOTIFY fdmOptionsChanged)

public:
    using ItemProperty = shadowtree::CppItemNodeApi<FdmQml_ItemBase>;
    FdmQml_ItemBase(QQuickItem *parent = nullptr);
public:
    QVariant fdmValue();
    void  setFdmValue(const QVariant &newValue);

    QVariant fdmDefaultValue();
    void  setFdmDefaultValue(const QVariant &newDefaultValue);

    QVariant fdmResolve();
    void  setFdmResolve(const QVariant & newFdmSettableGlobally);

    QString fdmLabel();
    void setFdmLabel(const QString &newLabel);

    QString fdmDescription();
    void setFdmDescription(const QString &newDescription);

    QString fdmAffects();
    void setFdmAffects(const QString &str);
    QString fdmAffectedBy();
    void setFdmAffectedBy(const QString &str);
    QString fdmAffectedById();
    void setFdmAffectedById(const QString &str);
    QString fdmAffects_UI();
    void setFdmAffects_UI(const QString &str);
    QString fdmAffectedBy_UI();
    void setFdmAffectedBy_UI(const QString &str);
    QString fdmType();
    void setFdmType(const QString &newType);

    QString fdmIcon();
    void setFdmIcon(const QString &newIcon);

    double  fdmMinimumValue();
    void setFdmMinimumValue(double newFdmMinimumValue);

    double  fdmMaximumValue();
    void setFdmMaximumValue(double newFdmMaximumValue);

    double  fdmMinimumValueWarning();
    void setFdmMinimumValueWarning(double newFdmMinimumValueWarning);

    double  fdmMaximumValueWarning();
    void setFdmMaximumValueWarning(double newFdmMaximumValueWarning);

    bool    fdmValueWarning();
    void setFdmValueWarning(bool newFdmValueWarning);

    bool    fdmValueError();
    void setFdmValueError(bool newFdmValueError);

    bool    fdmEnabled();   
    void setFdmEnabled(bool newFdmEnabled);

    bool    fdmSettablePerMesh();
    void setFdmSettablePerMesh(bool newFdmSettablePerMesh);

    bool    fdmSettablePerExtruder();
    void setFdmSettablePerExtruder(bool newFdmSettablePerExtruder);

    bool    fdmSettablePerMeshgroup();
    void setFdmSettablePerMeshgroup(bool newFdmSettablePerMeshgroup);

    bool    fdmSettableGlobally();
    void setFdmSettableGlobally(bool newFdmSettableGlobally);

    QString fdmUnit();
    void setFdmUnit(const QString &newFdmUnit);

    int     fdmLimitToExtruder();
    void setFdmLimitToExtruder(int newFdmLimitToExtruder);

    QVariantMap fdmOptions();
    void setFdmOptions(const QVariantMap &newFdmOptions);

signals:
    void fdmValueChanged();
    void fdmDefaultValueChanged();
    void fdmLabelChanged();
    void fdmDescriptionChanged();
    void fdmAffectsChanged();
    void fdmAffectedByChanged();
    void fdmAffectedByIdChanged();
    void fdmAffects_UIChanged();
    void fdmAffectedBy_UIChanged();
    void fdmTypeChanged();
    void fdmIconChanged();
    void fdmMinimumValueChanged();
    void fdmMaximumValueChanged();
    void fdmMinimumValueWarningChanged();
    void fdmMaximumValueWarningChanged();
    void fdmValueWarningChanged();
    void fdmValueErrorChanged();
    void fdmEnabledChanged();
    void fdmSettablePerMeshChanged();
    void fdmSettablePerExtruderChanged();
    void fdmSettablePerMeshgroupChanged();
    void fdmSettableGloballyChanged();
    void fdmUnitChanged();
    void fdmLimitToExtruderChanged();
    void fdmOptionsChanged();
    void fdmResolveChanged();

public:

    struct __HasAttr{
        quint32 fdmValue        : 1;
        quint32 fdmValueUI      : 1;
        quint32 fdmDefaultValue : 1;
        quint32 fdmResolve      : 1;
        quint32 fdmIcon         : 1;

        quint32 fdmLabel         : 1;
        quint32 fdmDescription   : 1;
        quint32 fdmAffects          : 1;
        quint32 fdmAffectedBy       : 1;
        quint32 fdmAffectedById     : 1;
        quint32 fdmAffects_UI       : 1;
        quint32 fdmAffectedBy_UI    : 1;
        quint32 fdmType          : 1;
        quint32 fdmUnit          : 1;

        quint32 fdmMinimumValue        : 1;
        quint32 fdmMaximumValue        : 1;
        quint32 fdmMinimumValueWarning : 1;
        quint32 fdmMaximumValueWarning : 1;

        quint32 fdmValueWarning  : 1;
        quint32 fdmValueError    : 1;

        quint32 fdmEnabled              : 1;
        quint32 fdmSettablePerMesh      : 1;
        quint32 fdmSettablePerExtruder  : 1;
        quint32 fdmSettablePerMeshgroup : 1;
        quint32 fdmSettableGlobally     : 1;
        quint32 fdmLimitToExtruder : 1;
        quint32 fdmOptions : 1;
    };
    __HasAttr __hasAttr{0};
    inline const __HasAttr & attr(){return __hasAttr;}
};

/*================ class FdmQml_File, FdmQml_Root, FdmQml_Settings, FdmQml_Children ================*/
class FdmQml_File    : public FdmQml_ItemBase{ Q_OBJECT };
class FdmQml_Root    : public FdmQml_ItemBase{ Q_OBJECT };
class FdmQml_Settings: public FdmQml_ItemBase{ Q_OBJECT };
class FdmQml_Children: public FdmQml_ItemBase{ Q_OBJECT };

/*================ class FdmQml_Category ================*/
class FdmQml_Category : public FdmQml_ItemBase{
    Q_OBJECT
    Q_PROPERTY(QString fdmLabel                 READ fdmLabel                   WRITE setFdmLabel                   NOTIFY fdmLabelChanged)
    Q_PROPERTY(QString fdmDescription           READ fdmDescription             WRITE setFdmDescription             NOTIFY fdmDescriptionChanged)
    Q_PROPERTY(QString fdmType                  READ fdmType                    WRITE setFdmType                    NOTIFY fdmTypeChanged)
    Q_PROPERTY(QString fdmIcon                  READ fdmIcon                    WRITE setFdmIcon                    NOTIFY fdmIconChanged)
    Q_PROPERTY(bool fdmEnabled                  READ fdmEnabled                 WRITE setFdmEnabled                 NOTIFY fdmEnabledChanged)

    Q_PROPERTY(QString fdmAffects READ fdmAffects WRITE setFdmAffects NOTIFY fdmAffectsChanged)
    Q_PROPERTY(QString fdmAffectedBy READ fdmAffectedBy WRITE setFdmAffectedBy NOTIFY fdmAffectedByChanged)
    Q_PROPERTY(QString fdmAffectedById READ fdmAffectedById WRITE setFdmAffectedById NOTIFY fdmAffectedByIdChanged)

    Q_PROPERTY(double fdmMinimumValue           READ fdmMinimumValue            WRITE setFdmMinimumValue            NOTIFY fdmMinimumValueChanged)
    Q_PROPERTY(double fdmMaximumValue           READ fdmMaximumValue            WRITE setFdmMaximumValue            NOTIFY fdmMaximumValueChanged)
    Q_PROPERTY(double fdmMinimumValueWarning    READ fdmMinimumValueWarning     WRITE setFdmMinimumValueWarning     NOTIFY fdmMinimumValueWarningChanged)
    Q_PROPERTY(double fdmMaximumValueWarning    READ fdmMaximumValueWarning     WRITE setFdmMaximumValueWarning     NOTIFY fdmMaximumValueWarningChanged)
};

/*================ class FdmQml_Param ================*/
class FdmQml_Param : public FdmQml_ItemBase{
    Q_OBJECT

    Q_PROPERTY(QVariant fdmValue                READ fdmValue                   WRITE setFdmValue                   NOTIFY fdmValueChanged)
    Q_PROPERTY(QVariant fdmValueUI              READ fdmValue                   WRITE setFdmValue                   NOTIFY fdmValueChanged) //  add for Alice
    Q_PROPERTY(QVariant fdmDefaultValue         READ fdmDefaultValue            WRITE setFdmDefaultValue            NOTIFY fdmDefaultValueChanged)
    Q_PROPERTY(QVariant fdmResolve              READ fdmResolve                 WRITE setFdmResolve                 NOTIFY fdmResolveChanged)
    Q_PROPERTY(QString fdmAffects READ fdmAffects WRITE setFdmAffects NOTIFY fdmAffectsChanged)
    Q_PROPERTY(QString fdmAffectedBy READ fdmAffectedBy WRITE setFdmAffectedBy NOTIFY fdmAffectedByChanged)
    Q_PROPERTY(QString fdmAffectedById READ fdmAffectedById WRITE setFdmAffectedById NOTIFY fdmAffectedByIdChanged)

    Q_PROPERTY(QString fdmLabel                 READ fdmLabel                   WRITE setFdmLabel                   NOTIFY fdmLabelChanged)
    Q_PROPERTY(QString fdmDescription           READ fdmDescription             WRITE setFdmDescription             NOTIFY fdmDescriptionChanged)
    Q_PROPERTY(QString fdmType                  READ fdmType                    WRITE setFdmType                    NOTIFY fdmTypeChanged)
    Q_PROPERTY(QString fdmUnit                  READ fdmUnit                    WRITE setFdmUnit                    NOTIFY fdmUnitChanged)

    Q_PROPERTY(double fdmMinimumValue           READ fdmMinimumValue            WRITE setFdmMinimumValue            NOTIFY fdmMinimumValueChanged)
    Q_PROPERTY(double fdmMaximumValue           READ fdmMaximumValue            WRITE setFdmMaximumValue            NOTIFY fdmMaximumValueChanged)
    Q_PROPERTY(double fdmMinimumValueWarning    READ fdmMinimumValueWarning     WRITE setFdmMinimumValueWarning     NOTIFY fdmMinimumValueWarningChanged)
    Q_PROPERTY(double fdmMaximumValueWarning    READ fdmMaximumValueWarning     WRITE setFdmMaximumValueWarning     NOTIFY fdmMaximumValueWarningChanged)
    Q_PROPERTY(bool fdmValueWarning             READ fdmValueWarning            WRITE setFdmValueWarning            NOTIFY fdmValueWarningChanged)
    Q_PROPERTY(bool fdmValueError               READ fdmValueError              WRITE setFdmValueError              NOTIFY fdmValueErrorChanged)

    Q_PROPERTY(bool fdmEnabled                  READ fdmEnabled                 WRITE setFdmEnabled                 NOTIFY fdmEnabledChanged)
    Q_PROPERTY(bool fdmSettablePerMesh          READ fdmSettablePerMesh         WRITE setFdmSettablePerMesh         NOTIFY fdmSettablePerMeshChanged)
    Q_PROPERTY(bool fdmSettablePerExtruder      READ fdmSettablePerExtruder     WRITE setFdmSettablePerExtruder     NOTIFY fdmSettablePerExtruderChanged)
    Q_PROPERTY(bool fdmSettablePerMeshgroup     READ fdmSettablePerMeshgroup    WRITE setFdmSettablePerMeshgroup    NOTIFY fdmSettablePerMeshgroupChanged)
    Q_PROPERTY(bool fdmSettableGlobally         READ fdmSettableGlobally        WRITE setFdmSettableGlobally        NOTIFY fdmSettableGloballyChanged)

    Q_PROPERTY(int fdmLimitToExtruder           READ fdmLimitToExtruder         WRITE setFdmLimitToExtruder         NOTIFY fdmLimitToExtruderChanged)
    Q_PROPERTY(QVariantMap fdmOptions           READ fdmOptions                 WRITE setFdmOptions                 NOTIFY fdmOptionsChanged)
};

void FdmQml_registerType();



#endif // QMLJSONSETTING_H
