pragma Singleton
import QtQuick 2.2

Item {
    readonly property int globalSpacing: 30
    readonly property string globalBackgroundPattern: "medium"

    // Colors
    property QtObject colors: QtObject {
        readonly property color light: "#292929"
        readonly property color medium: "#202020"
        readonly property color dark: "#151515"

        readonly property color text: "#ffffff"
        readonly property color label: "#999999"
    }

    // Fonts
    property QtObject fonts: QtObject {
        readonly property font standard: Qt.font({ family: "Roboto", pointSize: 23, weight: 40 })
        readonly property font disclaimer: Qt.font({ family: "Roboto", pointSize: 17, weight: 40 })
        readonly property font header: Qt.font({ family: "Roboto", pointSize: 32, weight: 50 })
    }

    // Box
    readonly property string boxBackgroundPattern: "dark"
    readonly property color boxBorderColor: colors.light
    readonly property int boxBorderWidth: 1

    // Menu items
    readonly property int menuDefaultWidth: 400
    readonly property int menuDefaultHeight: 60
    readonly property int menuHorizontalMargins: globalSpacing
    readonly property int menuVerticalMargins: globalSpacing / 2
}
