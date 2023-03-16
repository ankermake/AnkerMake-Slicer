import QtQuick 2.0
import QtQuick.Controls 2.15

Rectangle {
    id:root
    property alias delegate: listview.delegate
    property alias model: listview.model
    property alias currentIndex: listview.currentIndex
    property int contentHeight: listview.contentHeight
    property int itemHeight: 32
    anchors.margins: 0
    color: "transparent"
    ListView {
        id:listview
        anchors.fill: parent
        anchors.margins: 0
        clip: true
        spacing: 0
        highlight: Rectangle {
            color:  "#404249"
        }
        ScrollBar.vertical:  ScrollBar{
            policy: ScrollBar.AsNeeded
            anchors.right: listview.right
            stepSize: itemHeight
            width: 8
            height: 100
            active: false
            contentItem: Rectangle {
                radius: width/2
                focus: true
                color: '#797B80'
            }
        }
    }
  //  }
}

/*##^##
Designer {
    D{i:0;autoSize:true;height:322;width:210}
}
##^##*/
