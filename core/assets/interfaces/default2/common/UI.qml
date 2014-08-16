pragma Singleton
import QtQuick 2.2

Item {
    FontLoader { id: robotoThin; source: "../fonts/Roboto-Thin-Modified.ttf" }
    FontLoader { id: robotoLight; source: "../fonts/Roboto-Light-Modified.ttf" }
    FontLoader { id: robotoRegular; source: "../fonts/Roboto-Regular-Modified.ttf" }

    readonly property string thinFontFamily: robotoThin.name
    readonly property string lightFontFamily: robotoLight.name
    readonly property string regularFontFamily: robotoRegular.name

    readonly property int globalSpacing: 30 * scale
    readonly property string globalBackgroundPattern: "medium"
    readonly property real defaultListFraction: 0.6

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
        readonly property font standard: createFont(lightFontFamily, scale * 32)
        readonly property font major: createFont(lightFontFamily, scale * 34)
        readonly property font minor: createFont(lightFontFamily, scale * 28)
        readonly property font disclaimer: createFont(regularFontFamily, scale * 23)
        readonly property font header: createFont(thinFontFamily, scale * 40, Font.AllUppercase)

        readonly property font input: createFont(regularFontFamily, scale * 42)

        readonly property font nowPlayingMajor: createFont(regularFontFamily, scale * 42)
        readonly property font nowPlayingMinor: createFont(regularFontFamily, scale * 32)
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
        readonly property font labelFont: createFont(thinFontFamily, 27, Font.AllUppercase)
    }

    property QtObject mainMenu: QtObject {
        readonly property font font: fonts.header
        readonly property int itemHeight: scale * 180
        readonly property int width: scale * 280
    }

    property QtObject playback: QtObject {
        readonly property size controlSize: Qt.size(Math.round(96 * scale), Math.round(96 * scale))
    }

    property QtObject playlistPage: QtObject {
        readonly property int mosaicSize: 96 * scale
        readonly property int fadeTime: 300
        readonly property real listFraction: defaultListFraction
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
        readonly property font albumHeaderMajor: createFont(lightFontFamily, scale * 42)
        readonly property font albumHeaderMinor: createFont(lightFontFamily, scale * 39)
        readonly property real listFraction: defaultListFraction
    }

    function calculateScaleFactor(width, height) {
        return height <= 0 ? 1.0 : height / 1080.0;
    }

    function createFont(family, pixelSize, capitalization) {
        var fontData = { family: family, pixelSize: Math.round(pixelSize)}
        if (arguments.length >= 3) {
            fontData.capitalization = capitalization
        }

        return Qt.font(fontData)
    }

    // Live bindings below
    property int pageTopMargin: 0
    property int screenHeight: 1080
    property int screenWidth: 1920
}
