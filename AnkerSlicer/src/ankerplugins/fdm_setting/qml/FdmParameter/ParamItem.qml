import QtQuick 2.0
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.15

Item {
    id: root
    implicitWidth: parent.width
    implicitHeight: 42
    visible: model.visible
    //visible: true
    anchors.leftMargin: 2
    anchors.rightMargin: 15

    signal showAllChildern(string category)
    signal showTooltip(string description, point p)
    signal hideTooltip()
    //当item为子节点时， 缩进 占位
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
        //color: "#999999"
        Text {
            id: name
            text: model.label
            elide:Text.ElideRight
            color: "#999999"
            width: 300
           // wrapMode: Text.Wrap.Overflow
        }
//        text: model.label
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
           FdmParamSettingsWidget.showToolTip(model.label, model.description, model.affects, model.affectedBy, mapToGlobal(0,0));
        }

        onClicked: {
            mouse.accepted = false
        }
        onExited:  {
          //  console.log("exited................");
            FdmParamSettingsWidget.hideToolTip();
        }

    }
}
