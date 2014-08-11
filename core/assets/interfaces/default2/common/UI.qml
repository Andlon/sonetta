pragma Singleton
import QtQuick 2.2

Item {
    readonly property int globalSpacing: 30 * scale
    readonly property string globalBackgroundPattern: "medium"

    readonly property real scale: calculateScaleFactor(screenWidth, screenHeight)

    property QtObject timing: QtObject {
        readonly property int highlightMove: 200
        readonly property int dialogPopup: 250
        readonly property int fade: 500
    }

    property QtObject dialog: QtObject {
        readonly property real brightnessDelta: -0.80
        readonly property int blurRadius: 32
    }

    property QtObject colors: QtObject {
        readonly property color light: "#292929"
        readonly property color medium: "#202020"
        readonly property color dark: "#151515"

        readonly property color text: "#ffffff"
        readonly property color label: "#999999"
        readonly property color darkLabel: "#555555"
        readonly property color focus: "#ffc000"
        readonly property color focusText: light
        readonly property color focusLabel: Qt.tint(label, Qt.rgba(focusText.r, focusText.g, focusText.b, 0.75))
        readonly property color darkFocusLabel: Qt.darker(darkLabel, 0.75)
        readonly property color currentUnfocused: "#555555"
    }

    property QtObject fonts: QtObject {
        readonly property font standard: Qt.font({ family: "Roboto", pointSize: 23 * scale, weight: 35 })
        readonly property font major: Qt.font({ family: "Roboto", pointSize: 25 * scale, weight: 35 })
        readonly property font minor: Qt.font({ family: "Roboto", pointSize: 21 * scale, weight: 35 })
        readonly property font disclaimer: Qt.font({ family: "Roboto", pointSize: 17 * scale, weight: 40 })
        readonly property font header: Qt.font({ family: "Roboto", pointSize: 26 * scale, weight: 9, capitalization: Font.AllUppercase })

        readonly property font input: Qt.font({ family: "Roboto", pointSize: 32 * scale, weight: 40 })
        readonly property font mainMenu: header

        readonly property font nowPlayingMajor: Qt.font({ family: "Roboto", pointSize: 32 * scale, weight: 35 })
        readonly property font nowPlayingMinor: Qt.font({ family: "Roboto", pointSize: 22 * scale, weight: 35 })

    }

    property QtObject box: QtObject {
        readonly property string backgroundPattern: "dark"
        readonly property color borderColor: colors.light
        readonly property int borderWidth: 1
    }

    property QtObject menu: QtObject {
        readonly property int defaultWidth: 450 * scale
        readonly property int defaultHeight: 60 * scale
        readonly property int defaultLabelWidth: 160 * scale
        readonly property int horizontalMargins: globalSpacing
        readonly property int verticalMargins: globalSpacing / 2
        readonly property int verticalSpacing: globalSpacing / 4
        readonly property int checkboxMargin: 8 * scale
        readonly property int frameSpacing: 3 * scale
        readonly property int indentation: 2 * globalSpacing
        readonly property font labelFont: Qt.font({ family: "Roboto", pointSize: 21 * scale, weight: 9, capitalization: Font.AllUppercase })
    }

    property QtObject playlistPage: QtObject {
        readonly property int mosaicSize: 96 * scale
        readonly property int fadeTime: 300
    }

    property QtObject playlist: QtObject {
        readonly property font defaultFont: fonts.standard
        readonly property font singleRowFont: defaultFont
        readonly property font doubleRowTrackFont: fonts.major
        readonly property font doubleRowArtistFont: fonts.minor
        readonly property font doubleRowNumberFont: fonts.major
    }

    property QtObject views: QtObject {
        readonly property size imageSize: Qt.size(96 * scale, 96 * scale)
    }

    property QtObject browse: QtObject {
        readonly property font albumHeaderMajor: Qt.font({ family: "Roboto", pointSize: 32 * scale, weight: 29 })
        readonly property font albumHeaderMinor: Qt.font({ family: "Roboto", pointSize: 27 * scale, weight: 29 })
    }

    function calculateScaleFactor(width, height) {
        if (height <= 0)
            return 1.0;
        return height / 1080;
    }

    // Live bindings below
    property int pageTopMargin: 0
    property int screenHeight: 1080
    property int screenWidth: 1920
}
