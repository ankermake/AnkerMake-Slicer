import QtQuick 2.0
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.3

import "../BaseControl" as BaseControl

ComboBox {
    id: combox
    implicitHeight: 30
    property int listViewItemHeight: 40
    property int maxItemCount: 5
    delegate: ItemDelegate {
        height: listViewItemHeight
        width: combox.width
        contentItem: Text {
            text: combox.textRole ? ((Array.isArray(combox.model) ? modelData[combox.textRole] : model[combox.textRole])): modelData
            color: "#FFFFFF"
            elide: Text.ElideRight
            renderType: Text.NativeRendering
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignLeft
        }
        hoverEnabled: combox.hoverEnabled
        background: Rectangle{
            color: (combox.highlightedIndex === index) ? "#61D37D" :"transparent"
        }
    }

    background: Rectangle {
        border.color: combox.hovered  ? "#71D78A" : (combox.pressed ? "#71D78A" : "#51545F")
        border.width: 1
        color: "transparent"
        radius: 4
    }
    contentItem: Rectangle {
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: name.left
        anchors.bottom: parent.bottom
        color: "transparent"
        anchors.leftMargin: 8
        anchors.rightMargin: 8
        Text {
            anchors.fill: parent
            text: combox.displayText
            horizontalAlignment: Text.AlignJustify
            verticalAlignment: Text.AlignVCenter
            color: combox.enabled === true ? "#FFFFFF" : "#666666"
            elide: Text.ElideRight
        }
    }
    indicator:Image {
        id: name
        x: combox.width - width - 8
        y: combox.topPadding + (combox.availableHeight - height) / 2
        source: "qrc:/images/images/fdm_an_icon_n.png"
    }

    popup: Popup {
        id:pop
        y: combox.height + 4
        width: combox.width
        property int extraHeight: 8;
        implicitHeight:combox.delegateModel.count < maxItemCount ? (listViewItemHeight * combox.delegateModel.count + extraHeight ) : (listViewItemHeight * maxItemCount + extraHeight )
        padding: 0
        bottomInset: 0
        topInset: 0
        background: Rectangle {
          color: "transparent"
        }
        contentItem: Rectangle{
           color: "#3A3B3F"
            radius: 4
             BaseControl.BaseListView {
                id: listview
                anchors.fill: parent
                anchors.topMargin: 4
                anchors.bottomMargin: 4
                anchors.leftMargin: 0
                anchors.rightMargin: 0
                itemHeight: listViewItemHeight
                clip: true
                model: combox.popup.visible ? combox.delegateModel : null
                currentIndex: combox.highlightedIndex
            }
        }
    }
}
