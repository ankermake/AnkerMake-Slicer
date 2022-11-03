import QtQuick 2.0
import QtQuick.Controls 2.12

import "../BaseControl" as BaseControl
Item {
    implicitWidth: 200
    implicitHeight: 20
    id: root
    BaseControl.SwitchButton {
        id: switchbutton
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        anchors.leftMargin: 0
        anchors.verticalCenter: parent.verticalCenter
        isChecked :Boolean(model.value)
        onCheckedStateChanged: {
            paramModel.changeData(model.index,!isChecked);
        }
    }
}
