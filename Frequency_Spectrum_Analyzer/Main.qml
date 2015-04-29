import QtQuick 2.3

Rectangle {
	id: main
	width: 1300
	height: 400
	color: "#00000000"
	enabled: false

	Row {
		id: bands
		objectName: "bands"
		width: parent.width
		height: parent.height / 2
		spacing: 4
	}

	Rectangle {
		id: divider
		objectName: "divider"
		color: "#40ffffff"
		width: parent.width
		height: 1
		anchors.top: bands.bottom
	}

	Row {
		id: bandsReflection
		objectName: "bandsReflection"
		width: parent.width
		height: parent.height / 2
		anchors.top: divider.bottom
		spacing: 4
	}
}
