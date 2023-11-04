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
        textInput:  inputFocus ? input.textInput : (model.type === "float" )  ? Math.round(model.value * 1000 ) / 1000 : model.value
        textLabel: model.unit
        validator: RegExpValidator{ regExp: (model.type === "float" ) ? /^-?[0-9]{0,5}[.,]?[0-9]{0,3}$/ : (model.type === "[int]" ) ? /^\[?(\s*-?[0-9]{0,9}\s*,)*(\s*-?[0-9]{0,9})\s*\]?$/ : /^-?[0-9]{0,9}$/  /* /^\[?(\s*-?[0-9]{0,9}\s*,)*(\s*-?[0-9]{0,9})\s*\]?$/*/  /*(model.type === "int") ? /^-?[0-9]{0,10}$/ : (model.type === "float") ? /^-?[0-9]{0,9}[.,]?[0-9]{0,3}$/*/ /*: /^.*$/*/ }
        borderColor : isError() ? "red" : (isWarning() ? "yellow" : "#4F5259")
        onInputTextEdited: {
            paramModel.changeData(model.index,textInput);
        }
    }

    function isError() {
      //console.log(  " label =", model.label, "value = ",model.value ,"minimumValue = " , model.minimumValue, "maximumValue = ",model.maximumValue , "model.minimumValueWarning = " ,model.minimumValueWarning  , "model.maximumValueWarning = ",model.maximumValueWarning )
      return  (model.minimumValue < model.maximumValue) && (model.value < model.minimumValue || model.maximumValue < model.value)
    }

    function isWarning() {
        return  (model.minimumValueWarning < model.maximumValueWarning) && ( model.value < model.minimumValueWarning || model.maximumValueWarning < model.value)
    }
}
