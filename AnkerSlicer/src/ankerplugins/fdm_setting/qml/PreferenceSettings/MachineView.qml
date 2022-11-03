import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.3

import "../PreferenceSettings" as FdmSetting
import "../BaseControl" as BaseControl
Rectangle {
    id: root
    color: "#292A2D"
    signal qmlCurrentMachineChanged(string name)
    signal qmlDeleteCustomMachine(string name)
    signal qmlRenameCustomMachine(string oldName)
    signal qmlCreateMachine(string name)
    signal qmlCurrentTabChanged(int dex)

//    Label {
//        id:titleLabel
//        anchors.left: parent.left
//        anchors.top: parent.top
//        horizontalAlignment: Text.AlignLeft
//        verticalAlignment: Text.AlignVCenter
//        font.pixelSize: 18
//        font.weight: Font.Bold
//        anchors.topMargin: 12
//        anchors.leftMargin: 12
//        text: qsTr("Machine Setting")
//    }


    FdmSetting.PreferencesListView {
        id:listview
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.bottom: parent.bottom
        anchors.topMargin: 12
        anchors.leftMargin: 12
        width: 210
        defaultModel: machineParameter.defaultMachineList
        customModel: machineParameter.customMachineList
        defaultName : machineParameter.currentMachineName
    }
    Connections {
        target: listview
        function onCurrentChanged(name) {
            qmlCurrentMachineChanged(name)
        }
        function onCreate(name) {
            qmlCreateMachine(name)
        }

        function onRenameCurrent (oldName){
            qmlRenameCustomMachine(oldName)
        }
        function onDeleteCurrent(name) {
            qmlDeleteCustomMachine(name)
        }
    }

    Connections{
        target: extr
        function onCurrentTabChanged(dex) {
            qmlCurrentTabChanged(dex)
        }
    }
    FdmSetting.TabView {
        id: extr
        anchors.top: parent.top
        anchors.left: listview.right
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        anchors.leftMargin: 24
        anchors.rightMargin: 8
        anchors.topMargin: 12

        Rectangle {
            id:printer
            property string title: qsTr("Printer")
            anchors.fill: parent
            anchors.leftMargin: 0
            anchors.rightMargin: 0
            anchors.topMargin: 16
            anchors.bottomMargin: 0
            color: "transparent"
           // border.width: 1
           // border.color: "yellow"

                ScrollView {
                    id:printerView
                    //                ScrollBar.horizontal.policy: ScrollBar.AlwaysOff
                    ScrollBar.vertical.policy: ScrollBar.AlwaysOn
                    anchors.fill: parent
                    anchors.margins: 0

                    ColumnLayout {
                        id:printerRectangle
                        spacing: 8

                        FdmSetting.PreferencesTitle {
                            id:printSettingsTitle
                            text:qsTr("Printer Settings")
                            height: 50
                            Layout.preferredWidth: 460

                        }

                        Column {
                            id:printSettingsParameters
//                            anchors.top: printSettingsTitle.bottom
//                            anchors.left: printerRectangle.left
//                            anchors.right: printerRectangle.right
                            //  anchors.fill: parent
                            //anchors.topMargin: 10
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
                            FdmSetting.FdmCheckLine {
                                switchIsChecked: machineList["machine_center_is_zero"].fdmValueUI
                                lineLabel: qsTr("Origin at Center")
                                switchEnable: (machineParameter.defaultMachineList).indexOf(machineParameter.currentMachineName) <0 ? true : false
                                onSwitchStateChanged: {
                                    machineList["machine_center_is_zero"].fdmValueUI = !machineList["machine_center_is_zero"].fdmValueUI
                                }
                            }
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
                                //console.log("printhedSettingsTitle  expand ==", ifexpand)
                                // printSettingsParameters.height  = expand ? parent.height - 60 : 0
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

            //}
        }
        Repeater {
            id:extruder
            anchors.fill: parent
            anchors.topMargin: 16
            model:1/* machineList["machine_extruder_count"].fdmValueUI*/
            Rectangle {
                property string title: qsTr("Extruder " + (index + 1))
                anchors.fill: parent
                anchors.topMargin: 16
                color: "transparent"

                FdmSetting.PreferencesTitle {
                    id:nozzleTitle
                    anchors.left: parent.left
                    anchors.right: parent.right
                    anchors.top: parent.top
                    text:qsTr("Nozzle Settings")
                    height: 20
                }

                Column {
                    id:nozzleSettings
                    anchors.top: nozzleTitle.bottom
                    anchors.left: parent.left
                    anchors.right: parent.right
                    anchors.topMargin: 10
                    spacing: 8
                    FdmSetting.FdmInput {
                        value: machineList["machine_nozzle_size"].fdmValueUI
                        unit: machineList["machine_nozzle_size"].fdmUnit
                        lineLabel: qsTr("Compatible Material Diameter")
                        inputEnable : (machineParameter.defaultMachineList).indexOf(machineParameter.currentMachineName) <0 ? true : false
                        onTextEditFinished: {

                            machineList["machine_nozzle_size"].fdmValueUI = value
                            console.log("machine_nozzle_size ==" ,value )
                        }
                    }
                    FdmSetting.FdmInput {
                        value: machineList["machine_nozzle_offset_x"].fdmValueUI
                        unit: machineList["machine_nozzle_offset_x"].fdmUnit
                        lineLabel: qsTr("Nozzle Offset X")
                        inputEnable : (machineParameter.defaultMachineList).indexOf(machineParameter.currentMachineName) <0 ? true : false
                        onTextEditFinished: {
                            machineList["machine_nozzle_offset_x"].fdmValueUI = value
                        }
                    }
                    FdmSetting.FdmInput {
                        value: machineList["machine_nozzle_offset_y"].fdmValueUI
                        unit: machineList["machine_nozzle_offset_y"].fdmUnit
                        lineLabel: qsTr("Nozzle Offset Y")
                        inputEnable : (machineParameter.defaultMachineList).indexOf(machineParameter.currentMachineName) <0 ? true : false
                        onTextEditFinished: {
                            machineList["machine_nozzle_offset_y"].fdmValueUI = value
                        }
                    }
                    FdmSetting.FdmInput {
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
                        // console.log("nozzleSettings  == " , nozzleSettings.visible )
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
        }
    }


}
/*##^##
Designer {
    D{i:0;formeditorZoom:0.75}
}
##^##*/
