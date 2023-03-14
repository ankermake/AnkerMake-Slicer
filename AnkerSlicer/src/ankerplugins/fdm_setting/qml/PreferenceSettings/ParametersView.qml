import QtQuick 2.0
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.3

import "../PreferenceSettings" as FdmSetting
import "../BaseControl" as BaseControl
Rectangle {
    id: root
    color: "#292A2D"

    signal qmlCurrentParameterChanged(string name)
    signal qmlDeleteCustomParameter(string name)
    signal qmlRenameCustomParameter(string oldName)
    signal qmlCreateParameter(string name)
    signal qmlImportButtonClicked()
    signal qmlExportButtonClicked(string name)
    signal qmlMachineNameChanged(string name)
    signal qmlMaterialNameChanged(string name)
    signal qmlNozzleSizeChanged(string nozzleSize)

    FdmSetting.PreferencesListView {
        id:listview
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.bottom: parent.bottom
        anchors.topMargin: 12
        anchors.leftMargin: 12
        width: 210
        customModel: parameter.customParameterList
        defaultModel: parameter.defaultParameterList
        defaultName: parameter.currentParameterName
        portButtonVisible: true

    }
    Connections {
        target: listview
        function onCurrentChanged(name) {
            qmlCurrentParameterChanged(name)
        }
        function onCreate(name) {
            qmlCreateParameter(name)
        }

        function onRenameCurrent (oldName){
            qmlRenameCustomParameter(oldName)
        }
        function onDeleteCurrent(name) {
            qmlDeleteCustomParameter(name)
        }
        function onImportParameter() {
            qmlImportButtonClicked()
        }
        function onExportParameter(name) {
            qmlExportButtonClicked(name)
        }
    }

    RowLayout{
        id:comboxLayout
        anchors.left: listview.right
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.leftMargin: 24
        anchors.rightMargin: 24
        spacing: 16
        Rectangle{
            implicitHeight: 70
            Layout.fillWidth: true
            color: "transparent"
            Label{
                id:currentTitle
                anchors.top: parent.top
                color:"#FFFFFF"
                text: qsTr("Machine")
            }
            BaseControl.DefaultComboBox{
                id:machineCombox
                anchors.top: currentTitle.bottom
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.topMargin: 8
                width: 226
                height: 40
                model:parameter.machineList
                currentIndex: parameter.machineList.indexOf(parameter.machineName)
                onActivated: {
                    qmlMachineNameChanged(machineCombox.currentText)
                }
            }
        }
        Rectangle{
            implicitHeight: 70
            Layout.fillWidth: true
            color: "transparent"
            Label{
                id:nozzle
                anchors.top: parent.top
                color: "#FFFFFF"
                text: qsTr("Nozzle")
            }
            BaseControl.DefaultComboBox{
                id:nozzleCombox
                anchors.top: nozzle.bottom
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.topMargin: 8
                width: 226
                height: 40
                model:parameter.nozzleSizeList
                currentIndex:parameter.nozzleSizeList.indexOf(parameter.nozzleSize)
                onActivated: {
                    qmlNozzleSizeChanged(nozzleCombox.currentText)
                }
            }

        }
    }

    Rectangle{
        id:materialRectangle
        anchors.left: listview.right
        anchors.right: parent.right
        anchors.top: comboxLayout.bottom
        anchors.leftMargin: 24
        anchors.rightMargin: 24
        implicitHeight: 70
        Layout.fillWidth: true
        color: "transparent"
        Label{
            id:material
            anchors.top: parent.top
            color: "#FFFFFF"
            text: qsTr("Material")
        }
        BaseControl.DefaultComboBox{
            id:materialCombox
            anchors.top: material.bottom
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.topMargin: 8
            width: 226
            height: 40
            model:parameter.materialList
            currentIndex:parameter.materialList.indexOf(parameter.materialName)
            onActivated: {
                qmlMaterialNameChanged(materialCombox.currentText)
            }
        }

    }
    Rectangle {
        id:lines
        anchors.left: listview.right
        anchors.right: parent.right
        anchors.top: materialRectangle.bottom
        anchors.leftMargin: 24
        anchors.rightMargin: 24
        anchors.topMargin: 17
        color: "#393B44"
        height: 1
    }
    BaseControl.BaseListView {
        id: listView
        anchors.top:lines.bottom
        anchors.left: listview.right
        anchors.right: parent.right
        anchors.leftMargin: 24
        anchors.topMargin: 24
        anchors.bottomMargin: 24
        anchors.rightMargin: 6
        model : preferenceModel;
        implicitHeight: 340
        currentIndex: -1

        delegate: Loader
        {
            id:viewDelegate
            width: listView.width - 14
            height: model.visible ? 42 : 0
            opacity: model.visible ? 1 : 0
            active: model.type !== undefined

            source:
            {
                switch(model.type){
                case "category" :
                    return "qrc:/qml/PreferenceSettings/CategoryItem.qml" ;
                default:
                    return "qrc:/qml/PreferenceSettings/ParamItem.qml"
                }
            }
        }
    }
}
