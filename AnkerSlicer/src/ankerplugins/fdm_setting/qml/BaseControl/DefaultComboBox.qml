import QtQuick 2.0
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.3
ComboBox {
    id: combox
    implicitHeight: 30
//    delegate: ItemDelegate {
//        width: combox.width
//        contentItem: Rectangle {
//            Component.onCompleted:  {
//                console.log("listview = " ,listview.model/*.model.get(index).key*/,"type ==",typeof(listview.model))
//            }
//            anchors.fill: parent
//            radius: 4
//            color:  "transparent"
//           // color: control.highlightedIndex === index ? "#61D37D" :"transparent"
//            anchors.margins: 12

//            RowLayout {
//                id :combLayout
//                anchors.left: parent.left
//                anchors.right: parent.right
//               // anchors.top: .bottom
//                Label {
//                    id:myText
//                   // text: listview.model.model.get(index).value
//                    Layout.fillWidth: true
//                    font.weight: Font.Bold
//                    color: combox.highlightedIndex === index ? "#333333": "#FFFFFF"
//                    // font: control.font
//                    elide: Text.ElideRight
//                    verticalAlignment: Text.AlignVCenter
//                }


//            }
//        }
//        highlighted: combox.highlightedIndex === index
//    }
    delegate: ItemDelegate {
           implicitWidth: combox.implicitWidth
//           implicitHeight: combox.implicitHeight
           width: combox.width
           contentItem: Text {
               text: combox.textRole
                     ? (Array.isArray(combox.model)
                        ? modelData[combox.textRole]
                        : model[combox.textRole])
                     : modelData
               color: "#FFFFFF"
               elide: Text.ElideRight
               renderType: Text.NativeRendering
               verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignLeft
           }
           hoverEnabled: combox.hoverEnabled
           background: Rectangle{
               //radius: control.radius
               color: (combox.highlightedIndex === index) ? "#61D37D" :"transparent"
//               Rectangle{
//                   height: 1
//                   width: parent.width
//                   anchors.bottom: parent.bottom
//                   color: "red"
//               }
           }
       }

    background: Rectangle {
        // visible: control.enabled && control.editable && !control.flat
      //  border.width:  1
        border.color: combox.hovered  ? "#71D78A" : (combox.pressed ? "#71D78A" : "#51545F")
        border.width: 1 /*combox.hovered ? 1 : ( combox.pressed ? 1 : 0 )*/
        color: "transparent"
        radius: 4
    }
    contentItem: Rectangle {
        //anchors.fill: parent
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
            //font: combox.font
            horizontalAlignment: Text.AlignJustify
            verticalAlignment: Text.AlignVCenter
            color: combox.enabled == true ? "#FFFFFF" : "#666666"
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
        y: combox.height - 1
        width: combox.width
        implicitHeight:/* listview.contentHeight*/ combox.delegateModel.count < 6 ? 40 * combox.delegateModel.count : 40 * 6
        padding: 1

        contentItem: Rectangle{
             color: "#3A3B3F"
           // color: "#3A3B3F"
            ListView {
                id: listview
                anchors.top: parent.top
                anchors.left: parent.left
                anchors.right: parent.right
                spacing: 0
                height:  combox.delegateModel.count < 6 ? 40 * combox.delegateModel.count : 40 * 6
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
                model: combox.popup.visible ? combox.delegateModel : null
                currentIndex: combox.highlightedIndex
                //ScrollIndicator.vertical: ScrollIndicator { }
//                delegate: Rectangle
//                {
//                    anchors.fill: parent
//                    radius: 4
//                    color:  "transparent"
//                    // color: control.highlightedIndex === index ? "#61D37D" :"transparent"
//                    anchors.margins: 12

//                    Label {
//                        text: modelData
//                        anchors.fill: parent
//                        color: /*combox.highlightedIndex == index ? "#999999": */"#FFFFFF"
//                        // font: control.font
//                        //elide: Text.ElideRight
//                        //verticalAlignment: Text.AlignVCenter
//                    }
//                }
            }
        }
    }
}
