import QtQuick 2.0
import QtQuick.Layouts 1.0
import QtQml.Models 2.1
import QtGraphicalEffects 1.0

import Sonetta 0.1

import "VirtualKeyboard.js" as VK

FocusScope {
    id: root
    width: childrenRect.width
    height: childrenRect.height

    signal character (string c)
    signal prev
    signal next
    signal enter
    signal backspace
    signal cancel

    property bool wrapNavigationTop: false
    property bool wrapNavigationBottom: false
    property bool wrapNavigationLeft: false
    property bool wrapNavigationRight: false

    //    property url backspaceImage: "../images/keys/backspace.png"
    //    property url returnImage: "../images/keys/return.png"
    //    property url shiftUpImage: "../images/keys/shiftup.png"
    //    property url shiftDownImage: "../images/keys/shiftdown.png"
    //    property url symbolsImage: "../images/keys/symbols.png"
    //    property url lettersImage: "../images/keys/letters.png"
    property url spaceImage: "../images/keys/space.png"
    //    property url prevImage: "../images/keys/prev.png" // Cursor
    //    property url nextImage: "../images/keys/next.png" // Cursor

    property color highlightColor: ui.colors.highlight
    property color color: ui.colors.standard

    property size cellSize: Qt.size(56, 56)

    property int currentRow: 0
    property int currentColumn: 0

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

    ListView {
        id: actions
        focus: true
        model: ["Done", "Back", "Caps", "Shift", "Symbol"]
        width: contentItem.childrenRect.width
        height: childrenRect.height
        delegate: H4 {
            height: root.cellSize.height
            width: contentWidth
            elide: Text.ElideNone
            text: modelData
            color: actions.activeFocus && ListView.isCurrentItem ? ui.colors.highlight : ui.colors.standard
            verticalAlignment: Text.AlignVCenter
        }

        Navigation.onRight: focusGrid()

        Navigation.onOk: {
            if (currentItem)
            {
                switch (currentIndex)
                {
                case 0:
                    VK.handleAction("return")
                    break
                case 1:
                    VK.handleAction("backspace")
                    break
                case 2:
                    VK.handleAction("shift")
                    break
                case 3:
                    VK.handleAction("shift")
                    break
                case 4:
                    VK.handleAction("togglesymbols")
                }
            }
        }

        Navigation.onUp: decrementCurrentIndex()
        Navigation.onDown: incrementCurrentIndex()
    }

    Rectangle {
        id: divider
        anchors {
            top: actions.top
            topMargin: ui.misc.globalPadding / 2
            bottom: actions.bottom
            bottomMargin: ui.misc.globalPadding / 2
            left: actions.right
            leftMargin: ui.misc.globalPadding / 2
        }

        color: ui.colors.label
        width: 2
    }

    GridLayout {
        id: grid
        focus: true
        anchors {
            left: divider.right
            leftMargin: ui.misc.globalPadding / 2
        }

        width: VK.getSize().width
        height: VK.getSize().height
        rowSpacing: 0
        columnSpacing: 0
        Repeater {
            id: gridRepeater
            model: keys
            delegate: key
        }


        Navigation.onRight: event.accepted = VK.moveRight()
        Navigation.onUp: event.accepted = VK.moveUp()
        Navigation.onDown: event.accepted = VK.moveDown()
        Navigation.onOk: VK.handleKeypress()

        onActiveFocusChanged: if (activeFocus) VK.updateFocusItem()

        Navigation.onLeft: {
            if (!VK.moveLeft())
            {
                // Could not move key left, change focus to actions
                focusActions()
            }
        }
    }

    ListModel {
        id: keys

        ListElement { row: 0; col: 0; upper: "A"; lower: "a"; symbol: "1"; }
        ListElement { row: 0; col: 1; upper: "B"; lower: "b"; symbol: "2"; }
        ListElement { row: 0; col: 2; upper: "C"; lower: "c"; symbol: "3"; }
        ListElement { row: 0; col: 3; upper: "D"; lower: "d"; symbol: "4"; }
        ListElement { row: 0; col: 4; upper: "E"; lower: "e"; symbol: "5"; }
        ListElement { row: 0; col: 5; upper: "F"; lower: "f"; symbol: "6"; }

        ListElement { row: 1; col: 0; upper: "G"; lower: "g"; symbol: "7"; }
        ListElement { row: 1; col: 1; upper: "H"; lower: "h"; symbol: "8"; }
        ListElement { row: 1; col: 2; upper: "I"; lower: "i"; symbol: "9"; }
        ListElement { row: 1; col: 3; upper: "J"; lower: "j"; symbol: "0"; }
        ListElement { row: 1; col: 4; upper: "K"; lower: "k"; symbol: "="; }
        ListElement { row: 1; col: 5; upper: "L"; lower: "l"; symbol: "?"; }

        ListElement { row: 2; col: 0; upper: "M"; lower: "m"; symbol: "!"; }
        ListElement { row: 2; col: 1; upper: "N"; lower: "n"; symbol: "\""; }
        ListElement { row: 2; col: 2; upper: "O"; lower: "o"; symbol: "#"; }
        ListElement { row: 2; col: 3; upper: "P"; lower: "p"; symbol: "$"; }
        ListElement { row: 2; col: 4; upper: "Q"; lower: "q"; symbol: "%"; }
        ListElement { row: 2; col: 5; upper: "R"; lower: "r"; symbol: "&"; }

        ListElement { row: 3; col: 0; upper: "S"; lower: "s"; symbol: "/"; }
        ListElement { row: 3; col: 1; upper: "T"; lower: "t"; symbol: "("; }
        ListElement { row: 3; col: 2; upper: "U"; lower: "u"; symbol: ")"; }
        ListElement { row: 3; col: 3; upper: "V"; lower: "v"; symbol: "+"; }
        ListElement { row: 3; col: 4; upper: "W"; lower: "w"; symbol: "-"; }
        ListElement { row: 3; col: 5; upper: "X"; lower: "x"; symbol: "\\"; }

        ListElement { row: 4; col: 0; upper: "Y"; lower: "y"; symbol: "<"; }
        ListElement { row: 4; col: 1; upper: "Z"; lower: "z"; symbol: ">"; }
        ListElement { row: 4; col: 2; action: "space"; colspan: 2 }
        ListElement { row: 4; col: 4; upper: ","; lower: ","; symbol: ";"; }
        ListElement { row: 4; col: 5; upper: "."; lower: "."; symbol: ":"; }
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

        H4 {
            id: text
            anchors.fill: parent
            color: "White"
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
        }
    }

    Component {
        id: defaultIconComponent

        Image {
            //sourceSize: Qt.size(width, height)
            fillMode: Image.PreserveAspectFit
        }
    }


    Navigation.onBack: backspace()

    onCurrentRowChanged: VK.updateFocusItem()
    onCurrentColumnChanged: VK.updateFocusItem()

    onKeystateChanged: {
        if (keystate == "upper")
            isLowercase = false
        else if (keystate == "lower")
            isLowercase = true
    }

    Keys.onPressed: {
        // Accept text from keyboard
        if (event.text !== "")
        {
            character(event.text)
        }
    }

    Component.onCompleted: {
        VK.updateModelTable()
        if (activeFocus) VK.updateFocusItem()
    }

    onActiveFocusChanged: {
        if (activeFocus)
        {
            actions.currentIndex = 0
            actions.focus = true
        }
    }

    function focusActions()
    {
        // Determine currentIndex for actions
        actions.currentIndex = currentRow
        actions.focus = true
    }

    function focusGrid()
    {
        // Update row and col to match actions' index
        currentRow = actions.currentIndex
        currentColumn = 0
        grid.focus = true
    }
}
