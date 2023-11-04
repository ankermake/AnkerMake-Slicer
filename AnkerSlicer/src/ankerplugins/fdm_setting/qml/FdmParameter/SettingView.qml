import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

import "../FdmParameter" as FdmSetting
import "../BaseControl" as BaseControl

Rectangle {
    id:settingView
    visible: true
    implicitWidth: 500
    implicitHeight: 986
    color: "#292A2D"
    property bool isSelected: false
    //property var qualityList: [qsTr("Nomal"),qsTr("High")]

    signal qmlSettingCurrentMachineChanged(string name)
    signal qmlSettingCurrentMaterialChanged(string name)
    signal qmlSettingCurrentParameterChanged(string name)
    signal qmlOpenMachinePreference(string name)
    signal qmlOpenMaterialPreference(string name)
    signal qmlOpenParameterPreference(string name)
    signal qmlPrintModeChanged(string name)
    signal qmlPrintModeIdxChanged(int index)
    signal qmlResetParameter()
    signal qmlAiQualityCurrentIdxChanged(int index)
    signal qmlAiInfillDensityChanged(double newAiInfillDensity)

    signal qmlLayerHeightCurrentIndexChanged (int index)
    signal qmlInfillCurrentIndexChanged (int index)
    signal qmlGlobalSupportChanged (int state)
    signal qmlGenerateAdhesionChanged (int state)

    signal qmlNozzleSizeCurrentIndexChanged(string currentText)

    //与C++中的保持一致 参考ak_const.h
    enum EGlobalSupportState
    {
        Unactived = 0,
        Selected ,
        UnSelected
    }
    //文案描述有4個
    enum EGlobalSupportTextCode
    {
        //手动支撑禁用全局支撑
        GlobalSupportUnactived = 0,
        //手动支撑没禁用全局支撑。全局支撑打开
        GlobalSupportAndManualSupport,
        //场景中没手动支撑.仅仅全局支撑会生效
        GlobalSupportOnly,
        //全局支撑关闭
        GlobalSupportUnSelected
    }

    //按钮状态有2个
    enum EGenerateAdhesionState
    {
        GenAdhesionSelected = 0,
        GenAdhesionUnSelected
    }
    RowLayout {
        id:settings
        anchors.left: parent.left
        anchors.top: parent.top
        anchors.right: parent.right
        anchors.margins: 16
        anchors.topMargin: 13
        spacing: 16

        FdmSetting.ComboxItem {
            id:printer
            objectName: "printerCombox"
            height: 55
            Layout.fillWidth: true
            Layout.fillHeight: true
            anchors.margins: 16
            title: qsTr("Machine");
            comBoxModel: rightParameter.machineList
            currentIndex: rightParameter.machineList.indexOf(rightParameter.machineName)

            onComboxActived: {
                if(currentIndex === rightParameter.machineList.length - 1) {
                    qmlOpenMachinePreference("");
                }
                qmlSettingCurrentMachineChanged(printer.currentText)
            }
            onComboxButtonClicked: {
                qmlOpenMachinePreference(name);
            }
        }

        FdmSetting.ComboxItem {
            id:nozzel
            Layout.fillWidth: true
            Layout.fillHeight: true
            height: 55
            title:qsTr("Nozzle")
            buttonAllVisble:true
            comBoxModel: rightParameter.nozzleSizeList
            currentIndex: rightParameter.nozzleSizeList.indexOf(rightParameter.nozzleSize)
            onComboxActived:  {
                qmlNozzleSizeCurrentIndexChanged(nozzel.currentText)
                //console.log("text ==" , nozzel.currentText )
            }
        }


    }
    FdmSetting.ComboxItem {
        id:material
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: settings.bottom
      //  anchors.margins: 16
        anchors.leftMargin: 16
        anchors.rightMargin: 16
        anchors.topMargin: 16
        anchors.bottomMargin: 0
        height: 55
        title:qsTr("Material")
        comBoxModel: rightParameter.materialList
        currentIndex: rightParameter.materialList.indexOf(rightParameter.materialName)
        onComboxActived:  {
            console.log("currentTextrightParameter.machineList.indexOf(rightParameter.machineName) =",printer.currentText)
            if(currentIndex === rightParameter.materialList.length - 1) {
                qmlOpenMaterialPreference("")
            }
            qmlSettingCurrentMaterialChanged(material.currentText)

        }
        onComboxButtonClicked: {
            qmlOpenMaterialPreference(name);
        }
    }
    Rectangle {
        id:lines
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: material.bottom
        anchors.topMargin: 20
        anchors.bottomMargin: 0
        color: "#383838"
        height: 1
    }

//    FdmSetting.ComboxItem {
//        id:printModeRect
//        anchors.left: parent.left
//        anchors.right: parent.right
//        anchors.top: settings.bottom
//        anchors.leftMargin: 16
//        anchors.rightMargin: 16
//        anchors.topMargin: 16
//        anchors.bottomMargin: 0
//        height: 55
//        title:qsTr("Speed")
//        comBoxModel: rightParameter.printModeList
//        currentIndex: rightParameter.printModeList.indexOf(rightParameter.printMode)
//        onComboxActived:  {
//            console.log("rightParameter.printMode is",rightParameter.printMode," currentText name is  ==",currentText,"currentIndex =",currentIndex)
//             qmlPrintModeIdxChanged(currentIndex);
//        }
//    }

    FdmSetting.ComboxItem {
        id:parameterCombox
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: lines.bottom
        anchors.leftMargin: 16
        anchors.rightMargin: 16
        anchors.topMargin: 20
        anchors.bottomMargin: 0
        height: 55
        title: qsTr("Parameters")
        comBoxModel: rightParameter.allParameterList
        currentIndex: rightParameter.allParameterList.indexOf(rightParameter.currentParameterName)
        buttonVisible : parameterCombox.currentIndex == 0 ? false : true
        preferencesBurronVisible: false
        onComboxActived: {
            // console.log("currentTextrightParameter.machineList.indexOf(rightParameter.machineName) =",rightParameter.allParameterList)
            if(currentIndex == rightParameter.allParameterList.length - 1) {
                qmlOpenParameterPreference("")
            }
            qmlSettingCurrentParameterChanged(parameterCombox.currentText)
        }
        onComboxButtonClicked: {
           // console.log("qml OPen click prenerence")
            qmlOpenParameterPreference(name);
        }
        onResetParameters: {
            qmlResetParameter()
        }
    }

    Rectangle{
        id: printModeRect
        height: 55
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: parameterCombox.bottom
        anchors.leftMargin: 16
        anchors.rightMargin: 16
        anchors.topMargin: 16
        anchors.bottomMargin: 0
        implicitWidth: parent.width
        Layout.fillWidth: true
        color: "transparent"
        visible: parameterCombox.currentIndex < 2 && rightParameter.printModeList.length>1
        Row{
            spacing:4
            id:printModeImgLabel
            //anchors.horizontalCenter: parent.horizontalCenter
            Label{
                id: speedLabelId
                color: "#FFFFFF"
                text: qsTr("Speed")
                font.pixelSize: 14
            }
            Image{
                height: 24
                width:  24
                anchors.top: speedLabelId.top
                source: "qrc:/images/images/fdm_question_icon.png"
                MouseArea {
                    anchors.fill: parent
                    propagateComposedEvents : true
                    hoverEnabled:true
                    onEntered: {
                       //console.log("enterrrrrrrrrrrrrrrrrr", "mouse_X = ", mouseX, "mouse_y = " ,mouseY, "gloabal = " , mapToGlobal(0,0));
                       //FdmParamSettingsWidget.showToolTip("<img src='qrc:/images/images/fdm_an_icon_n.png' />", "<img src='qrc:/images/images/fdm_an_icon_n.png' />", "", "", mapToGlobal(0,0));
                        FdmParamSettingsWidget.showPrintModeToolTip(speedLabelId.mapToGlobal(0,0));
                    }

                    onExited:  {
                       //console.log("exited................");
                        //FdmParamSettingsWidget.hideToolTip();
                        FdmParamSettingsWidget.hideHtmlToolTip();
                    }

                }
            }
        }

        BaseControl.DefaultComboBox{
        //BaseControl.BaseComboBox{
            id:printModeCombox
            anchors.top: printModeImgLabel.bottom
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.topMargin: 8
            //buttonAllVisble: true
            width: 226
            height: 30
            model:rightParameter.printModeList
            currentIndex:rightParameter.printModeList.indexOf(rightParameter.printMode)
            onActivated: {
               //updateLanguage();
               console.log("rightParameter.printMode is",rightParameter.printMode," currentText name is  ==",currentText,"currentIndex =",currentIndex)
               //qmlPrintModeChanged(currentText);
                qmlPrintModeIdxChanged(currentIndex);
            }

        }
    }

    Loader {
        id:modeLoader
        height: 0
        //anchors.top: parameterCombox.bottom
        anchors.top: printModeRect.visible ? printModeRect.bottom : parameterCombox.bottom
        anchors.right: parent.right
        anchors.left: parent.left
        anchors.bottom: parent.bottom
        anchors.leftMargin: 16
        anchors.rightMargin: parameterCombox.currentIndex === 0 ? 16 : 4
        anchors.topMargin: 20
        sourceComponent: parameterCombox.currentIndex == 0 ? simpleMode : expertMode
    }

    Component {
        id:simpleMode
        Column {
            spacing: 16
            Rectangle{
                height: 60
                anchors.leftMargin: 12
                implicitWidth: parent.width
                Layout.fillWidth: true
                color: "transparent"
                Label{
                    id:layerHeightLabel
                    anchors.top: parent.top
                    color: "#FFFFFF"
                    text: qsTr("Layer Height")
                }
                BaseControl.DefaultComboBox{
                    id:layerHeightCombox
                    anchors.top: layerHeightLabel.bottom
                    anchors.left: parent.left
                    anchors.right: parent.right
                    anchors.topMargin: 8
                    width: 226
                    height: 30
                    model:rightParameter.aiQualityList
                    currentIndex:rightParameter.aiQualityCurrentIdx
                    onActivated: {
                       // console.log("currnt ==",layerHeightCombox.currentIndex,"index ==")
                        qmlLayerHeightCurrentIndexChanged(layerHeightCombox.currentIndex)
                    }
                }


            }

            Rectangle{
                height: 60
                anchors.leftMargin: 12
                implicitWidth: parent.width
                Layout.fillWidth: true
                color: "transparent"
                visible: false
                Label{
                    id:infillThicknessLabel
                    anchors.top: parent.top
                    color: "#FFFFFF"
                    text: qsTr("Infill Layer Thickness")
                }
                BaseControl.BaseTextEdit{
                    id:infillThicknessLineEdit
                    anchors.top: infillThicknessLabel.bottom
                    anchors.left: parent.left
                    anchors.right: parent.right
                    anchors.topMargin: 8
                    width: 226
                    height: 30
                    gCodetext:rightParameter.infillThickness
                }
            }

            Rectangle{
                height: 60
                implicitWidth: parent.width
                Layout.fillWidth: true
                color: "transparent"

                Label{
                    id:infillLabel
                    anchors.top: parent.top
                    color: "#FFFFFF"
                    text: qsTr("Infill")
                }
                BaseControl.DefaultComboBox{
                    id:infillCombox
                    anchors.top: infillLabel.bottom
                    anchors.left: parent.left
                    anchors.right: parent.right
                    anchors.topMargin: 8
                    width: 226
                    height: 30
                    model:rightParameter.aiInfillDensityList

                    currentIndex:rightParameter.aiInfillDensityCurrentIdx
                    onActivated: {
                       qmlInfillCurrentIndexChanged(infillCombox.currentIndex)
                    }
                }

            }

            Rectangle {
                id:globalSupport
                implicitWidth: parent.width
                height: 40
                color: "transparent"
                Label {
                    id:title
                    text: qsTr("Global Support")
                    color: "#FFFFFF"
                }
                BaseControl.SwitchButton {
                    id:supportSwitch
                    anchors.right: globalSupport.right
                    anchors.top: globalSupport.top
                    enabled:  rightParameter.globalSupportState === 0 ? false : true
                    isChecked : rightParameter.globalSupportState === 1 ? true :false
                    onCheckedStateChanged : {
                        qmlGlobalSupportChanged((!supportSwitch.isChecked) ? 1 : 2)
                    }
                }
                Label {
                    anchors.top: title.bottom
                    anchors.left: globalSupport.left
                    color: "#999999"
                    text: rightParameter.globalSupportTextCode === 0 ? qsTr("Global Support have been disabled by Manual Support.") : (rightParameter.globalSupportTextCode === 1 ? qsTr("Global Support and Manual Support will take effect at the same time.") : qsTr(""))
                }
            }

            Rectangle {
                id:generateAdhesion
                implicitWidth: parent.width
                height: 20
                color: "transparent"
                Label {
                    text: qsTr("Start Adhesion")
                    color: "#FFFFFF"
                }
                BaseControl.SwitchButton {
                    id:generateAdensionSwitch
                    isChecked:  rightParameter.generateAdhesionState === 0 ? true :false
                    anchors.right: generateAdhesion.right
                    anchors.top: generateAdhesion.top
                    onCheckedStateChanged : {
                      qmlGenerateAdhesionChanged((!generateAdensionSwitch.isChecked) ? 0 : 1)
                    }
                }
            }
        }
    }
//    Component {
//        id:simpleModeSpeedy
//         Column {
//             FdmSetting.ComboxItem {
//                 id: speedyCombox
//                 anchors.left: parent.left
//                 anchors.right: parent.right
//                 anchors.top: lines.bottom
//                 anchors.leftMargin: 16
//                 anchors.rightMargin: 16
//                 anchors.topMargin: 20
//                 anchors.bottomMargin: 0
//                 height: 55
//                 title: qsTr("Speed")
//                 comBoxModel: rightParameter.allParameterList
//                 currentIndex: rightParameter.allParameterList.indexOf(rightParameter.currentParameterName)
//                 buttonVisible : parameterCombox.currentIndex === 0 ? false : true
//                 preferencesBurronVisible: false
//                 onComboxActived: {
//                     // console.log("currentTextrightParameter.machineList.indexOf(rightParameter.machineName) =",rightParameter.allParameterList)
//                     if(currentIndex === rightParameter.allParameterList.length - 1) {
//                         qmlOpenParameterPreference("")
//                     }
//                     qmlSettingCurrentParameterChanged(parameterCombox.currentText)
//                 }
//                 onComboxButtonClicked: {
//                    // console.log("qml OPen click prenerence")
//                     qmlOpenParameterPreference(name);
//                 }
//                 onResetParameters: {
//                     qmlResetParameter()
//                 }
//             }
//             Loader{
//                 sourceComponent:  simpleMode
//             }
//         }
//    }

    Component {
        id:simpleModeSpeedy
        Column {
            spacing: 16
            Rectangle{
                height: 60
                anchors.leftMargin: 12
                implicitWidth: parent.width
                Layout.fillWidth: true
                color: "transparent"
                Row{
                    spacing:4
                    id:printModeLabel
                    //anchors.horizontalCenter: parent.horizontalCenter
                    Label{
                        id: speedLabelId
                        color: "#FFFFFF"
                        text: qsTr("Speed")
                        font.pixelSize: 14
                    }
                    Image{
                        height: 24
                        width:  24
                        anchors.top: speedLabelId.top
                        source: "qrc:/images/images/fdm_question_icon.png"
                        MouseArea {
                            anchors.fill: parent
                            propagateComposedEvents : true
                            hoverEnabled:true
                            onEntered: {
                               //console.log("enterrrrrrrrrrrrrrrrrr", "mouse_X = ", mouseX, "mouse_y = " ,mouseY, "gloabal = " , mapToGlobal(0,0));
                               //FdmParamSettingsWidget.showToolTip("<img src='qrc:/images/images/fdm_an_icon_n.png' />", "<img src='qrc:/images/images/fdm_an_icon_n.png' />", "", "", mapToGlobal(0,0));
                                FdmParamSettingsWidget.showPrintModeToolTip(speedLabelId.mapToGlobal(0,0));
                            }

                            onExited:  {
                               //console.log("exited................");
                                //FdmParamSettingsWidget.hideToolTip();
                                FdmParamSettingsWidget.hideHtmlToolTip();
                            }

                        }
                    }

                }


                BaseControl.DefaultComboBox{
                    id:printModeCombox
                    anchors.top: printModeLabel.bottom
                    anchors.left: parent.left
                    anchors.right: parent.right
                    anchors.topMargin: 8
                    width: 226
                    height: 30
                    model:rightParameter.printModeList
                    currentIndex:rightParameter.printModeList.indexOf(rightParameter.printMode)
                    onActivated: {
                       //updateLanguage();
                       console.log("rightParameter.printMode is",rightParameter.printMode," currentText name is  ==",currentText,"currentIndex =",currentIndex)
                       //qmlPrintModeChanged(currentText);
                        qmlPrintModeIdxChanged(currentIndex);
                    }

                }
            }

            Rectangle{
                height: 60
                anchors.leftMargin: 12
                implicitWidth: parent.width
                Layout.fillWidth: true
                color: "transparent"
                Label{
                    id:layerHeightLabel
                    anchors.top: parent.top
                    color: "#FFFFFF"
                    text: qsTr("Layer Height")
                }
                BaseControl.DefaultComboBox{
                    id:layerHeightCombox
                    anchors.top: layerHeightLabel.bottom
                    anchors.left: parent.left
                    anchors.right: parent.right
                    anchors.topMargin: 8
                    width: 226
                    height: 30
                    model:rightParameter.aiQualityList
                    currentIndex:rightParameter.aiQualityCurrentIdx
                    onActivated: {
                       // console.log("currnt ==",layerHeightCombox.currentIndex,"index ==")
                        qmlLayerHeightCurrentIndexChanged(layerHeightCombox.currentIndex)
                    }
                }


            }

            Rectangle{
                height: 60
                implicitWidth: parent.width
                Layout.fillWidth: true
                color: "transparent"

                Label{
                    id:infillLabel
                    anchors.top: parent.top
                    color: "#FFFFFF"
                    text: qsTr("Infill")
                }
                BaseControl.DefaultComboBox{
                    id:infillCombox
                    anchors.top: infillLabel.bottom
                    anchors.left: parent.left
                    anchors.right: parent.right
                    anchors.topMargin: 8
                    width: 226
                    height: 30
                    model:rightParameter.aiInfillDensityList

                    currentIndex:rightParameter.aiInfillDensityCurrentIdx
                    onActivated: {
                       qmlInfillCurrentIndexChanged(infillCombox.currentIndex)
                    }
                }

            }

            Rectangle {
                id:globalSupport
                implicitWidth: parent.width
                height: 40
                color: "transparent"
                Label {
                    id:title
                    text: qsTr("Global Support")
                    color: "#FFFFFF"
                }
                BaseControl.SwitchButton {
                    id:supportSwitch
                    anchors.right: globalSupport.right
                    anchors.top: globalSupport.top
                    enabled:  rightParameter.globalSupportState === 0 ? false : true
                    isChecked : rightParameter.globalSupportState === 1 ? true :false
                    onCheckedStateChanged : {
                        qmlGlobalSupportChanged((!supportSwitch.isChecked) ? 1 : 2)
                    }
                }
                Label {
                    anchors.top: title.bottom
                    anchors.left: globalSupport.left
                    color: "#999999"
                    text: rightParameter.globalSupportTextCode === 0 ? qsTr("Global Support have been disabled by Manual Support.") : (rightParameter.globalSupportTextCode === 1 ? qsTr("Global Support and Manual Support will take effect at the same time.") : qsTr(""))
                }
            }

            Rectangle {
                id:generateAdhesion
                implicitWidth: parent.width
                height: 20
                color: "transparent"
                Label {
                    text: qsTr("Start Adhesion")
                    color: "#FFFFFF"
                }
                BaseControl.SwitchButton {
                    id:generateAdensionSwitch
                    isChecked:  rightParameter.generateAdhesionState === 0 ? true :false
                    anchors.right: generateAdhesion.right
                    anchors.top: generateAdhesion.top
                    onCheckedStateChanged : {
                      qmlGenerateAdhesionChanged((!generateAdensionSwitch.isChecked) ? 0 : 1)
                    }
                }
            }
        }
    }

    Component {
        id: expertMode
        BaseControl.BaseListView {
            id: listView
            model : paramModel;
            currentIndex : -1
            delegate: Loader
            {
                id:viewDelegate
                anchors { left: parent.left; right: parent.right }
               // height: model.visible ? column.implicitHeight + 4 : 0
//                width: listView.width
               height: model.visible ? 42 : 0
               // Behavior on height { NumberAnimation { duration: 10 } }
                opacity: model.visible ? 1 : 0
                Behavior on opacity { NumberAnimation { duration: 100 } }
                active: model.type !== undefined

                source:
                {
                    switch(model.type){
                    case "category" :
                        return "CategoryItem.qml" ;
                    default:
                        return "ParamItem.qml"
                    }
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
