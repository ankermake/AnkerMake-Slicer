import QtQuick 2.0
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import QtQml.Models 2.15

import "../BaseControl" as BaseControl

Rectangle {
    id: root
    implicitWidth: 130
    implicitHeight: 30
    color: "transparent"
    property int row: -1

    BaseControl.DefaultComboBox{
        id: select
        anchors.fill :parent
        anchors.verticalCenter: parent.verticalCenter
        enabled: (parameter.defaultParameterList).indexOf(parameter.currentParameterName) <0 ? true : false
        textRole: "value"
        model:  ListModel {
            id:listModel
            Component.onCompleted: {
                readValues(model.options);
            }
        }

        onActivated: {
            //console.log("row = ", row,"current text =", currentText,"text =" ,listModel.get(currentIndex).key);
            preferenceModel.changeData(row,listModel.get(currentIndex).key);
        }
    }

    Component.onCompleted: {
        row = model.index;
    }
    Binding {
        target:select
        property: "currentIndex"
        value: calCount(model.value,model.options,0)
    }
    function readValues(options) {
        for(var option in options) {
            listModel.append({"key":option,"value":options[option]})
        }
    }

    function calCount(string,options,coutnt) {
        for(var option in options) {
            //console.log("object item", option, "=" ,options[option],"string= ",string ,"cout =",coutnt)
            if(option === string) {
                return coutnt;
            }
            coutnt++;
        }
        return 0;
    }
}
