import QtQuick 2.3
import QtQuick.Layouts 1.1
import Navigation 0.1

import "."
import "VirtualKeyboard.js" as VK

FocusScope {
    id: root

    height: childrenRect.height
    width: childrenRect.width
    focus: true

    // For now, set these properties by use of UI, but replace with generics once
    // this component makes it into a generic Sonetta module for reuse in various interfaces
    property font font: UI.fonts.standard
    property size keySize: Qt.size(64, 64)
    property int spacing: UI.globalSpacing / 6
    property int padding: UI.globalSpacing / 2

    property color labelColor: UI.colors.label
    property color focusColor: UI.colors.focus
    property color textColor: UI.colors.text

    property int currentRow: -1
    property int currentColumn: -1
    readonly property Item currentItem: VK.getItem(currentRow, currentColumn)

    property Component textDelegate: defaultTextDelegate
    property Component spaceDelegate: defaultTextDelegate
    property Component backDelegate: defaultBackDelegate
    property Component doneDelegate: defaultTextDelegate
    property Component capsDelegate: defaultShiftDelegate
    property Component symbolsDelegate: defaultSymbolsDelegate
    property Component backgroundDelegate: Box {
        anchors.fill: parent
    }

    signal character(var character)
    signal space
    signal back
    signal done

    /*
      Possible values for keyboardState:

      "upper": Keys are uppercase
      "lower": Keys are lowercase
      "shift": Keys are uppercase for exactly one
      "symbolsAndLower": Keys are symbols, but upon return to letters will be lowercase
      "symbolsAndUpper": Keys are symbols, but upon return to letters will be uppercase

      */
    readonly property alias keyboardState: m.state

    QtObject {
        id: m

        property int rowCount
        property int columnCount

        property string state: "shift"

        function extractLabelFromModel(model)
        {
            if (model)
            {
                switch (m.state)
                {
                case "upper":
                case "shift":
                    return model.upper
                case "lower":
                    return model.lower
                case "symbolsAndLower":
                case "symbolsAndUpper":
                    return model.symbol
                default:
                    console.error("VirtualKeyboard: Request to extract label in unknown state.")
                    return ""
                }
            }
        }

        function toggleShift()
        {
            m.state = function() {
                switch (m.state)
                {
                case "lower":
                case "symbolsAndLower":
                    return "shift"
                case "shift":
                    return "upper"
                case "upper":
                case "symbolsandUpper":
                    return "lower";
                }
            } ();
        }

        function toggleSymbols()
        {
            switch (m.state)
            {
            case "symbolsAndLower":
                m.state = "lower"
                break
            case "symbolsAndUpper":
                m.state = "upper"
                break
            case "lower":
            case "shift":
                m.state = "symbolsAndLower"
                break
            case "upper":
                m.state = "symbolsAndUpper"
            }
        }
    }

    ListModel {
        id: keys

        // Modifiers
        ListElement { row: 0; col: 0; upper: "OK"; lower: "OK"; symbol: "OK"; colspan: 2; action: "done"}
        ListElement { row: 1; col: 0; upper: "Back"; lower: "Back"; symbol: "Back"; colspan: 2; action: "back" }
        ListElement { row: 2; col: 0; upper: "Shift"; lower: "Shift"; symbol: "Shift"; colspan: 2; action:"shift" }
        ListElement { row: 3; col: 0; upper: "Symbols"; lower: "Symbols"; symbol: "Symbols"; colspan: 2; action: "symbols" }

        ListElement { row: 0; col: 2; upper: "1"; lower: "1"; symbol: "'" }
        ListElement { row: 0; col: 3; upper: "2"; lower: "2"; symbol: "." }
        ListElement { row: 0; col: 4; upper: "3"; lower: "3"; symbol: "-" }
        ListElement { row: 0; col: 5; upper: "4"; lower: "4"; symbol: "+" }
        ListElement { row: 0; col: 6; upper: "5"; lower: "5"; symbol: "§" }
        ListElement { row: 0; col: 7; upper: "6"; lower: "6"; symbol: "´" }
        ListElement { row: 0; col: 8; upper: "7"; lower: "7"; symbol: "`" }
        ListElement { row: 0; col: 9; upper: "8"; lower: "8"; symbol: "¤" }
        ListElement { row: 0; col: 10; upper: "9"; lower: "9"; symbol: "¨" }
        ListElement { row: 0; col: 11; upper: "0"; lower: "0"; symbol: "~" }

        ListElement { row: 1; col: 2; upper: "A"; lower: "a"; symbol: "@"; }
        ListElement { row: 1; col: 3; upper: "B"; lower: "b"; symbol: "£"; }
        ListElement { row: 1; col: 4; upper: "C"; lower: "c"; symbol: "$"; }
        ListElement { row: 1; col: 5; upper: "D"; lower: "d"; symbol: "^"; }
        ListElement { row: 1; col: 6; upper: "E"; lower: "e"; symbol: "'"; }
        ListElement { row: 1; col: 7; upper: "F"; lower: "f"; symbol: "{"; }
        ListElement { row: 1; col: 8; upper: "G"; lower: "g"; symbol: "}"; }
        ListElement { row: 1; col: 9; upper: "H"; lower: "h"; symbol: "["; }
        ListElement { row: 1; col: 10; upper: "I"; lower: "i"; symbol: "]"; }
        ListElement { row: 1; col: 11; upper: "J"; lower: "j"; symbol: "*"; }

        ListElement { row: 2; col: 2; upper: "K"; lower: "k"; symbol: "="; }
        ListElement { row: 2; col: 3; upper: "L"; lower: "l"; symbol: "?"; }
        ListElement { row: 2; col: 4; upper: "M"; lower: "m"; symbol: "!"; }
        ListElement { row: 2; col: 5; upper: "N"; lower: "n"; symbol: "\""; }
        ListElement { row: 2; col: 6; upper: "O"; lower: "o"; symbol: "#"; }
        ListElement { row: 2; col: 7; upper: "P"; lower: "p"; symbol: "$"; }
        ListElement { row: 2; col: 8; upper: "Q"; lower: "q"; symbol: "%"; }
        ListElement { row: 2; col: 9; upper: "R"; lower: "r"; symbol: "&"; }
        ListElement { row: 2; col: 10; upper: "S"; lower: "s"; symbol: "/"; }
        ListElement { row: 2; col: 11; upper: "T"; lower: "t"; symbol: "("; }

        ListElement { row: 3; col: 2; upper: "Space"; lower: "Space"; symbol: "Space"; colspan: 4; action: "space"}
        ListElement { row: 3; col: 6; upper: "U"; lower: "u"; symbol: ")"; }
        ListElement { row: 3; col: 7; upper: "V"; lower: "v"; symbol: "+"; }
        ListElement { row: 3; col: 8; upper: "W"; lower: "w"; symbol: "-"; }
        ListElement { row: 3; col: 9; upper: "X"; lower: "x"; symbol: "\\"; }
        ListElement { row: 3; col: 10; upper: "Y"; lower: "y"; symbol: "<"; }
        ListElement { row: 3; col: 11; upper: "Z"; lower: "z"; symbol: ">"; }
    }

    GridLayout {
        id: grid
        width: root.keySize.width * columns + (columns - 1) * columnSpacing
        height: root.keySize.height * m.rowCount + (m.rowCount - 1) * rowSpacing
        columns: m.columnCount
        columnSpacing: root.spacing
        rowSpacing: root.spacing

        Repeater {
            id: gridRepeater
            model: keys
            delegate: FocusScope {
                id: delegateRoot
                property string label: m.extractLabelFromModel(model)
                property string action: model.action ? model.action : ""
                property bool hasFocus: delegateRoot.activeFocus

                width: root.keySize.width * Layout.columnSpan + (Layout.columnSpan - 1) * grid.columnSpacing
                height: root.keySize.height * Layout.rowSpan + (Layout.rowSpan - 1) * grid.rowSpacing

                Layout.row: model.row
                Layout.column: model.col
                Layout.rowSpan: model.rowspan ? model.rowspan : 1
                Layout.columnSpan: model.colspan ? model.colspan : 1

                Loader {
                    anchors.fill: parent
                    sourceComponent: root.backgroundDelegate
                }

                Loader {
                    focus: true
                    property alias label: delegateRoot.label
                    property alias action: delegateRoot.action
                    property alias hasFocus: delegateRoot.hasFocus
                    anchors.fill: parent
                    anchors.margins: root.padding
                    sourceComponent: {
                        switch (action)
                        {
                        case "shift":
                            return root.capsDelegate
                        case "space":
                            return root.spaceDelegate
                        case "done":
                            return root.doneDelegate
                        case "back":
                            return root.backDelegate
                        case "symbols":
                            return root.symbolsDelegate
                        default:
                            return root.textDelegate
                        }
                    }
                }

                Navigation.onOk: {
                    if (model.action)
                    {
                        switch (model.action)
                        {
                        case "space":
                            root.space()
                            break
                        case "done":
                            root.done()
                            break
                        case "back":
                            root.back()
                            break
                        case "shift":
                            m.toggleShift()
                            break
                        case "symbols":
                            m.toggleSymbols()
                            return
                        }
                    }
                    else
                    {
                        root.character(m.extractLabelFromModel(model))
                        if (m.state === "shift")
                            m.state = "lower"
                    }
                }
            }
        }
    }

    onActiveFocusChanged: {
        if (activeFocus) {
            VK.updateFocus()
        }
    }

    Navigation.onRight: event.accepted = VK.moveColumn(+1)
    Navigation.onLeft: event.accepted = VK.moveColumn(-1)
    Navigation.onDown: event.accepted = VK.moveRow(+1)
    Navigation.onUp: event.accepted = VK.moveRow(-1)
    Navigation.onBack: root.back()

    Keys.onPressed: {
        if (event.text !== "" && event.text.length === 1)
        {
            event.accepted = true
            root.character(event.text);
        }
    }

    Component.onCompleted: {
        VK.initialize()
    }

    Component {
        id: defaultTextDelegate
        Text {
            id: keyText
            font: root.font
            text: label
            anchors.fill: parent
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter

            color: hasFocus ? root.focusColor : root.textColor
        }
    }

    Component {
        id: defaultBackDelegate
        Image {
            anchors.fill: parent
            sourceSize: Qt.size(width, height)
            fillMode: Image.PreserveAspectFit
            mipmap: true
            source: hasFocus ? "../images/keys/backspace_focus.png" : "../images/keys/backspace.png"
        }
    }

    Component {
        id: defaultShiftDelegate
        Image {
            id: shiftIcon
            anchors.fill: parent
            sourceSize: Qt.size(width, height)
            fillMode: Image.PreserveAspectFit
            mipmap: true
            source: hasFocus ? "../images/keys/caps_focus.png" : "../images/keys/caps.png"

            states: [
                State {
                    when: root.keyboardState === "shift"
                    PropertyChanges { target: shiftIcon; rotation: 90 }
                },
                State {
                    when: root.keyboardState === "lower" || root.keyboardState === "symbolsAndLower"
                    PropertyChanges { target: shiftIcon; rotation: 180 }
                },
                State {
                    when: root.keyboardState === "upper" || root.keyboardState === "symbolsAndUpper"
                    PropertyChanges { target: shiftIcon; rotation: 0 }
                }
            ]

            transitions: Transition {
                RotationAnimation { duration: UI.timing.highlightMove }
            }
        }
    }

    Component {
        id: defaultSymbolsDelegate
        Text {
            id: keyText
            anchors.fill: parent
            font: root.font
            color: parent.activeFocus ? root.focusColor : root.textColor
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            text: (root.keyboardState == "symbolsAndLower" || root.keyboardState == "symbolsAndUpper") ? "ABC" : "#$!"
        }
    }
}
