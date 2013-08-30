import QtQuick 2.0
import QtQuick.Layouts 1.0
import QtQml.Models 2.1
import QtGraphicalEffects 1.0

import Sonetta 0.1

import "VirtualKeyboard.js" as VK

FocusScope {
    id: root
    width: grid.width
    height: grid.height

    signal character (string c)
    signal prev
    signal next
    signal enter
    signal backspace

    property bool wrapNavigationTop: true
    property bool wrapNavigationBottom: true
    property bool wrapNavigationLeft: true
    property bool wrapNavigationRight: true

    property url backspaceImage: "../images/keys/backspace.png"
    property url returnImage: "../images/keys/return.png"
    property url shiftUpImage: "../images/keys/shiftup.png"
    property url shiftDownImage: "../images/keys/shiftdown.png"
    property url symbolsImage: "../images/keys/symbols.png"
    property url lettersImage: "../images/keys/letters.png"
    property url spaceImage: "../images/keys/space.png"
    property url prevImage: "../images/keys/prev.png" // Cursor
    property url nextImage: "../images/keys/next.png" // Cursor

    property color highlightColor: "Green"
    property color color: "White"

    property size cellSize: Qt.size(56, 56)

    property int currentRow: 2
    property int currentColumn: 10

    /*
      Possible keystates:
      "upper" - the keyboard is currently displaying upper-case keys
      "lower" - the keyboard is currently displaying lower-case keys
      "symbol" - the keyboard is currently displaying symbols
      */
    property bool isLowercase: false
    property string keystate: "upper"

    property Component backgroundComponent: Item { }
    property Component textComponent: defaultTextComponent
    property Component iconComponent: defaultIconComponent

    GridLayout {
        id: grid
        width: VK.getSize().width
        height: VK.getSize().height
        rowSpacing: 0
        columnSpacing: 0
        Repeater {
            id: gridRepeater
            model: keys
            delegate: key
        }
    }

    ListModel {
        id: keys

        // First row
        ListElement { row: 0; col: 0; upper: "Q"; lower: "q"; symbol: "!"; }
        ListElement { row: 0; col: 1; upper: "W"; lower: "w"; symbol: "\""; }
        ListElement { row: 0; col: 2; upper: "E"; lower: "e"; symbol: "#"; }
        ListElement { row: 0; col: 3; upper: "R"; lower: "r"; symbol: "$"; }
        ListElement { row: 0; col: 4; upper: "T"; lower: "t"; symbol: "&"; }
        ListElement { row: 0; col: 5; upper: "Y"; lower: "y"; symbol: "/"; }
        ListElement { row: 0; col: 6; upper: "U"; lower: "u"; symbol: "("; }
        ListElement { row: 0; col: 7; upper: "I"; lower: "i"; symbol: ")"; }
        ListElement { row: 0; col: 8; upper: "O"; lower: "o"; symbol: "="; }
        ListElement { row: 0; col: 9; upper: "P"; lower: "p"; symbol: "?"; }
        ListElement { row: 0; col: 10; upper: "'"; lower: "'"; symbol: "?"; }

        // Second row
        ListElement { row: 1; col: 0; upper: "A"; lower: "a"; symbol: "!"; }
        ListElement { row: 1; col: 1; upper: "S"; lower: "s"; symbol: "!"; }
        ListElement { row: 1; col: 2; upper: "D"; lower: "d"; symbol: "!"; }
        ListElement { row: 1; col: 3; upper: "F"; lower: "f"; symbol: "!"; }
        ListElement { row: 1; col: 4; upper: "G"; lower: "g"; symbol: "!"; }
        ListElement { row: 1; col: 5; upper: "H"; lower: "h"; symbol: "!"; }
        ListElement { row: 1; col: 6; upper: "J"; lower: "j"; symbol: "!"; }
        ListElement { row: 1; col: 7; upper: "K"; lower: "k"; symbol: "!"; }
        ListElement { row: 1; col: 8; upper: "L"; lower: "l"; symbol: "!"; }
        ListElement { row: 1; col: 9; action: "backspace"; rowspan: 1; colspan: 2 }

        // Third row
        ListElement { row: 2; col: 0; action: "shift" }
        ListElement { row: 2; col: 1; upper: "Z"; lower: "z"; symbol: "!"; }
        ListElement { row: 2; col: 2; upper: "X"; lower: "x"; symbol: "!"; }
        ListElement { row: 2; col: 3; upper: "C"; lower: "c"; symbol: "!"; }
        ListElement { row: 2; col: 4; upper: "V"; lower: "v"; symbol: "!"; }
        ListElement { row: 2; col: 5; upper: "B"; lower: "b"; symbol: "!"; }
        ListElement { row: 2; col: 6; upper: "N"; lower: "n"; symbol: "!"; }
        ListElement { row: 2; col: 7; upper: "M"; lower: "m"; symbol: "!"; }
        ListElement { row: 2; col: 8; upper: ","; lower: ","; symbol: "!"; }
        ListElement { row: 2; col: 9; upper: ","; lower: ","; symbol: "!"; }
        ListElement { row: 2; col: 10; action: "return"; rowspan: 2; colspan: 1 }

        // Fourth row
        ListElement { row: 3; col: 0; action: "togglesymbols" }
        ListElement { row: 3; col: 1; upper: "@"; lower: "@"; symbol: "@"; }
        ListElement { row: 3; col: 2; action: "space"; rowspan: 1; colspan: 6 }
        ListElement { row: 3; col: 8; action: "prev" }
        ListElement { row: 3; col: 9; action: "next" }
    }

    Component {
        id: key

        FocusScope {
            id: keyRoot
            property int rowspan: model.rowspan ? model.rowspan : 1
            property int colspan: model.colspan ? model.colspan : 1
            property bool isIcon: model.action !== undefined // If key has an action associated, it has an icon
            property string iconPath: isIcon ? VK.getIconPath(model.action) : ""
            property string text: !isIcon ? VK.getKeyText(model.upper, model.lower, model.symbol) : ""

            Binding {
                target: foregroundLoader.item
                property: "text"
                value: keyRoot.text
                when: !isIcon
            }

            Binding {
                target: foregroundLoader.item
                property: "source"
                value: keyRoot.iconPath
                when: isIcon
            }

            property int cellWidth: root.cellSize.width * colspan;
            property int cellHeight: root.cellSize.height * rowspan;

            Layout.row: model.row
            Layout.column: model.col
            Layout.rowSpan: rowspan
            Layout.columnSpan: colspan
            Layout.maximumHeight: cellHeight
            Layout.maximumWidth: cellWidth
            Layout.minimumHeight: cellHeight
            Layout.minimumWidth: cellWidth

            Loader {
                id: backgroundLoader
                anchors.fill: parent
            }

            Loader {
                id: foregroundLoader
                anchors.fill: parent
                focus: true
                visible: !keyRoot.activeFocus

                Component.onCompleted: {
                    if (parent.isIcon)
                        sourceComponent = root.iconComponent
                    else
                        sourceComponent = root.textComponent
                }

                onLoaded: {
                    if (parent.isIcon)
                    {
                        item.source = keyRoot.iconPath
                    }
                    else
                    {
                        item.text = keyRoot.text
                    }
                }
            }

            ColorOverlay {
                anchors.fill: foregroundLoader
                source: foregroundLoader.item
                visible: keyRoot.activeFocus
                color: root.highlightColor
                cached: true
            }
        }
    }

    Component {
        id: defaultTextComponent

        Text {
            id: text
            anchors.fill: parent
            color: "White"
            font.pixelSize: root.cellSize.width * 0.9
            font.family: "Arial"
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
        }
    }

    Component {
        id: defaultIconComponent

        Image {
            sourceSize: Qt.size(width, height)
        }
    }

    Navigation.onLeft: event.accepted = VK.moveLeft()
    Navigation.onRight: event.accepted = VK.moveRight()
    Navigation.onUp: event.accepted = VK.moveUp()
    Navigation.onDown: event.accepted = VK.moveDown()
    Navigation.onOk: VK.handleKeypress()
    Navigation.onBack: backspace()

    onCurrentRowChanged: VK.updateFocusItem()
    onCurrentColumnChanged: VK.updateFocusItem()

    onKeystateChanged: {
        if (keystate == "upper")
            isLowercase = false
        else if (keystate == "lower")
            isLowercase = true
    }

    Component.onCompleted: {
        VK.updateModelTable()
        VK.updateFocusItem()
    }
}
