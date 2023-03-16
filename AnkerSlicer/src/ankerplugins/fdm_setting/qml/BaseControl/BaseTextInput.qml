import QtQuick 2.15
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.3

Rectangle {
    id: root
    implicitWidth: 130
    implicitHeight: 30
    color: "transparent"
    border.color: borderColor
    border.width: 1
    radius: 4

    property color borderColor: "#4F5259"
    property alias validator: input.validator
    property alias textInput: input.text
    property alias textLabel: unitLabel.text
    property alias textInputDisplayText: input.displayText
    property alias inputValidator: input.validator
    property bool inputFocus: false
    signal inputTextEdited()
    signal inputEditFinished()

    RowLayout {
        id:textLayout
        anchors.fill: parent
        TextInput {
            id: input
            text: model.value
            horizontalAlignment: Text.AlignLeft
            leftPadding: 8
            verticalAlignment: Text.AlignVCenter
            Layout.fillHeight: true
            Layout.fillWidth: true
            selectByMouse: true
            readOnly: false
            activeFocusOnPress :true
            color: root.enabled == true ? "#FFFFFF" : "#666666"
          //  validator: RegExpValidator{ regExp: (unitLabel.text == "float" ) ? /^-?[0-9]{0,5}[.,]?[0-9]{0,3}$/ : (unitLabel.text == "[int]" ) ? /^\[?(\s*-?[0-9]{0,9}\s*,)*(\s*-?[0-9]{0,9})\s*\]?$/ : /^-?[0-9]{0,9}$/  /* /^\[?(\s*-?[0-9]{0,9}\s*,)*(\s*-?[0-9]{0,9})\s*\]?$/*/  /*(model.type == "int") ? /^-?[0-9]{0,10}$/ : (model.type == "float") ? /^-?[0-9]{0,9}[.,]?[0-9]{0,3}$/*/ /*: /^.*$/*/ }
            // validator: RegExpValidator{ regExp: (model.type == "float" ) ? /^-?[0-9]{0,5}[.,]?[0-9]{0,3}$/ : (model.type == "[int]" ) ? /^\[?(\s*-?[0-9]{0,9}\s*,)*(\s*-?[0-9]{0,9})\s*\]?$/ : /^-?[0-9]{0,9}$/  /* /^\[?(\s*-?[0-9]{0,9}\s*,)*(\s*-?[0-9]{0,9})\s*\]?$/*/  /*(model.type == "int") ? /^-?[0-9]{0,10}$/ : (model.type == "float") ? /^-?[0-9]{0,9}[.,]?[0-9]{0,3}$/*/ /*: /^.*$/*/ }
            /*RegExpValidator{ regExp: /[+-]?\d+(\.\d*)?|[+-]?\.\d+/ }*/
            onTextEdited: {
                inputTextEdited()
            }
            onEditingFinished: {
                inputFocus = false
                inputTextEdited()
            }
            onAccepted: {
                console.log("onAccepted ....")
                inputTextEdited()
            }
        }
        Label {
            id: unitLabel
            color: "#999999"
           // text: "mm"
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            rightPadding: 6
            Layout.minimumWidth: 20
            Layout.fillWidth: false
            Layout.fillHeight: true
            Layout.alignment: Qt.AlignRight | Qt.AlignVCenter
        }

    }
    MouseArea{
        anchors.fill: parent
        propagateComposedEvents : true
        hoverEnabled: true
        cursorShape: Qt.IBeamCursor
        onEntered: {
            borderColor = "#61D37D"
//           console.log("MouseArea  enter=============== model.label == ",model.label);
        }
        onClicked: {
            mouse.accepted = true;
            inputFocus = true;
            input.forceActiveFocus();
            input.update();
           // console.log("click...................", model.label ,"foucus == ", input.focus, "activeFocusOnPress =" , input.activeFocusOnPress  ,"length ==" , input.length )
        }
        onExited: {
            borderColor = "#4F5259"
            inputEditFinished()
            //console.log("MouseArea  Exited===============");
        }
    }
}

/*##^##
Designer {
    D{i:0;formeditorZoom:3}
}
##^##*/
