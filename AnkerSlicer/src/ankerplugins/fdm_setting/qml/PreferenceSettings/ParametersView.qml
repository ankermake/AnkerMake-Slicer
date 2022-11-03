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

    property int height1 : 0

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
//        anchors.bottomMargin: 12
//        text: qsTr("Parameter Setting")
//    }

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
    }

    Rectangle {
        id:lines
        anchors.left: listview.right
        anchors.right: parent.right
        anchors.top: comboxLayout.bottom
        anchors.leftMargin: 24
        anchors.rightMargin: 24
        anchors.topMargin: 17
        color: "#393B44"
        height: 1
    }

//    ScrollView {
//        id: scrollView
//        anchors.top:lines.bottom
//        anchors.left: listview.right
//        anchors.right: parent.right
//        ScrollBar.horizontal.policy : ScrollBar.AlwaysOff
//        anchors.margins: 24
//        height: 400
        ListView {
            id: listView
            anchors.top:lines.bottom
            anchors.left: listview.right
            anchors.right: parent.right
           // anchors.margins: 24
            anchors.leftMargin: 24
            anchors.topMargin: 24
            anchors.bottomMargin: 24
            anchors.rightMargin: 6
            model : preferenceModel;
            implicitHeight: 340
            spacing: 0
            ScrollBar.vertical:  ScrollBar{
                policy: ScrollBar.AsNeeded
                anchors.right: listView.right
                width: 8
                active: true
                contentItem: Rectangle {
                    radius: width/2
                    color: '#D6D6D6'
                }
            }
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
    //}

}
