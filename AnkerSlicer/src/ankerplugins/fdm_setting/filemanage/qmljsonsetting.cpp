#include "qmljsonsetting.hpp"
#include <QFileInfo>

using namespace fdmsettings;


/*================ class FdmQml_ItemBase ================*/
FdmQml_ItemBase::FdmQml_ItemBase(QQuickItem *parent)
    :QQuickItem(parent), ItemProperty(this){}

QVariant FdmQml_ItemBase::fdmValue(){
    return getStoreProperty(fdmPrinterJsonValue);
}

void FdmQml_ItemBase::setFdmValue(const QVariant &newValue){
    if(!__hasAttr.fdmValue){__hasAttr.fdmValue = true;} else
    if (fdmValue() == newValue)
        return;
    setStoreProperty(fdmPrinterJsonValue, newValue);
    auto a = fdmValue();
    if (a == newValue){}
    emit fdmValueChanged();
}

QVariant FdmQml_ItemBase::fdmDefaultValue(){
    return getStoreProperty(fdmPrinterJsonDefaultValue);
}

void FdmQml_ItemBase::setFdmDefaultValue(const QVariant &newDefaultValue_){
    QVariant newDefaultValue = newDefaultValue_;
    if(!__hasAttr.fdmDefaultValue){__hasAttr.fdmDefaultValue = true;}
    //else if (fdmDefaultValue() == newDefaultValue) return;
    { 
        if(fdmType() == "float"){
            newDefaultValue = newDefaultValue.toDouble();
        }
    }
    setStoreProperty(fdmPrinterJsonDefaultValue, newDefaultValue);

    if(fdmType() == "polygons" || fdmType() == "polygon" || fdmType() == "[int]" ){
        QVariant a = newDefaultValue;
        auto b = newDefaultValue.value<QJSValue>();
        qDebug() << getNodeName() << fdmType() << a << b.toVariant() << b.toVariant().toString();
    }

    emit fdmDefaultValueChanged();
    setFdmValue(newDefaultValue);
    setStoreProperty(fdmPrinterJsonValue, newDefaultValue);
}

QVariant FdmQml_ItemBase::fdmResolve(){
    return getStoreProperty(fdmPrinterJsonResolve);
}

void FdmQml_ItemBase::setFdmResolve(const QVariant &newFdmResolve){
    if(!__hasAttr.fdmResolve){__hasAttr.fdmResolve = true;} else
    if (fdmResolve() == newFdmResolve)
        return;
    setStoreProperty(fdmPrinterJsonResolve, newFdmResolve);
    emit fdmResolveChanged();
}

QString FdmQml_ItemBase::fdmLabel(){
    return getStoreProperty(fdmPrinterJsonLabel).toString();
}

void FdmQml_ItemBase::setFdmLabel(const QString &newLabel){
    if(!__hasAttr.fdmLabel){__hasAttr.fdmLabel = true;} else
    if (fdmLabel() == newLabel)
        return;
    setStoreProperty(fdmPrinterJsonLabel, newLabel);
    emit fdmLabelChanged();
}

QString FdmQml_ItemBase::fdmDescription(){
    return getStoreProperty(fdmPrinterJsonDescription).toString();
}

void FdmQml_ItemBase::setFdmDescription(const QString &newDescription){
    if(!__hasAttr.fdmDescription){__hasAttr.fdmDescription = true;} else
    if (fdmDescription() == newDescription)
        return;
    setStoreProperty(fdmPrinterJsonDescription, newDescription);
    emit fdmDescriptionChanged();
}

QString FdmQml_ItemBase::fdmAffects()
{
    
    return getStoreProperty(fdmPrinterAffects).toString();
}

void FdmQml_ItemBase::setFdmAffects(const QString &str)
{
    
    Q_UNUSED(str);
    
}

QString FdmQml_ItemBase::fdmAffectedBy()
{
    
    return getStoreProperty(fdmPrinterAffectedBy).toString();
}

void FdmQml_ItemBase::setFdmAffectedBy(const QString &str)
{
    Q_UNUSED(str);
    
}

QString FdmQml_ItemBase::fdmAffectedById()
{
    return getStoreProperty(fdmPrinterAffectedById).toString();
}

