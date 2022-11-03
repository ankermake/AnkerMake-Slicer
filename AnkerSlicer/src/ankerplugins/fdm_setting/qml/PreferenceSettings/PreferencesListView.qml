import QtQuick 2.0
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.3
import QtQml.Models 2.15
import QtQuick.Controls.Styles 1.4

Rectangle{
    id:defalutPart
    radius: 8
    color: "#323437"
    implicitWidth: 210
    property alias defaultModel : defaultListview.model
    property alias customModel : customListview.model
    property color backgroundColor: "#f5f6f7"

    property  string defaultName
    property  bool portButtonVisible : false
    signal currentChanged(string name)
    signal create(string name)
    signal renameCurrent(string name)
    signal deleteCurrent(string name)
    signal importParameter()
    signal exportParameter(string name)

    Label {
        id:defaultLabel
        anchors.left: defalutPart.left
        anchors.right: defalutPart.right
        anchors.top: defalutPart.top
        anchors.margins: 12
        font.weight: Font.Bold
        color: "#999999"
        text: qsTr("Default")
    }
    ListView {
        id : defaultListview
        anchors.top: defaultLabel.bottom
        anchors.left: defalutPart.left
        anchors.right: defalutPart.right
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
                color:/* defaultListview.currentIndex == index ?*/ "#FFFFFF"/* : "#333333"*/
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
        anchors.left: defalutPart.left
        anchors.right: defalutPart.right
        anchors.top: defaultListview.bottom
        anchors.margins: 12
        font.weight: Font.Bold
        color: "#999999"
        text: qsTr("Custom")
    }


    ListView {
        id : customListview
        anchors.top: customLabel.bottom
        anchors.left: defalutPart.left
        anchors.right: defalutPart.right
       // implicitHeight: customModel !== undefined ? customModel.length * (32 + customListview.spacing + 1) :0
        implicitHeight: 340
       // implicitHeight: parent.height - defaultLabel.height - defaultListview.height - customLabel.height - createButton.height -importButton.height ;
        ScrollBar.vertical:  ScrollBar{
            policy: ScrollBar.AsNeeded
            anchors.right: customListview.right
            width: 8
            active: true
            contentItem: Rectangle {
                radius: width/2
                color: '#797B80'
            }
        }
        anchors.topMargin: 12
        delegate:  customItem
        model: customModel
        focus: true
        currentIndex:  customModel.indexOf(defaultName)

        highlight: Rectangle {
            color:  "#404249"
        }
    }

    Button {
        id:importButton
        anchors.left: parent.left
        anchors.bottom: createButton.top
        //anchors.top: customListview.bottom
        anchors.margins: 12
        height: 30
        width: 89
        visible: portButtonVisible
        text: qsTr("Import")

        background: Rectangle {
            radius: 4
            color: "#4B4D53"
        }
        contentItem: Text {
            id: text2
            text: qsTr("Import")
            color: "#FFFFFF"
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            //elide: Text.ElideRight
        }
        onClicked: {
            importParameter()
        }
    }
    Button {
        id:exportButton
        anchors.right: parent.right
        anchors.left: importButton.right
        anchors.bottom: createButton.top
       // anchors.top: customListview.bottom
        anchors.margins: 12
        height: 30
        width: importButton.width
        visible: portButtonVisible
       // text: qsTr("Export")
        contentItem: Text {
            text: qsTr("Export")
            color: "#FFFFFF"
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            //elide: Text.ElideRight
        }
        background: Rectangle {
            radius: 5
            color: "#4B4D53"
        }
        onClicked: {
            exportParameter(defaultName)
        }
    }
    Button {
        id:createButton
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        //anchors.top: exportButton.bottom
        anchors.margins: 12
        height: 30
       // text: qsTr("Create")
        contentItem: Text {
            text: qsTr("Create")
            color: "#FFFFFF"
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
        }
        background: Rectangle {
            radius: 5
            color: "#4B4D53"
        }
        onClicked: {
            if(defaultListview.currentIndex != -1) {
                create(defaultModel[defaultListview.currentIndex])
            }
            else if(customListview.currentIndex != -1){
                create(customModel[customListview.currentIndex])
            }
        }

    }

    Component {
        id:customItem
        Item {
            id : root
            width:defalutPart.width
            height: 32
            visible: true
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
                    width: 108
                    elide: Text.ElideRight
                    color: /*customListview.currentIndex == index ? */"#FFFFFF"/* : "#333333"*/
                }
                Button{
                    id:deleteButton
                    anchors.top:parent.top
                    anchors.right: parent.right
                    anchors.bottom: parent.bottom
                    anchors.leftMargin: 2
                    anchors.rightMargin: 16
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
                    anchors.margins: 2
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
    D{i:0;autoSize:true;formeditorZoom:1.33;height:480;width:640}
}
##^##*/
