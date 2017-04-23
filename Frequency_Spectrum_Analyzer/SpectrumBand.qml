import QtQuick 2.3

Item {
    id: main
    property alias bandHeight: band.height
    property bool anchorBottom: true
    width: 10
    height: 200
    enabled: false

    Rectangle {
        color: "#00000000"
        width: main.width
        height: main.height

        Rectangle {
            id: band
            objectName: "band"
            width: parent.width
            height: parent.height
            anchors.top: if (!anchorBottom) parent.top
            anchors.bottom: if (anchorBottom) parent.bottom
            //gradient: (anchorBottom) ? bottomGradient : topGradient
            color: "#DDFFFFFF"

            Behavior on height { PropertyAnimation { duration: 80; easing.type: Easing.InOutQuad} }
        }

        Gradient {
            id: bottomGradient
            GradientStop {
                position: 0.626
                color: "#15bcfc"
            }

            GradientStop {
                position: 0
                color: "#f620a3"
            }

            GradientStop {
                position: 1
                color: "#24bf7b"
            }
        }

        Gradient {
            id: topGradient
            GradientStop {
                position: 0.374
                color: "#15bcfc"
            }

            GradientStop {
                position: 1
                color: "#f620a3"
            }

            GradientStop {
                position: 0
                color: "#24bf7b"
            }
        }
    }
}

