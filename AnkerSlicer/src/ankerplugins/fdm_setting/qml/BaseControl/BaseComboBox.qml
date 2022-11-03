import QtQuick 2.0
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.3

ComboBox {
    id: control
    implicitHeight: 30

    signal buttonClicked(string name)
    property bool buttonVisible: true
    property bool buttonAllVisble: false
    property alias backgroundColor: backgroundRectangle.color
    delegate: ItemDelegate {
        Component.onCompleted: {

            if(buttonAllVisble) {
                button.visible = false
                return;
            }
            if(buttonVisible) {
                if(index >= 0) {
                    button.visible = (index == (control.model.length - 1)) ? false : true
                }
            }
            else {
                if(index == 0) {
                    button.visible = false;
                }
                else if(index >0) {
                    button.visible = (index == (control.model.length - 1)) ? false : true
                }
            }
        }
        width: control.width
        contentItem: Rectangle
        {
            anchors.fill: parent
//            radius: 4
           // color:  "transparent"
            color: control.highlightedIndex === index ? "#61D37D" :"transparent"
            anchors.margins: 0
            Rectangle {
                id :line
                anchors.top: parent.top
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.margins: 0
//                radius: 4
                // border.color: "#303030"
                // border.width: 1
                 // color: "#F5F6F7"
                height: 1
                visible: false
            }
            RowLayout {
                id :combLayout
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.top: line.bottom
                anchors.bottom: parent.bottom
                anchors.leftMargin: 12
                anchors.rightMargin: 12
                Label {
                    id:myText
                    text: modelData
                    Layout.fillWidth: true
                    font.weight: Font.Bold
                    color:  "#FFFFFF"
                    // font: control.font
                    elide: Text.ElideRight
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignLeft
                }

                Button {
                    id:button
                    // display: AbstractButton.IconOnly
                    anchors.margins: 0
                    height: 20
                    width: 20
                    onClicked: {
                        buttonClicked(myText.text)
                    }
                    background: Image {
                        id: name
                        // fillMode:Image.PreserveAspectFit
                        anchors.fill: parent
                        anchors.margins: 0
                        source: "qrc:/images/images/fdm_setting_icon_n.png"
                    }
                }

            }
        }
        highlighted: control.highlightedIndex === index
    }

    indicator:Image {
        id: canvas
        x: control.width - width - 12
        y: control.topPadding + (control.availableHeight - height) / 2
        source: "qrc:/images/images/fdm_an_icon_n.png"
    }
    contentItem:  Rectangle
    {
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: canvas.left
        anchors.bottom: parent.bottom
        color:"transparent"
        anchors.leftMargin: 12
        Label {
            anchors.fill: parent
            text: control.displayText
            //font: control.font
            font.weight: Font.Bold
            color: "#FFFFFF"
            //color:  control.hovered  ? "#71D78A" : (control.pressed ? "#61D37D" : "#E7E7E9") /*"#FFFFFF" : "#333333"*/
            horizontalAlignment: Text.AlignJustify
            verticalAlignment: Text.AlignVCenter
            elide: Text.ElideRight
        }
    }

    background: Rectangle {
        id:backgroundRectangle
        implicitWidth: 226
        implicitHeight: 40
        border.color: control.hovered  ? "#71D78A" : (control.pressed ? "#61D37D" : "#3A3B3F")
        border.width:  control.hovered ? 1 : ( control.pressed ? 1 : 0 ) /*control.visualFocus ? 2 : 1*/
        radius: 7
        color: "#3A3B3F"
    }

    popup: Popup {
        y: control.height - 1
        width: control.width
        implicitHeight:/* listview.contentHeight*/ control.delegateModel.count < 6 ? 40 * control.delegateModel.count : 40 * 6
        padding:1

        contentItem: Rectangle{
            color: "#3A3B3F"
            border.width: 0
           // radius: 4
            ListView {
                id: listview
//                anchors.top: parent.top
//                anchors.left: parent.left
//                anchors.right: parent.right
                anchors.fill: parent
                anchors.topMargin: 4
                anchors.bottomMargin: 4
                snapMode: ListView.SnapToItem
                height:  control.delegateModel.count < 6 ? 40 * control.delegateModel.count : 40 * 6
                ScrollBar.vertical:  ScrollBar{
                    policy: ScrollBar.AsNeeded
                    anchors.right: listview.right
                    width: 6
                    active: true
                    contentItem: Rectangle {
                        radius: width/2
                        color: '#D6D6D6'
                    }
                }
                clip: true
                model: control.popup.visible ? control.delegateModel : null
                currentIndex: control.highlightedIndex
                ScrollIndicator.vertical: ScrollIndicator { }
            }
        }
    }


}
