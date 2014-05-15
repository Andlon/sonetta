import QtQuick 2.2
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

    property color labelColor: UI.colors.label
    property color focusColor: UI.colors.focus
    property color textColor: UI.colors.text

    property int currentRow: -1
    property int currentColumn: -1
    readonly property Item currentItem: VK.getItem(currentRow, currentColumn)

    QtObject {
        id: m

        property int rowCount
        property int columnCount
    }

    ListModel {
        id: keys

        // Modifiers
        ListElement { row: 0; col: 0; upper: "Done"; lower: "Done"; symbol: "Done"; colspan: 3 }
        ListElement { row: 1; col: 0; upper: "Back"; lower: "Backspace"; symbol: "Backspace"; colspan: 3 }
        ListElement { row: 2; col: 0; upper: "Shift"; lower: "Shift"; symbol: "Shift"; colspan: 3 }
        ListElement { row: 3; col: 0; upper: "Symbols"; lower: "Symbols"; symbol: "Symbols"; colspan: 3 }

        ListElement { row: 0; col: 3; upper: "1"; lower: "1"; symbol: "|" }
        ListElement { row: 0; col: 4; upper: "2"; lower: "2"; symbol: "|" }
        ListElement { row: 0; col: 5; upper: "3"; lower: "3"; symbol: "|" }
        ListElement { row: 0; col: 6; upper: "4"; lower: "4"; symbol: "|" }
        ListElement { row: 0; col: 7; upper: "5"; lower: "5"; symbol: "|" }
        ListElement { row: 0; col: 8; upper: "6"; lower: "6"; symbol: "|" }
        ListElement { row: 0; col: 9; upper: "7"; lower: "7"; symbol: "|" }
        ListElement { row: 0; col: 10; upper: "8"; lower: "8"; symbol: "|" }
        ListElement { row: 0; col: 11; upper: "9"; lower: "9"; symbol: "|" }
        ListElement { row: 0; col: 12; upper: "0"; lower: "0"; symbol: "|" }

        ListElement { row: 1; col: 3; upper: "A"; lower: "a"; symbol: "|"; }
        ListElement { row: 1; col: 4; upper: "B"; lower: "b"; symbol: "|"; }
        ListElement { row: 1; col: 5; upper: "C"; lower: "c"; symbol: "|"; }
        ListElement { row: 1; col: 6; upper: "D"; lower: "d"; symbol: "|"; }
        ListElement { row: 1; col: 7; upper: "E"; lower: "e"; symbol: "|"; }
        ListElement { row: 1; col: 8; upper: "F"; lower: "f"; symbol: "|"; }
        ListElement { row: 1; col: 9; upper: "G"; lower: "g"; symbol: "|"; }
        ListElement { row: 1; col: 10; upper: "H"; lower: "h"; symbol: "|"; }
        ListElement { row: 1; col: 11; upper: "I"; lower: "i"; symbol: "|"; }
        ListElement { row: 1; col: 12; upper: "J"; lower: "j"; symbol: "|"; }

        ListElement { row: 2; col: 3; upper: "K"; lower: "k"; symbol: "="; }
        ListElement { row: 2; col: 4; upper: "L"; lower: "l"; symbol: "?"; }
        ListElement { row: 2; col: 5; upper: "M"; lower: "m"; symbol: "!"; }
        ListElement { row: 2; col: 6; upper: "N"; lower: "n"; symbol: "\""; }
        ListElement { row: 2; col: 7; upper: "O"; lower: "o"; symbol: "#"; }
        ListElement { row: 2; col: 8; upper: "P"; lower: "p"; symbol: "$"; }
        ListElement { row: 2; col: 9; upper: "Q"; lower: "q"; symbol: "%"; }
        ListElement { row: 2; col: 10; upper: "R"; lower: "r"; symbol: "&"; }
        ListElement { row: 2; col: 11; upper: "S"; lower: "s"; symbol: "/"; }
        ListElement { row: 2; col: 12; upper: "T"; lower: "t"; symbol: "("; }

        ListElement { row: 3; col: 3; upper: "Space"; lower: "Space"; symbol: "Space"; colspan: 2 }
        ListElement { row: 3; col: 5; upper: "U"; lower: "u"; symbol: ")"; }
        ListElement { row: 3; col: 6; upper: "V"; lower: "v"; symbol: "+"; }
        ListElement { row: 3; col: 7; upper: "W"; lower: "w"; symbol: "-"; }
        ListElement { row: 3; col: 8; upper: "X"; lower: "x"; symbol: "\\"; }
        ListElement { row: 3; col: 9; upper: "Y"; lower: "y"; symbol: "<"; }
        ListElement { row: 3; col: 10; upper: "Z"; lower: "z"; symbol: ">"; }
        ListElement { row: 3; col: 11; upper: ","; lower: ","; symbol: ";"; }
        ListElement { row: 3; col: 12; upper: "."; lower: "."; symbol: ":"; }


    }

    GridLayout {
        id: grid
        width: root.keySize.width * columns + (columns - 1) * columnSpacing
        height: root.keySize.height * m.rowCount + (m.rowCount - 1) * rowSpacing
        columns: 13
        columnSpacing: root.spacing
        rowSpacing: root.spacing

        Repeater {
            id: gridRepeater
            model: keys
            delegate: Box {
                property string text: model.upper

                width: root.keySize.width * Layout.columnSpan + (Layout.columnSpan - 1) * grid.columnSpacing
                height: root.keySize.height * Layout.rowSpan + (Layout.rowSpan - 1) * grid.rowSpacing

                Layout.row: model.row
                Layout.column: model.col
                Layout.rowSpan: model.rowspan ? model.rowspan : 1
                Layout.columnSpan: model.colspan ? model.colspan : 1

                Text {
                    id: keyText
                    font: root.font
                    color: parent.activeFocus ? root.focusColor : root.textColor
                    text: parent.text
                    anchors.centerIn: parent
                }
            }
        }
    }

    onActiveFocusChanged: {
        if (activeFocus) {
            VK.updateFocus()
        }
    }

    Keys.forwardTo: Nav {
        onRight: VK.moveColumn(+1)
        onLeft: VK.moveColumn(-1)
        onDown: VK.moveRow(+1)
        onUp: VK.moveRow(-1)
    }

    Component.onCompleted: {
        VK.initialize()
    }
}
