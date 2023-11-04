#ifndef FDMSETTINGSCOMMON_H
#define FDMSETTINGSCOMMON_H

#include <QString>

namespace fdmsettings {


const QByteArray fdmPrinterJsonSettings    = QByteArray("settings");
const QByteArray fdmPrinterJsonIcon        = QByteArray("icon");
const QByteArray fdmPrinterJsonType        = QByteArray("type");
const QByteArray fdmPrinterJsonChildren    = QByteArray("children");

const QByteArray fdmPrinterJsonLabel       = QByteArray("label");
const QByteArray fdmPrinterJsonDescription = QByteArray("description");

const QByteArray fdmPrinterAffects         = QByteArray("@fdmAffects");
const QByteArray fdmPrinterAffectedBy      = QByteArray("@fdmAffectedBy");
const QByteArray fdmPrinterAffectedById    = QByteArray("@fdmAffectedById");
const QByteArray fdmPrinterAffects_UI      = QByteArray("@fdmAffects_UI");
const QByteArray fdmPrinterAffectedBy_UI   = QByteArray("@fdmAffectedBy_UI");
const QByteArray fdmPrinterAffects_Map     = QByteArray("@fdmAffects_Map");
const QByteArray fdmPrinterAffectedBy_Map  = QByteArray("@fdmAffectedBy_Map");
//paramItem
const QByteArray fdmPrinterJsonValue               = QByteArray("value");
const QByteArray fdmPrinterJsonDefaultValue        = QByteArray("default_value");
const QByteArray fdmPrinterJsonMinValueWarning     = QByteArray("minimum_value_warning");
const QByteArray fdmPrinterJsonMaxValueWarning     = QByteArray("maximum_value_warning");
const QByteArray fdmPrinterJsonMinValue            = QByteArray("minimum_value");
const QByteArray fdmPrinterJsonMaxValue            = QByteArray("maximum_value");

const QByteArray fdmPrinterJsonMaxValueWarning0    = QByteArray("maximum_value_warning0");
const QByteArray fdmPrinterJsonMaxValue0           = QByteArray("maximum_value0");
const QByteArray fdmPrinterJsonMaxValueWarning1    = QByteArray("maximum_value_warning1");
const QByteArray fdmPrinterJsonMaxValue1           = QByteArray("maximum_value1");

const QByteArray fdmPrinterJsonUnit                = QByteArray("unit");

const QByteArray fdmPrinterJsonValueWarning            = QByteArray("@value_warning");
const QByteArray fdmPrinterJsonValueError              = QByteArray("@value_error");


const QByteArray fdmPrinterJsonTablePerMesh        = QByteArray("settable_per_mesh");
const QByteArray fdmPrinterJsonTablePerExtruder    = QByteArray("settable_per_extruder");
const QByteArray fdmPrinterJsonTablePerMeshgroup   = QByteArray("settable_per_meshgroup");
const QByteArray fdmPrinterJsonTableGlobally       = QByteArray("settable_globally");
const QByteArray fdmPrinterJsonLimitToExtruder     = QByteArray("limit_to_extruder");
const QByteArray fdmPrinterJsonEnabled             = QByteArray("enabled");
const QByteArray fdmPrinterJsonResolve             = QByteArray("resolve");

const QByteArray fdmPrinterJsonOptions             = QByteArray("options"); // QMap<QString, QString>

//for ui
const QByteArray fdmNodeIcon                    = QByteArray("#fdmNodeIcon");         //QString-- icon path
//const QByteArray fdmNodeCheckState              = QByteArray("#fdmNodeCheckState");  //bool true-checked false-unchecked
const QByteArray fdmNodeLevel                   = QByteArray("#fdmNodeLevel");        //int
const QByteArray fdmNodeIsVisibility            = QByteArray("#fdmNodeIsVisibility"); //bool
//const QByteArray fdmNodeInitialValue            = QByteArray("#fdmNodeInitialValue");
//const QByteArray fdmNodeRealValue               = QByteArray("#fdmNodeRealValue");
const QByteArray fdmNodeValueRangeFlag          = QByteArray("#fdmNodeValueRangeFlag"); //int/enum  -1--error(red)  0--normal  1-- warning(orange)
const QByteArray fdmNodeExpand                  = QByteArray("#fdmNodeExpand");  // type = category专有属性
const QByteArray fdmNodeRowInModelList          = QByteArray("#fdmNodeRowInModelList");  // type = category专有属性


}

#endif // FDMSETTINGSCOMMON_H
