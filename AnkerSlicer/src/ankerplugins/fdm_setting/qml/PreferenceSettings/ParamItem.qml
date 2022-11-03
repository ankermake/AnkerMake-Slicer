import QtQuick 2.0
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.15

Item {
    id: root
    implicitHeight:30
    implicitWidth: 465
    visible: true
    anchors.leftMargin: 2

     signal showAllChildern(string category)

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
        text: model.label
        color: "#FFFFFF"
    }

    Loader {
        id :typeLoder
        anchors.right: parent.right
//        anchors.rightMargin: 16
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

}
