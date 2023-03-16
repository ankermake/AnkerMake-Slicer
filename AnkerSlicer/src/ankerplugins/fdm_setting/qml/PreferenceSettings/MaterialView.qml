import QtQuick 2.0
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.3

import "../PreferenceSettings" as FdmSetting
Rectangle {
    id: root
    color: "#292A2D"
    signal qmlCurrentMaterialChanged(string name)
    signal qmlDeleteCustomMaterial(string name)
    signal qmlRenameCustomMaterial(string oldName)
    signal qmlCreateMaterial(string name)

    FdmSetting.PreferencesListView {
        id:listview
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.bottom: parent.bottom
        anchors.topMargin: 12
        anchors.leftMargin: 12
        width: 210
        customModel: materialParameter.customMaterialList
        defaultModel: materialParameter.defaultMaterialList
        defaultName : materialParameter.currentMaterialName

    }
    Connections {
        target: listview
        function onCurrentChanged(name) {
            qmlCurrentMaterialChanged(name)
        }
        function onCreate(name) {
            qmlCreateMaterial(name)
        }

        function onRenameCurrent (oldName){
            qmlRenameCustomMaterial(oldName)
        }
        function onDeleteCurrent(name) {
            qmlDeleteCustomMaterial(name)
        }
    }

    function getcurrentValue(node,name) {

    }
    Rectangle {
        id:parameter
        anchors.left: listview.right
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.leftMargin: 24
        anchors.rightMargin: 24
        color: "transparent"
        Label {
            id:globalLabel
            anchors.top: parent.top
            anchors.left:parent.left
            anchors.margins: 8
            font.weight: Font.Bold
            font.pixelSize: 16
            color: "#FFFFFF"
            text: qsTr("Global")
        }
        FdmSetting.FdmInput {
            id:materialFlow
            anchors.top: globalLabel.bottom
            anchors.left: parent.left
            anchors.right: parent.right
            height: 30
            anchors.margins: 7
            lineLabel: qsTr("Flow")
            inputEnable: (materialParameter.defaultMaterialList).indexOf(materialParameter.currentMaterialName) <0 ? true : false
            value: materialList["material_flow"].fdmValueUI
            unit: materialList["material_flow"].fdmUnit
            onTextEditFinished: {
                materialList["material_flow"].fdmValueUI = value
               // console.log("index======" ,(materialParameter.defaultMaterialList).indexOf(materialParameter.currentMaterialName))
            }
        }

        FdmSetting.FdmInput {
            id:temperature
            anchors.top: materialFlow.bottom
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.margins: 7
            inputEnable: (materialParameter.defaultMaterialList).indexOf(materialParameter.currentMaterialName) <0 ? true : false
            lineLabel: qsTr("Printing Temperature")
            value: materialList["material_print_temperature"].fdmValueUI
            unit: materialList["material_print_temperature"].fdmUnit
            onTextEditFinished: {
                materialList["material_print_temperature"].fdmValueUI = value
            }
        }
        FdmSetting.FdmInput {
            id:plate
            anchors.top: temperature.bottom
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.margins: 7
            inputEnable: (materialParameter.defaultMaterialList).indexOf(materialParameter.currentMaterialName) <0 ? true : false
            lineLabel: qsTr("Plate Temperature")
            value: materialList["material_bed_temperature"].fdmValueUI
            unit: materialList["material_bed_temperature"].fdmUnit
            onTextEditFinished: {
                materialList["material_bed_temperature"].fdmValueUI = value
            }
        }

        FdmSetting.FdmInput {
            id:speed
            anchors.top: plate.bottom
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.margins: 7
            inputEnable: (materialParameter.defaultMaterialList).indexOf(materialParameter.currentMaterialName) <0 ? true : false
            lineLabel: qsTr("Fan Speed")
            value: materialList["cool_fan_speed"].fdmValueUI
            unit:  materialList["cool_fan_speed"].fdmUnit
            onTextEditFinished: {
                materialList["cool_fan_speed"].fdmValueUI = value
            }
        }

        Rectangle {
            id:line
            anchors.top: speed.bottom
            anchors.left:parent.left
            anchors.right: parent.right
            anchors.leftMargin: 8
            anchors.topMargin: 17
            anchors.rightMargin: 8
            anchors.bottomMargin: 15
            color: "#393B44"
            height: 1
        }

        Label {
            id:initialLabel
            anchors.top: line.bottom
            anchors.left:parent.left
             font.weight: Font.Bold
            font.pixelSize: 16
            anchors.margins: 8
            color: "#FFFFFF"
            text: qsTr("Initial Layer")

        }
        FdmSetting.FdmInput {
            id:flowIni
            anchors.top: initialLabel.bottom
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.margins: 7
            inputEnable: (materialParameter.defaultMaterialList).indexOf(materialParameter.currentMaterialName) <0 ? true : false
            lineLabel: qsTr("Flow")
            value: materialList["material_flow_layer_0"].fdmValueUI
            unit:  materialList["material_flow_layer_0"].fdmUnit
            onTextEditFinished: {
                materialList["material_flow_layer_0"].fdmValueUI = value
            }
        }
        FdmSetting.FdmInput {
            id:te
            anchors.top: flowIni.bottom
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.margins: 7
            inputEnable: (materialParameter.defaultMaterialList).indexOf(materialParameter.currentMaterialName) <0 ? true : false
            lineLabel: qsTr("Printing Temperature")
            value: materialList["material_print_temperature_layer_0"].fdmValueUI
            unit:  materialList["material_print_temperature_layer_0"].fdmUnit
            onTextEditFinished: {
                materialList["material_print_temperature_layer_0"].fdmValueUI = value
            }
        }
        FdmSetting.FdmInput {
            id:pl
            anchors.top: te.bottom
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.margins: 7
            inputEnable: (materialParameter.defaultMaterialList).indexOf(materialParameter.currentMaterialName) <0 ? true : false
            lineLabel: qsTr("Plate Temperature")
            value: materialList["material_bed_temperature_layer_0"].fdmValueUI
            unit:  materialList["material_bed_temperature_layer_0"].fdmUnit
            onTextEditFinished: {
                materialList["material_bed_temperature_layer_0"].fdmValueUI = value
            }
        }


    }



}



/*##^##
Designer {
    D{i:0;autoSize:true;height:480;width:640}
}
##^##*/
