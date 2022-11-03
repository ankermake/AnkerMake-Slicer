import QtQuick 2.15
import QtQuick.Controls 2.12
Rectangle {
    id: root
    implicitHeight:30
    implicitWidth: 465
    color: "transparent"
    signal showAllChildern(string category)

    Image {
        id: iconImg
        anchors.left: parent.left
        anchors.verticalCenter: parent.verticalCenter
        source: model.icon
    }

    Label {
        id: labelName
        anchors.left: iconImg.right
        anchors.verticalCenter: parent.verticalCenter
        anchors.leftMargin: 16
        text: model.label
        color: "#FFFFFF"
        font.weight: Font.Bold
    }

    Image {
        id:expandIcon
        anchors.right: parent.right
//        anchors.rightMargin:16
        anchors.verticalCenter: parent.verticalCenter
        source: model.expand ?  "qrc:/images/images/fdm_pack_icon_n.png" : "qrc:/images/images/fdm_an_icon_n.png"
    }

    MouseArea {
        anchors.fill: parent
        hoverEnabled: true
        onClicked:  {
            if(model.expand) {
                preferenceModel.collapseIndex(model.index);
            } else {
               preferenceModel.expandIndex(model.index);
            }
            expandIcon.source =( model.expand ?  "qrc:/images/images/fdm_pack_icon_n.png" : "qrc:/images/images/fdm_an_icon_n.png")
//            console.log("expand = " , model.expand , " source : " , expandIcon.source)
        }
    }
}