void FdmQml_ItemBase::setFdmAffectedById(const QString &str)
{
    if(!__hasAttr.fdmAffectedById){__hasAttr.fdmAffectedById = true;} else
    if (fdmAffectedById() == str)
        return;
    setStoreProperty(fdmPrinterAffectedById, str);
    emit fdmAffectedByIdChanged();
}

QString FdmQml_ItemBase::fdmAffects_UI()
{
    return getStoreProperty(fdmPrinterAffects_UI).toString();
}

void FdmQml_ItemBase::setFdmAffects_UI(const QString &str)
{
    if(!__hasAttr.fdmAffects_UI){__hasAttr.fdmAffects_UI = true;} else
    if (fdmAffects_UI() == str)
        return;
    setStoreProperty(fdmPrinterAffects_UI, str);
    emit fdmAffects_UIChanged();
}

QString FdmQml_ItemBase::fdmAffectedBy_UI()
{
    return getStoreProperty(fdmPrinterAffectedBy_UI).toString();
}

void FdmQml_ItemBase::setFdmAffectedBy_UI(const QString &str)
{
    if(!__hasAttr.fdmAffectedBy_UI){__hasAttr.fdmAffectedBy_UI = true;} else
    if (fdmAffectedBy_UI() == str)
        return;
    setStoreProperty(fdmPrinterAffectedBy_UI, str);
    emit fdmAffectedBy_UIChanged();
}

QString FdmQml_ItemBase::fdmType(){
    return getStoreProperty(fdmPrinterJsonType).toString();
}

void FdmQml_ItemBase::setFdmType(const QString &newType){
    if(!__hasAttr.fdmType){__hasAttr.fdmType = true;} else
    if (fdmType() == newType)
        return;
    { 
        if(newType == "float"){
            QVariant defVal = fdmDefaultValue();
            if(defVal.isValid()){
                double doubleVal = defVal.toDouble();
                setFdmDefaultValue(doubleVal);
            }
        }
    }
    setStoreProperty(fdmPrinterJsonType, newType);
    emit fdmTypeChanged();
}

QString FdmQml_ItemBase::fdmIcon(){
    return getStoreProperty(fdmPrinterJsonIcon).toString();
}

void FdmQml_ItemBase::setFdmIcon(const QString &newIcon){
    if(!__hasAttr.fdmIcon){__hasAttr.fdmIcon = true;} else
    if (fdmIcon() == newIcon)
        return;
    setStoreProperty(fdmPrinterJsonIcon, newIcon);
    emit fdmIconChanged();
}

double FdmQml_ItemBase::fdmMinimumValue(){
    return getStoreProperty(fdmPrinterJsonMinValue).toDouble();
}

void FdmQml_ItemBase::setFdmMinimumValue(double newFdmMinimumValue){
    if(!__hasAttr.fdmMinimumValue){__hasAttr.fdmMinimumValue = true;} else
    if (qFuzzyCompare(fdmMinimumValue(), newFdmMinimumValue))
        return;
    setStoreProperty(fdmPrinterJsonMinValue, newFdmMinimumValue);
    emit fdmMinimumValueChanged();
}

double FdmQml_ItemBase::fdmMaximumValue(){
    return getStoreProperty(fdmPrinterJsonMaxValue).toDouble();
}

void FdmQml_ItemBase::setFdmMaximumValue(double newFdmMaximumValue){
    if(!__hasAttr.fdmMaximumValue){__hasAttr.fdmMaximumValue = true;} else
    if (qFuzzyCompare(fdmMaximumValue(), newFdmMaximumValue))
        return;
    setStoreProperty(fdmPrinterJsonMaxValue, newFdmMaximumValue);
    emit fdmMaximumValueChanged();
}

double FdmQml_ItemBase::fdmMinimumValueWarning(){
    return getStoreProperty(fdmPrinterJsonMinValueWarning).toDouble();
}

