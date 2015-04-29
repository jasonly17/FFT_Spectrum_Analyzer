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
			width: main.width
			height: main.height
			anchors.bottom: if (anchorBottom) parent.bottom
			gradient: (anchorBottom) ? bottomGradient : topGradient

			Behavior on height { PropertyAnimation { duration: 60} }
		}

		Gradient {
			id: bottomGradient
			GradientStop {
				position: 0.493
				color: "#207ddc"
			}

			GradientStop {
				position: 0
				color: "#9443f0"
			}

			GradientStop {
				position: 1
				color: "#66c4ff"
			}
		}

		Gradient {
			id: topGradient
			GradientStop {
				position: 0.507
				color: "#207ddc"
			}

			GradientStop {
				position: 1
				color: "#9443f0"
			}

			GradientStop {
				position: 0
				color: "#66c4ff"
			}
		}
	}
}

