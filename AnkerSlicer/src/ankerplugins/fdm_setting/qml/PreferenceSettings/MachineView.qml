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


        FdmSetting.PrinterPage {
            id:printer
            property string title: qsTr("Printer")
            anchors.fill: parent
            anchors.margins: 0
        }

        FdmSetting.ExtruderPage {
            property string title: qsTr("Extruder")
            anchors.fill: parent
            anchors.margins: 0

        }
    }

}
/*##^##
Designer {
    D{i:0;formeditorZoom:0.75}
}
##^##*/
