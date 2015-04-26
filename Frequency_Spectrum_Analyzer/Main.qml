import QtQuick 2.3

Rectangle {
    id: main
    width: 200
    height: 200

    Row {
        id: row1
        anchors.fill: parent
        spacing: 4
        Repeater {
            id: spectrum
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 200
            model: 56
            Rectangle {
                id: delagate
                width: 10
                height: 200
                color: "#1bbfff"
                anchors.bottom: parent.bottom
                anchors.bottomMargin: 0
            }
        }

        Connections {
            target: engine
            onUpdate: {
                spectrum.itemAt(0).height = val1
                spectrum.itemAt(1).height = val2
                spectrum.itemAt(2).height = val3
            }
        }
    }
}
