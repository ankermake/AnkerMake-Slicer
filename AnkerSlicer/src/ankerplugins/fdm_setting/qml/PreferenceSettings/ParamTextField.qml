import QtQuick 2.15
import QtQuick.Controls 2.5

import "../BaseControl" as BaseControl
Rectangle {
    id: root
    implicitHeight: 30
    implicitWidth: 130
    color: "transparent"
    BaseControl.BaseTextInput {
        id: input
        anchors.fill: parent
        textInput: (model.type == "float" )  ? Math.floor(model.value * 1000) / 1000 : model.value
        textLabel: model.unit
        enabled: (parameter.defaultParameterList).indexOf(parameter.currentParameterName) <0 ? true : false
        validator: RegExpValidator{ regExp: (model.type == "float" ) ? /^-?[0-9]{0,5}[.,]?[0-9]{0,3}$/ : (model.type == "[int]" ) ? /^\[?(\s*-?[0-9]{0,9}\s*,)*(\s*-?[0-9]{0,9})\s*\]?$/ : /^-?[0-9]{0,9}$/  /* /^\[?(\s*-?[0-9]{0,9}\s*,)*(\s*-?[0-9]{0,9})\s*\]?$/*/  /*(model.type == "int") ? /^-?[0-9]{0,10}$/ : (model.type == "float") ? /^-?[0-9]{0,9}[.,]?[0-9]{0,3}$/*/ /*: /^.*$/*/ }

        onInputTextEdited: {
             preferenceModel.changeData(model.index,textInput);
        }
    }
}