void FdmQml_ItemBase::setFdmMinimumValueWarning(double newFdmMinimumValueWarning){
    if(!__hasAttr.fdmMinimumValueWarning){__hasAttr.fdmMinimumValueWarning = true;} else
    if (qFuzzyCompare(fdmMinimumValueWarning(), newFdmMinimumValueWarning))
        return;
    setStoreProperty(fdmPrinterJsonMinValueWarning, newFdmMinimumValueWarning);
    emit fdmMinimumValueWarningChanged();
}

double FdmQml_ItemBase::fdmMaximumValueWarning(){
    return getStoreProperty(fdmPrinterJsonMaxValueWarning).toDouble();
}

void FdmQml_ItemBase::setFdmMaximumValueWarning(double newFdmMaximumValueWarning){
    if(!__hasAttr.fdmMaximumValueWarning){__hasAttr.fdmMaximumValueWarning = true;} else
    if (qFuzzyCompare(fdmMaximumValueWarning(), newFdmMaximumValueWarning))
        return;
    setStoreProperty(fdmPrinterJsonMaxValueWarning, newFdmMaximumValueWarning);
    emit fdmMaximumValueWarningChanged();
}

bool FdmQml_ItemBase::fdmValueWarning(){
    return getStoreProperty(fdmPrinterJsonValueWarning).toBool();
}

void FdmQml_ItemBase::setFdmValueWarning(bool newFdmValueWarning){
    if(!__hasAttr.fdmValueWarning){__hasAttr.fdmValueWarning = true;} else
    if (fdmValueWarning() == newFdmValueWarning && hasStoreProperty(fdmPrinterJsonValueWarning))
        return;
    setStoreProperty(fdmPrinterJsonValueWarning, newFdmValueWarning);
    emit fdmValueWarningChanged();
}

bool FdmQml_ItemBase::fdmValueError(){
    return getStoreProperty(fdmPrinterJsonValueError).toBool();
}

void FdmQml_ItemBase::setFdmValueError(bool newFdmValueError){
    if(!__hasAttr.fdmValueError){__hasAttr.fdmValueError = true;} else
    if (fdmValueError() == newFdmValueError)
        return;
    setStoreProperty(fdmPrinterJsonValueError, newFdmValueError);
    emit fdmValueErrorChanged();
}

//
/**
 * @brief FdmQml_ItemBase::fdmEnabled
 * @return
 *  TODO: @2022-01-25 by CL


 */
bool FdmQml_ItemBase::fdmEnabled(){
    if(!__hasAttr.fdmEnabled){return true;}
    QVariant var = getStoreProperty(fdmPrinterJsonEnabled);
    bool res = var.type() == var.Bool ? var.toBool() : true;
    return res;
}

void FdmQml_ItemBase::setFdmEnabled(bool newFdmEnabled){
    if(!__hasAttr.fdmEnabled){__hasAttr.fdmEnabled = true;}// else
    //if (fdmEnabled() == newFdmEnabled)return;
    setStoreProperty(fdmPrinterJsonEnabled, newFdmEnabled);
    emit fdmEnabledChanged();
}

bool FdmQml_ItemBase::fdmSettablePerMesh(){
    return getStoreProperty(fdmPrinterJsonTablePerMesh).toBool();
}

void FdmQml_ItemBase::setFdmSettablePerMesh(bool newFdmSettablePerMesh){
    if(!__hasAttr.fdmSettablePerMesh){__hasAttr.fdmSettablePerMesh = true;} else
    if (fdmSettablePerMesh() == newFdmSettablePerMesh && hasStoreProperty(fdmPrinterJsonTablePerMesh))
        return;
    setStoreProperty(fdmPrinterJsonTablePerMesh, newFdmSettablePerMesh);
    emit fdmSettablePerMeshChanged();
}

bool FdmQml_ItemBase::fdmSettablePerExtruder(){
    return getStoreProperty(fdmPrinterJsonTablePerExtruder).toBool();
}

void FdmQml_ItemBase::setFdmSettablePerExtruder(bool newFdmSettablePerExtruder){
    if(!__hasAttr.fdmSettablePerExtruder){__hasAttr.fdmSettablePerExtruder = true;} else
    if (fdmSettablePerExtruder() == newFdmSettablePerExtruder && hasStoreProperty(fdmPrinterJsonTablePerExtruder))
        return;
    setStoreProperty(fdmPrinterJsonTablePerExtruder, newFdmSettablePerExtruder);
    emit fdmSettablePerExtruderChanged();
}

