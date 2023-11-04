import QtQuick 2.0
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.3
import QtQml.Models 2.15
import QtQuick.Controls.Styles 1.4
import "../BaseControl" as BaseControl
Rectangle{
    id:defalutPart
    radius: 8
    color: "#323437"

    implicitWidth: 210
    property alias defaultModel : defaultListview.model
    property alias customModel : customListview.model
    property color backgroundColor: "#f5f6f7"
    property color buttonBackgroundColor: "#4B4D53"
    property color buttonTextColor:"#FFFFFF"
    property color buttonHoveredTextColor: "#71D78A"
    property color buttonPressedTextColor: "#71D78A"
    property int itemHeight: 32
    property  string defaultName
    property  bool portButtonVisible : false
    signal currentChanged(string name)
    signal create(string name)
    signal renameCurrent(string name)
    signal deleteCurrent(string name)
    signal importParameter()
    signal exportParameter(string name)

    ColumnLayout {
        id:mainLayout
        anchors.fill: parent
        anchors.margins: 12
        spacing: 12
        Label {
            id:defaultLabel
            Layout.fillWidth: true
            height:itemHeight
            verticalAlignment: Text.AlignVCenter
            font.weight: Font.Bold
            color: "#999999"
            text: qsTr("Default")
        }

        ListView {
            id : defaultListview
            Layout.fillWidth: true
            Layout.leftMargin: -12
            Layout.rightMargin: -12
            implicitHeight: defaultModel !== undefined ? defaultModel.length * (32 + defaultListview.spacing + 1) :0
            anchors.topMargin: 12
            model:  defaultModel
            focus: true
            currentIndex:  defaultModel.indexOf(defaultName)

            delegate: Item {
                width: 210
                height: 32
                Label{
                    id:defaultText
                    text:  modelData
                    anchors.fill: parent
                    anchors.leftMargin: 12
                    verticalAlignment: Text.AlignVCenter
                    elide: Text.ElideRight
                    color: "#FFFFFF"
                }
                MouseArea {
                    anchors.fill: parent
                    onClicked:  {
                        currentChanged(defaultText.text)
                        //console.log("currentChanged ", defaultText.text)
                    }
                }
            }
            highlight: Rectangle {
                color:  "#404249"
            }
        }

        Label {
            id:customLabel
            Layout.fillWidth: true
            Layout.topMargin: 6
            Layout.bottomMargin: 6
            horizontalAlignment: Text.AlignLeft
            verticalAlignment: Text.AlignVCenter
            font.weight: Font.Bold
            color: "#999999"
            text: qsTr("Custom")
        }

        BaseControl.BaseListView {
            id : customListview
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.leftMargin: -12
            Layout.rightMargin: -12
            delegate:  customItem
            model: customModel
            focus: true
            currentIndex:  customModel.indexOf(defaultName)
            Component.onCompleted: {
                console.log("customListview customModel =" ,customModel ,"defaultName = ", defaultName)
               console.log("contentHeight =", customListview.contentHeight,"listview height =",customListview.height)
            }
        }

        Rectangle {
            id:importAndExportRectangle
            Layout.alignment: Qt.AlignBottom | Qt.AlignVCenter
            Layout.fillWidth: true
            Layout.margins: 0
            implicitHeight: portButtonVisible ? itemHeight: 0
            color: "transparent"
            RowLayout {
                id: importAndExportLayout
                anchors.fill: importAndExportRectangle
                spacing:12
                Button {
                    id:importButton
                    Layout.fillWidth: true
                    visible: portButtonVisible
                    background: Rectangle {
                        radius: 4
                        color: buttonBackgroundColor
                        border.width: (importButton.hovered |importButton.pressed) ? 1 :0
                        border.color: buttonHoveredTextColor
                    }
                    contentItem: Text {
                        id: text2
                        text: qsTr("Import")
                        color:(importButton.hovered |importButton.pressed) ? "#71D78A" : "#FFFFFF"
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                    }
                    onClicked: {
                        importParameter()
                    }
                }
                Button {
                    id:exportButton
                    visible: portButtonVisible
                    Layout.fillWidth: true
                    contentItem: Text {
                        text: qsTr("Export")
                        color: (exportButton.hovered |exportButton.pressed) ? "#71D78A" : "#FFFFFF"
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                    }
                    background: Rectangle {
                        radius: 5
                        color: "#4B4D53"
                        border.width: (exportButton.hovered |exportButton.pressed) ? 1 :0
                        border.color: buttonHoveredTextColor
                    }
                    onClicked: {
                        exportParameter(defaultName)
                    }
                }
            }
        }
        Button {
            id:createButton
            Layout.alignment: Qt.AlignBottom | Qt.AlignVCenter
            Layout.fillWidth: true
            implicitHeight: itemHeight
            contentItem: Text {
                text: qsTr("Create")
               color:createButton.hovered ? "#71D78A" : "#FFFFFF"
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
            }
            background: Rectangle {
                radius: 5
                color: "#4B4D53"
                border.width: (createButton.hovered |createButton.pressed) ? 1 :0
                border.color: buttonHoveredTextColor
            }
            onClicked: {
                if(defaultListview.currentIndex !== -1) {
                    create(defaultModel[defaultListview.currentIndex])
                }
                else if(customListview.currentIndex !== -1){
                    create(customModel[customListview.currentIndex])
                }
            }

        }
    }

    Component {
        id:defaultItem
        Item {
            id:defaultItem1
            width:defalutPart.width
            implicitHeight: itemHeight
            visible: true
//            Component.onCompleted: {
//                console.log("defaultListview text =", defaultListview.modelData , "model ==", defaultModel , "defaultText =",defaultText.text)
//            }
            Label{
                id:defaultText
                text: modelData
                width: 100
                height: 32
                verticalAlignment: Text.AlignVCenter
                elide: Text.ElideRight
                color:"#FFFFFF"

            }
            MouseArea {
                anchors.fill: defaultItem1
                onClicked:  {
                    currentChanged(defaultText.text)
                }
            }
        }
    }
    Component {
        id:customItem
        Item {
            id : root
            width:defalutPart.width
            implicitHeight: itemHeight
            visible: true
//            Component.onCompleted:  {
//                console.log("customItem modelData==", modelData)
//            }
            MouseArea {
                anchors.fill: parent
                onClicked:  {
                    currentChanged(customText.text)
                }
                Label {
                    id:customText
                    anchors.left: parent.left
                    anchors.top: parent.top
                    anchors.bottom: parent.bottom
                   verticalAlignment: Text.AlignVCenter
                   anchors.leftMargin: 12
                   text:  modelData
                   width: 100
                    elide: Text.ElideRight
                    color: "#FFFFFF"
                }
                Button{
                    id:deleteButton
                    anchors.top:parent.top
                    anchors.right: parent.right
                    anchors.bottom: parent.bottom
                    anchors.rightMargin: 12
                    width: 20
                    height: 20
                    opacity: 0.6
                    background: Image {
                        id: name
                        fillMode:Image.PreserveAspectFit
                        width: implicitHeight
                        source: "qrc:/images/images/fdm_delete_icon.png"
                    }
                    onClicked: {
                        deleteCurrent(customModel[index])
                    }
                }

                Button{
                    anchors.top:parent.top
                    anchors.right: deleteButton.left
                    anchors.bottom: parent.bottom
                    width: 20
                    height: 20
                    opacity: 0.6
                    background: Image {
                        fillMode:Image.PreserveAspectFit
                        width: implicitHeight
                        source: "qrc:/images/images/preferences_editor_icon.png"
                    }
                    onClicked: {
                        renameCurrent(customModel[index])
                    }
                }

            }
        }
    }

}



/*##^##
Designer {
    D{i:0;autoSize:true;formeditorZoom:1.33;height:480;width:210}
}
##^##*/
