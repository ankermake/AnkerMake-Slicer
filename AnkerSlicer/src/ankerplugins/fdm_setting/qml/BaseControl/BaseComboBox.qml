import QtQuick 2.0
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.3
import "../BaseControl" as BaseControl
ComboBox {
    id: control
    implicitHeight: 30

    signal buttonClicked(string name)
    property bool buttonVisible: true
    property bool buttonAllVisble: false
    property alias backgroundColor: backgroundRectangle.color
    property int listViewItemHeight: 40
    property int maxItemCount: 6
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
        height:listViewItemHeight
        contentItem: Rectangle
        {
            anchors.fill: parent
            color: control.highlightedIndex === index ? "#61D37D" :"transparent"
            anchors.margins: 0
            Rectangle {
                id :line
                anchors.top: parent.top
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.margins: 0
                height: 1
                visible: false
            }
            RowLayout {
                id :combLayout
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.top: line.bottom
                anchors.bottom: parent.bottom
                anchors.leftMargin: 0
                anchors.rightMargin: 12
                anchors.topMargin: 0
                anchors.bottomMargin: 0
                spacing: 0
                Label {
                    id:myText
                    text: modelData
                    Layout.fillWidth: true
                    anchors.margins: 0
                    font.weight: Font.Bold
                    color:  "#FFFFFF"
                    elide: Text.ElideRight
                    leftPadding: 12
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignLeft
                }

                Button {
                    id:button
                    height: 20
                    width: 20
                    onClicked: {
                        buttonClicked(myText.text)
                    }
                    background: Image {
                        id: name
                        anchors.fill: parent
                        anchors.margins: 0
                        source: "qrc:/images/images/fdm_setting_icon_n.png"
                    }
                }
            }
        }
    }

    indicator:Image {
        id: canvas
        x: control.width - width - 12
        y: control.topPadding + (control.availableHeight - height) / 2
        source: "qrc:/images/images/fdm_an_icon_n.png"
    }
    contentItem:  Rectangle
    {
        id: rectangle
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: canvas.left
        anchors.bottom: parent.bottom
        color:"transparent"
        Label {
            text: control.displayText
            font.weight: Font.Bold
            leftPadding: 12
            color: "#FFFFFF"
            horizontalAlignment: Text.AlignLeft
            verticalAlignment: Text.AlignVCenter
            elide: Text.ElideRight
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.top: parent.top
            anchors.bottom: parent.bottom
        }
    }

    background: Rectangle {
        id:backgroundRectangle
        implicitWidth: 226
        implicitHeight: listViewItemHeight
        border.color: control.hovered  ? "#71D78A" : (control.pressed ? "#61D37D" : "#3A3B3F")
        border.width:  control.hovered ? 1 : ( control.pressed ? 1 : 0 )
        radius: 4
        color: "#3A3B3F"
    }

    popup: Popup {
        id:popupList
        y: control.height + 4
        width: control.width
        property int extraHeight:  8;
        implicitHeight:control.delegateModel.count < maxItemCount ? (listViewItemHeight * control.delegateModel.count + extraHeight): (listViewItemHeight * maxItemCount + extraHeight)
        padding:0
        bottomInset: 0
        topInset: 0

        closePolicy:Popup.CloseOnEscape | Popup.CloseOnPressOutsideParent
        background: Rectangle {
           color: "transparent"
           anchors.margins: 0

        }
        contentItem: Rectangle{
            color: "#3A3B3F"
            border.width: 0
            anchors.margins: 0
            radius: 4
            BaseControl.BaseListView {
                id: listview
                anchors.fill: parent
                anchors.topMargin: 4
                anchors.bottomMargin: 4
                itemHeight:listViewItemHeight
                clip: true
                model: control.popup.visible ? control.delegateModel : null
                currentIndex: control.highlightedIndex
               // ScrollIndicator.vertical: ScrollIndicator { }
            }
        }
    }


}
