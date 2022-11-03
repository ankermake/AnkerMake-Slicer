import QtQuick 2.0
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.15

Item {
    id: root
    implicitWidth: parent.width
    implicitHeight: 42
    visible: true
    anchors.leftMargin: 2
    anchors.rightMargin: 15

    signal showAllChildern(string category)
    signal showTooltip(string description, point p)
    signal hideTooltip()
    //when item is children node，use tab
    Item {
        id: itemSpace
        anchors.left: parent.left
        width: (model.level -1) * 20
        height: parent.height
    }

    Label {
        id :label
        anchors.left: itemSpace.right
        anchors.verticalCenter: parent.verticalCenter
        color: "#999999"
        text: model.label
    }

    Loader {
        id :typeLoder
        anchors.right: parent.right
        anchors.topMargin: 12
        anchors.bottomMargin: 12
        anchors.rightMargin: 16
        anchors.verticalCenter: parent.verticalCenter
        source: {
            switch (model.type) {
            case "bool":
                return "ParamCheckBox.qml"
            case "enum":
                return "ParamComboBox.qml"
            default:
                return "ParamTextField.qml"
            }
        }
    }

    MouseArea {
        anchors.fill: parent
        propagateComposedEvents : true
        hoverEnabled:true
        onEntered: {
        //   console.log("enterrrrrrrrrrrrrrrrrr", "mouse_X = ", mouseX, "mouse_y = " ,mouseY, "gloabal = " , mapToGlobal(0,0));
            //showTooltip()
           FdmParamSettingsWidget.showToolTip(model.label, model.description, model.affects, model.affectedBy, mapToGlobal(0,0));
//            typeLoder.item.int
        }
        //onPressed: {mouse.accepted = false}

        onClicked: {
            mouse.accepted = false
        }
        onExited:  {
            // mouse.accepted=false
          //  console.log("exited................");
           // hideTooltip()
            FdmParamSettingsWidget.hideToolTip();
            //mouse.accepted=false
        }

    }
}
