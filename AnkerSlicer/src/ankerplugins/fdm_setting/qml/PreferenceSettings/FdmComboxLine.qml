import QtQuick 2.0

import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import QtQml.Models 2.15
import "../BaseControl"  as BaseControl
Rectangle {
    id:root
    implicitHeight: 30
    implicitWidth: 460
    color: "transparent"

    property alias lineLabel: label.text
    property variant comboboxModel
    property variant comboxCurrentValue
    property bool comboxEnable: true
    property alias commboxListmodel: combox.model

    signal comboxCurrentTextChanged(string comboxCurrentText)
    Text {
        id:label
        anchors.left: parent.left
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        verticalAlignment: Text.AlignVCenter
        color: "#FFFFFF"
    }

    BaseControl.DefaultComboBox {
        id: combox
        width: 130
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        textRole: "value"
        enabled: comboxEnable
        model: ListModel {

        }
        onCurrentIndexChanged: {
           // console.log("current currentIndex ===",combox.currentIndex,"commboxListmodel ===" , commboxListmodel.get(combox.currentIndex).key, " comboxCurrentValue == ",comboxCurrentValue)
            comboxCurrentTextChanged(commboxListmodel.get(combox.currentIndex).key)
        }
    }
    Binding {
        target:combox
        property: "currentIndex"
        value: calCount(comboxCurrentValue,comboboxModel,0)
    }

    Binding {
        target:combox
        property: "model"
        value: readValues(comboboxModel)
    }

    function readValues(options) {
        commboxListmodel.clear()
        for(var option in options) {
            commboxListmodel.append({"key":option,"value":options[option]})
        }
        return commboxListmodel
    }

    function calCount(string,options,coutnt) {
        for(var option in options) {
          //  console.log("object item", option, "=" ,options[option],"string= ",string ,"cout =",coutnt)
            if(option === string) {
                return coutnt;
            }
            coutnt++;
        }
        return 0;
    }

}
