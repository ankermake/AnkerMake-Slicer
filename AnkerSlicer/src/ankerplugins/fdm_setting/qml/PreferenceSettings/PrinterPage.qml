import QtQuick 2.0
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.3
import "../PreferenceSettings" as FdmSetting
import "../BaseControl" as BaseControl
Rectangle {
    id:root
    color:  "#292A2D"
    anchors.topMargin: 0
    ScrollView {
        id:printerView
        ScrollBar.vertical.policy: ScrollBar.AlwaysOn
        anchors.fill: parent
        anchors.margins: 0
        wheelEnabled: true

        ColumnLayout {
            id:printerRectangle
           // anchors.fill: printerView
            spacing: 8

            FdmSetting.PreferencesTitle {
                id:printSettingsTitle
                text:qsTr("Printer Settings")
                height: 50
                Layout.preferredWidth: 460

            }

            Column {
                id:printSettingsParameters
                Layout.fillWidth: true
                spacing: 8

                FdmSetting.FdmInput {
                    Layout.fillWidth: true
                    value: machineList["machine_width"].fdmValueUI
                    unit: machineList["machine_width"].fdmUnit
                    lineLabel: qsTr("X (Width)")
                    inputEnable : (machineParameter.defaultMachineList).indexOf(machineParameter.currentMachineName) <0 ? true : false
                    onTextEditFinished: {
                        machineList["machine_width"].fdmValueUI = value
                    }
                }
                FdmSetting.FdmInput {
                    value: machineList["machine_depth"].fdmValueUI
                    unit: machineList["machine_depth"].fdmUnit
                    inputEnable : (machineParameter.defaultMachineList).indexOf(machineParameter.currentMachineName) <0 ? true : false
                    lineLabel: qsTr("Y (Depth)")
                    onTextEditFinished: {
                        machineList["machine_depth"].fdmValueUI = value
                    }
                }
                FdmSetting.FdmInput {
                    value: machineList["machine_height"].fdmValueUI
                    unit: machineList["machine_height"].fdmUnit
                    lineLabel: qsTr("Z (Height)")
                    inputEnable : (machineParameter.defaultMachineList).indexOf(machineParameter.currentMachineName) <0 ? true : false
                    onTextEditFinished: {
                        machineList["machine_height"].fdmValueUI = value
                    }
                }
                FdmSetting.FdmComboxLine {
                    lineLabel: qsTr("Build Plate Shape")
                    comboxCurrentValue: machineList["machine_shape"].fdmValueUI
                    comboboxModel: machineList["machine_shape"].fdmOptions
                    comboxEnable: (machineParameter.defaultMachineList).indexOf(machineParameter.currentMachineName) <0 ? true : false
                    onComboxCurrentTextChanged: {
                        machineList["machine_shape"].fdmValueUI = comboxCurrentText
                    }
                }
//                FdmSetting.FdmCheckLine {
//                    switchIsChecked: machineList["machine_center_is_zero"].fdmValueUI
//                    lineLabel: qsTr("Origin at Center")
//                    switchEnable: (machineParameter.defaultMachineList).indexOf(machineParameter.currentMachineName) <0 ? true : false
//                    onSwitchStateChanged: {
//                        machineList["machine_center_is_zero"].fdmValueUI = !machineList["machine_center_is_zero"].fdmValueUI
//                    }
//                }
                FdmSetting.FdmCheckLine {
                    switchIsChecked: machineList["machine_heated_bed"].fdmValueUI
                    lineLabel: qsTr("Heated Build Plate")
                    switchEnable: (machineParameter.defaultMachineList).indexOf(machineParameter.currentMachineName) <0 ? true : false
                    onSwitchStateChanged: {
                        machineList["machine_heated_bed"].fdmValueUI = !machineList["machine_heated_bed"].fdmValueUI
                    }
                }
                FdmSetting.FdmCheckLine {
                    switchIsChecked: machineList["machine_heated_build_volume"].fdmValueUI
                    lineLabel: qsTr("Build Temperature Stabilization")
                    switchEnable: (machineParameter.defaultMachineList).indexOf(machineParameter.currentMachineName) <0 ? true : false
                    onSwitchStateChanged: {
                        machineList["machine_heated_build_volume"].fdmValueUI = !machineList["machine_heated_build_volume"].fdmValueUI
                    }
                }
                FdmSetting.FdmComboxLine {
                    lineLabel: qsTr("G-Code Flavor")
                    comboxCurrentValue: machineList["machine_gcode_flavor"].fdmValueUI
                    comboboxModel: machineList["machine_gcode_flavor"].fdmOptions
                    comboxEnable: (machineParameter.defaultMachineList).indexOf(machineParameter.currentMachineName) <0 ? true : false

                    onComboxCurrentTextChanged: {
                        machineList["machine_gcode_flavor"].fdmValueUI = comboxCurrentText
                    }
                }
            }

            Connections{
                target: printSettingsTitle
                function onExpandCurrent(ifexpand) {
                    printSettingsParameters.visible = ! printSettingsParameters.visible
                    printSettingsParameters.height =  printSettingsParameters.visible ? (30 + printSettingsParameters.spacing - 1) *8  : 0
                }
            }
            Rectangle {
                id:printSettingsLines
                Layout.fillWidth: true
                color: "#393B44"
                height: 1
            }


            FdmSetting.PreferencesTitle {
                id:printhedSettingsTitle
                Layout.fillWidth: true
                text:qsTr("Printhead Settings")
                height: 20
            }

            Connections{
                target: printhedSettingsTitle
                function onExpandCurrent(ifexpand) {
                    printhedSettingsParameters.visible = !printhedSettingsParameters.visible
                    printhedSettingsParameters.height =  printhedSettingsParameters.visible ? 30*2: 0
                }
            }
            Column {
                id:printhedSettingsParameters
                Layout.fillWidth: true

                spacing: 8
                FdmSetting.FdmComboxLine {
                    id:extruderNumer
                    lineLabel: qsTr("Amount of Extruders")
                    comboxCurrentValue: machineList["machine_extruder_count"].fdmValueUI
                    comboboxModel: machineList["machine_extruder_count"].fdmOptions
                    comboxEnable: (machineParameter.defaultMachineList).indexOf(machineParameter.currentMachineName) <0 ? true : false
                    onComboxCurrentTextChanged: {
                        machineList["machine_extruder_count"].fdmValueUI = comboxCurrentText
                    }
                }
                FdmSetting.FdmCheckLine {
                    switchIsChecked: machineList["machine_use_extruder_offset_to_offset_coords"].fdmValueUI
                    lineLabel: qsTr("Apply Extruder Offsets to G-Code")
                    height: 30
                    switchEnable: (machineParameter.defaultMachineList).indexOf(machineParameter.currentMachineName) <0 ? true : false
                    onSwitchStateChanged: {
                        machineList["machine_use_extruder_offset_to_offset_coords"].fdmValueUI = !machineList["machine_use_extruder_offset_to_offset_coords"].fdmValueUI
                    }
                }
            }

            Rectangle {
                id:printhedSettingsParametersLines
                Layout.fillWidth: true
                color: "#393B44"
                height: 1
            }
            FdmSetting.PreferencesTitle {
                id:startGcodeTitle
                Layout.fillWidth: true
                text:qsTr("Start G-Code")
                height: 20
            }

            BaseControl.BaseTextEdit {
                id: startGcodeText
                height: 100
                Layout.preferredWidth: 460
                gCodetext: machineList["machine_start_gcode"].fdmValueUI
                textEnabled: (machineParameter.defaultMachineList).indexOf(machineParameter.currentMachineName) < 0 ? true : false
                onGCodeTextEditFinished: {
                    machineList["machine_start_gcode"].fdmValueUI = startGcodeText.gCodetext
                    console.log("(machineParameter.defaultMachineList).indexOf(machineParameter.currentMachineName) = ",(machineParameter.defaultMachineList).indexOf(machineParameter.currentMachineName))
                }
            }
            Connections{
                target: startGcodeTitle
                function onExpandCurrent(ifexpand) {
                    startGcodeText.visible = !startGcodeText.visible
                    startGcodeText.height =  startGcodeText.visible ? 96 : 0
                }
            }

            Rectangle {
                id:startGcodeTextLines
                Layout.fillWidth: true
                color: "#393B44"
                height: 1
            }

            FdmSetting.PreferencesTitle {
                id:endGcodeTitle
                Layout.fillWidth: true
                text:qsTr("End G-Code")
                height: 20
            }

            Connections{
                target: endGcodeTitle
                function onExpandCurrent(ifexpand) {
                    endGcodeText.visible = !endGcodeText.visible
                    endGcodeText.height =  endGcodeText.visible ? 96 : 0
                }
            }

            BaseControl.BaseTextEdit {
                id: endGcodeText
                Layout.fillWidth: true
                 width: 460
                height: 100
                gCodetext: machineList["machine_end_gcode"].fdmValueUI
                textEnabled: (machineParameter.defaultMachineList).indexOf(machineParameter.currentMachineName) <0 ? true : false
                onGCodeTextEditFinished: {
                    // console.log("doc = ", endGcodeText.gCodeDocument,"text ==" , endGcodeText.gCodetext)
                    machineList["machine_end_gcode"].fdmValueUI = endGcodeText.gCodetext
                }
            }

            Rectangle {
                id:endGcodeTextLines
                Layout.fillWidth: true
                color: "#393B44"
                height: 1
            }
        }
    }
}

/*##^##
Designer {
    D{i:0;autoSize:true;height:480;width:640}
}
##^##*/