bool FdmQml_ItemBase::fdmSettablePerMeshgroup(){
    return getStoreProperty(fdmPrinterJsonTablePerMeshgroup).toBool();
}

void FdmQml_ItemBase::setFdmSettablePerMeshgroup(bool newFdmSettablePerMeshgroup){
    if(!__hasAttr.fdmSettablePerMeshgroup){__hasAttr.fdmSettablePerMeshgroup = true;} else
    if (fdmSettablePerMeshgroup() == newFdmSettablePerMeshgroup && hasStoreProperty(fdmPrinterJsonTablePerMeshgroup))
        return;
    setStoreProperty(fdmPrinterJsonTablePerMeshgroup, newFdmSettablePerMeshgroup);
    emit fdmSettablePerMeshgroupChanged();
}

bool FdmQml_ItemBase::fdmSettableGlobally(){
    return getStoreProperty(fdmPrinterJsonTableGlobally).toBool();
}

void FdmQml_ItemBase::setFdmSettableGlobally(bool newFdmSettableGlobally){
    if(!__hasAttr.fdmSettableGlobally){__hasAttr.fdmSettableGlobally = true;} else
    if (fdmSettableGlobally() == newFdmSettableGlobally && hasStoreProperty(fdmPrinterJsonTableGlobally))
        return;
    setStoreProperty(fdmPrinterJsonTableGlobally, newFdmSettableGlobally);
    emit fdmSettableGloballyChanged();
}

QString FdmQml_ItemBase::fdmUnit(){
    return getStoreProperty(fdmPrinterJsonUnit).toString();
}

void FdmQml_ItemBase::setFdmUnit(const QString &newFdmUnit){
    if(!__hasAttr.fdmUnit){__hasAttr.fdmUnit = true;} else
    if (fdmUnit() == newFdmUnit)
        return;
    setStoreProperty(fdmPrinterJsonUnit, newFdmUnit);
    emit fdmUnitChanged();
}

int FdmQml_ItemBase::fdmLimitToExtruder(){
    return getStoreProperty(fdmPrinterJsonLimitToExtruder).toInt();
}

void FdmQml_ItemBase::setFdmLimitToExtruder(int newFdmLimitToExtruder){
    if(!__hasAttr.fdmLimitToExtruder){__hasAttr.fdmLimitToExtruder = true;} else
    if (fdmLimitToExtruder() == newFdmLimitToExtruder)
        return;
    setStoreProperty(fdmPrinterJsonLimitToExtruder, newFdmLimitToExtruder);
    emit fdmLimitToExtruderChanged();
}

QVariantMap FdmQml_ItemBase::fdmOptions(){
    return getStoreProperty(fdmPrinterJsonOptions).toMap();
}

void FdmQml_ItemBase::setFdmOptions(const QVariantMap &newFdmOptions){
    if(!__hasAttr.fdmOptions){__hasAttr.fdmOptions = true;} else
    if(fdmOptions() == newFdmOptions)
        return;
    setStoreProperty(fdmPrinterJsonOptions, newFdmOptions);
    emit fdmOptionsChanged();
}

/*================ class FdmQml_Engine loadQmlFile ================*/




void FdmQml_registerType(){
    qmlRegisterType<FdmQml_ItemBase>("FdmQml", 1, 0, "FdmQml_Object");
    qmlRegisterType<FdmQml_File>    ("FdmQml", 1, 0, "FdmQml_File");
    qmlRegisterType<FdmQml_Root>    ("FdmQml", 1, 0, "FdmQml_Root");
    qmlRegisterType<FdmQml_Settings>("FdmQml", 1, 0, "FdmQml_Settings");
    qmlRegisterType<FdmQml_Children>("FdmQml", 1, 0, "FdmQml_Children");
    qmlRegisterType<FdmQml_Category>("FdmQml", 1, 0, "FdmQml_Category");
    qmlRegisterType<FdmQml_Param>   ("FdmQml", 1, 0, "FdmQml_Param");
}


