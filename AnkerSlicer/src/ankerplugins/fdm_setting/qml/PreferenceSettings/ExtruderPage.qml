import QtQuick 2.0
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.3

import "../PreferenceSettings" as FdmSetting
import "../BaseControl" as BaseControl
Rectangle {
    anchors.fill: parent
    anchors.margins: 0
    color: "#292A2D"
    FdmSetting.PreferencesTitle {
        id:nozzleTitle
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: parent.top
        text:qsTr("Nozzle Settings")
        height: 20
    }

    ColumnLayout {
        id:nozzleSettings
        anchors.top: nozzleTitle.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.topMargin: 10
        anchors.rightMargin: 0
        spacing: 8
//        FdmSetting.FdmInput {
//            Layout.fillWidth: true
//            value: machineList["machine_nozzle_size"].fdmValueUI
//            unit: machineList["machine_nozzle_size"].fdmUnit
//            lineLabel: qsTr("Compatible Material Diameter")
//            inputEnable : (machineParameter.defaultMachineList).indexOf(machineParameter.currentMachineName) <0 ? true : false
//            onTextEditFinished: {

//                machineList["machine_nozzle_size"].fdmValueUI = value
//                console.log("machine_nozzle_size ==" ,value )
//            }
//        }
        FdmSetting.FdmInput {
             Layout.fillWidth: true
            value: machineList["machine_nozzle_offset_x"].fdmValueUI
            unit: machineList["machine_nozzle_offset_x"].fdmUnit
            lineLabel: qsTr("Nozzle Offset X")
            inputEnable : (machineParameter.defaultMachineList).indexOf(machineParameter.currentMachineName) <0 ? true : false
            onTextEditFinished: {
                machineList["machine_nozzle_offset_x"].fdmValueUI = value
            }
        }
        FdmSetting.FdmInput {
             Layout.fillWidth: true
            value: machineList["machine_nozzle_offset_y"].fdmValueUI
            unit: machineList["machine_nozzle_offset_y"].fdmUnit
            lineLabel: qsTr("Nozzle Offset Y")
            inputEnable : (machineParameter.defaultMachineList).indexOf(machineParameter.currentMachineName) <0 ? true : false
            onTextEditFinished: {
                machineList["machine_nozzle_offset_y"].fdmValueUI = value
            }
        }
        FdmSetting.FdmInput {
             Layout.fillWidth: true
            value: machineList["machine_extruder_cooling_fan_number"].fdmValueUI
            unit: machineList["machine_extruder_cooling_fan_number"].fdmUnit
            lineLabel: qsTr("Cooling Fan Number")
            inputEnable : (machineParameter.defaultMachineList).indexOf(machineParameter.currentMachineName) <0 ? true : false
            onTextEditFinished: {
                machineList["machine_extruder_cooling_fan_number"].fdmValueUI = value
            }
        }
    }

    Connections{
        target: nozzleTitle
        function onExpandCurrent(ifexpand) {
            nozzleSettings.visible = !nozzleSettings.visible
            // console.log("nozzleSettings  === " , nozzleSettings.visible )
            nozzleSettings.height =  nozzleSettings.visible ? (30 + nozzleSettings.spacing - 1) *4 : 0
        }
    }

    Rectangle {
        id:nozzleSettingsLines
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: nozzleSettings.bottom
        anchors.topMargin: 17
        color: "#393B44"
        height: 1
    }

    FdmSetting.PreferencesTitle {
        id:extruderStartGcodeTitle
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: nozzleSettingsLines.bottom
        anchors.topMargin: 15
        text:qsTr("Start G-Code")
        height: 20

    }

    BaseControl.BaseTextEdit {
        id: extruderStartGcodeText
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: extruderStartGcodeTitle.bottom
        anchors.topMargin: 8
        height: 100
        gCodetext: machineList["machine_extruder_start_code"].fdmValueUI
        textEnabled: (machineParameter.defaultMachineList).indexOf(machineParameter.currentMachineName) <0 ? true : false
        onGCodeTextEditFinished: {
            // console.log("doc = ", endGcodeText.gCodeDocument,"text ==" , endGcodeText.gCodetext)
            machineList["machine_extruder_start_code"].fdmValueUI = extruderStartGcodeText.gCodetext
        }
    }
    Connections{
        target: extruderStartGcodeTitle
        function onExpandCurrent(ifexpand) {
            extruderStartGcodeText.visible = !extruderStartGcodeText.visible
            extruderStartGcodeText.height =  extruderStartGcodeText.visible ? 96 : 0
        }
    }

    Rectangle {
        id:extruderStartGcodeTextLines
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: extruderStartGcodeText.bottom
        anchors.topMargin: 17
        color: "#393B44"
        height: 1
    }

    FdmSetting.PreferencesTitle {
        id:extruderEndGcodeTitle
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: extruderStartGcodeTextLines.bottom
        anchors.topMargin: 15
        text:qsTr("End G-Code")
        height: 20
    }

    Connections{
        target: extruderEndGcodeTitle
        function onExpandCurrent(ifexpand) {
            extruderEndGcodeText.visible = !extruderEndGcodeText.visible
            extruderEndGcodeText.height =  extruderEndGcodeText.visible ? 96 : 0
        }
    }

    BaseControl.BaseTextEdit {
        id: extruderEndGcodeText
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: extruderEndGcodeTitle.bottom
        anchors.topMargin: 8
        height: 100
        gCodetext: machineList["machine_extruder_end_code"].fdmValueUI
        textEnabled: (machineParameter.defaultMachineList).indexOf(machineParameter.currentMachineName) <0 ? true : false
        onGCodeTextEditFinished: {
            // console.log("doc = ", endGcodeText.gCodeDocument,"text ==" , endGcodeText.gCodetext)
            machineList["machine_extruder_end_code"].fdmValueUI = extruderEndGcodeText.gCodetext
        }
    }
    Rectangle {
        id:extruderEndGcodeTextLines
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: extruderEndGcodeText.bottom
        anchors.topMargin: 17
        color: "#393B44"
        height: 1
    }

}

/*##^##
Designer {
    D{i:0;autoSize:true;height:480;width:640}
}
##^##*/
