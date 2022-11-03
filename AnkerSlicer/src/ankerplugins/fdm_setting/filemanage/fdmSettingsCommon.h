#ifndef FDMSETTINGSCOMMON_H
#define FDMSETTINGSCOMMON_H

#include <QString>

namespace fdmsettings {


const QString fdmPrinterJsonSettings    = QStringLiteral("settings");
const QString fdmPrinterJsonIcon        = QStringLiteral("icon");
const QString fdmPrinterJsonType        = QStringLiteral("type");
const QString fdmPrinterJsonChildern    = QStringLiteral("children");

const QString fdmPrinterJsonLabel       = QStringLiteral("label");
const QString fdmPrinterJsonDescription = QStringLiteral("description");
const QString fdmPrinterJsonLabelRaw       = QStringLiteral("@label_raw");
const QString fdmPrinterJsonDescriptionRaw = QStringLiteral("@description_raw");

const QString fdmPrinterAffects         = QStringLiteral("@fdmAffects");
const QString fdmPrinterAffectedBy      = QStringLiteral("@fdmAffectedBy");
const QString fdmPrinterAffectedById    = QStringLiteral("@fdmAffectedById");
const QString fdmPrinterAffects_UI      = QStringLiteral("@fdmAffects_UI");
const QString fdmPrinterAffectedBy_UI   = QStringLiteral("@fdmAffectedBy_UI");
const QString fdmPrinterAffects_Map     = QStringLiteral("@fdmAffects_Map");
const QString fdmPrinterAffectedBy_Map  = QStringLiteral("@fdmAffectedBy_Map");
//paramItem
const QString fdmPrinterJsonValue               = QStringLiteral("value");
const QString fdmPrinterJsonDefaultValue        = QStringLiteral("default_value");
const QString fdmPrinterJsonMinValueWarning     = QStringLiteral("minimum_value_warning");
const QString fdmPrinterJsonMaxValueWarning     = QStringLiteral("maximum_value_warning");
const QString fdmPrinterJsonMinValue            = QStringLiteral("minimum_value");
const QString fdmPrinterJsonMaxValue            = QStringLiteral("maximum_value");
const QString fdmPrinterJsonUnit                = QStringLiteral("unit");

const QString fdmPrinterJsonValueWarning            = QStringLiteral("@value_warning");
const QString fdmPrinterJsonValueError              = QStringLiteral("@value_error");

const QString fdmPrinterJsonValueWork               = QStringLiteral("@value");
const QString fdmPrinterJsonDefaultValueWork        = QStringLiteral("@default_value");
const QString fdmPrinterJsonMinValueWarningWork     = QStringLiteral("@minimum_value_warning");
const QString fdmPrinterJsonMaxValueWaringWork      = QStringLiteral("@maximum_value_warning");
const QString fdmPrinterJsonMinValueWork            = QStringLiteral("@minimum_value");
const QString fdmPrinterJsonMaxValueWork            = QStringLiteral("@maximum_value");

const QString fdmPrinterJsonTablePerMesh        = QStringLiteral("settable_per_mesh");
const QString fdmPrinterJsonTablePerExtruder    = QStringLiteral("settable_per_extruder");
const QString fdmPrinterJsonTablePerMeshgroup   = QStringLiteral("settable_per_meshgroup");
const QString fdmPrinterJsonTableGlobally       = QStringLiteral("settable_globally");
const QString fdmPrinterJsonLimitToExtruder     = QStringLiteral("limit_to_extruder");
const QString fdmPrinterJsonEnble               = QStringLiteral("enabled");
const QString fdmPrinterJsonResolve             = QStringLiteral("resolve");

const QString fdmPrinterJsonOptions             = QStringLiteral("options"); // QMap<QString, QString>
const QString fdmPrinterJsonChildren            = QStringLiteral("children");
//for ui
const QString fdmNodeIcon                    = QStringLiteral("#fdmNodeIcon");         //QString-- icon path
//const QString fdmNodeCheckState              = QStringLiteral("#fdmNodeCheckState");  //bool true-checked false-unchecked
const QString fdmNodeLevel                   = QStringLiteral("#fdmNodeLevel");        //int
const QString fdmNodeIsVisibility            = QStringLiteral("#fdmNodeIsVisibility"); //bool
//const QString fdmNodeInitialValue            = QStringLiteral("#fdmNodeInitialValue");
//const QString fdmNodeRealValue               = QStringLiteral("#fdmNodeRealValue");
const QString fdmNodeValueRangeFlag          = QStringLiteral("#fdmNodeValueRangeFlag"); //int/enum  -1--error(red)  0--normal  1-- warning(orange)
const QString fdmNodeExpand                  = QStringLiteral("#fdmNodeExpand");  
const QString fdmNodeRowInModelList          = QStringLiteral("#fdmNodeRowInModelList");  


}

#endif // FDMSETTINGSCOMMON_H
