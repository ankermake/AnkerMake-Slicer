import QtQuick 2.0

Item {
    id:root

    default property alias content: stack.children
    property int current: 0
    signal currentTabChanged(int dex)

    onCurrentChanged: setOpacities()
    Component.onCompleted: {
        setOpacities()
    }
    function setOpacities() {
        for(var i = 0 ; i <stack.children.length; ++i) {
            stack.children[i].opacity = (i == current ? 1 :0)
            stack.children[i].visible = (i == current ? true : false)
        }
    }
    Row {
        id:header

        Repeater {
            model: stack.children.length
            delegate: Rectangle {
                width: 100
                height: 26
                color: "transparent"

//                Component.onCompleted:  {
//                    console.log("index =", index ,"text ==" ,stack.children[index].title )
//                }
                Text {
                    id: title
                    anchors.top: parent.top
                    anchors.left: parent.left
                    anchors.right: parent.right
                    horizontalAlignment: Qt.AlignLeft;
                    verticalAlignment: Qt.AlignVCenter
                    text: stack.children[index].title
                    elide: Text.ElideRight
                    font.weight: current == index ? Font.Bold :  Font.Normal
                    color: current == index ? "#FFFFFF" : "#999999"
                }
                Rectangle {
                    id:lines
                    height: 2
                    anchors.top: title.bottom
                    anchors.rightMargin:60
                    anchors.leftMargin: 10
                    anchors.left: parent.left
                    anchors.right: parent.right
                    border.color: "#18b930"
                    border.width: 3
                    visible:  current == index
                }

                MouseArea {
                    anchors.fill: parent
                    hoverEnabled: true
                    focus: true
                    propagateComposedEvents: true
                    onClicked: {
                        mouse.accepted = false
                        root.current = index
                        currentTabChanged(index)
                    }
                }
            }
        }
    }

    Rectangle {
        id: stack
        width: root.width
        clip: true
        anchors {top: header.bottom ; bottom: root.bottom ; topMargin: 12}
    }
}

/*##^##
Designer {
    D{i:0;autoSize:true;height:480;width:640}
}
##^##*/
