import QtQuick 2.0
import com.bitfraction.sonata 0.1


FocusScope {
    id: root
    property size cellSize: Qt.size(64, 64)
    property string inputText
    property int position: 0
    property bool isPassword: false

    signal closed

    visible: false
    anchors.fill: parent

    function show(text)
    {
        visible = true;
        loader.focus = true;
        inputText = text;
        position = text.length
    }

    function close()
    {
        visible = false;
        root.focus = false;
        root.closed()
    }


    Keys.onPressed: {
        // Prevent key presses from propagating
        event.accepted = true;
    }

    Navigation.onNavigationEvent: {
        // Prevent navigation events from propagating
        event.accepted = true;
    }

    MouseArea {
        // Prevent mouse events from propagating
        anchors.fill: parent
        preventStealing: true
    }

    Loader {
        id: loader
        width: childrenRect.width
        height: childrenRect.height

        active: root.visible

        y: root.visible ? parent.height - height : parent.height
        anchors.horizontalCenter: parent.horizontalCenter

        Behavior on y {
            NumberAnimation { }
        }

        sourceComponent: FocusScope {
            id: keyboard

            property bool isUppercase: false
            property bool isSymbols: false

            width: content.width + 2 * background.radius
            height: content.height + 2 * background.radius

            function switchCase()
            {
                keyboard.isUppercase = !keyboard.isUppercase;
            }

            function switchSymbols()
            {
                keyboard.isSymbols = !keyboard.isSymbols;
            }

            Rectangle {
                id: background

                anchors.fill: parent

                color: "#88000000"
                radius: 15
            }

            Item {
                id: content
                height: childrenRect.height
                width: childrenRect.width

                x: background.radius
                y: background.radius

                focus: true

                property int currentRow: 0
                property int currentCol: 0

                Navigation.onBack: inputText.backspace()
                Navigation.onRight: { updatePosition(currentRow, currentCol + 1) }
                Navigation.onLeft: { updatePosition(currentRow, currentCol - 1) }
                Navigation.onUp: { updatePosition(currentRow - 1, currentCol) }
                Navigation.onDown: { updatePosition(currentRow + 1, currentCol) }

                Keys.onPressed: {
                    if (event.text)
                    {
                        inputText.insertText(event.text)
                    }
                }

                onFocusChanged: if (activeFocus) updatePosition(currentRow, currentCol)
                Component.onCompleted:{  updatePosition(currentRow, currentCol); leftButtons.forceActiveFocus(); }

                Rectangle {
                    id: inputRect
                    width: buttonGrid.width
                    height: 50
                    color: "transparent"
                    anchors.left: buttonGrid.left
                    anchors.right: buttonGrid.right
                    clip: true
                    TextInput {
                        id: inputText
                        height: 50
                        anchors { left: parent.left; right: parent.right; verticalCenter: parent.verticalCenter }
                        font.pointSize: 19
                        font.family: "Roboto"
                        text: root.inputText
                        readOnly: true
                        cursorVisible: true
                        activeFocusOnPress: false
                        color: "White"
                        echoMode: root.isPassword ? TextInput.Password : TextInput.Normal

                        onTextChanged: root.inputText = text

                        function backspace()
                        {
                            if (root.position > 0)
                            {
                                var text = inputText.text
                                text = text.substring(0, root.position - 1) + text.substring(root.position)
                                root.position -= 1;
                                inputText.text = text;
                            }
                        }


                        function insertText(text)
                        {
                            inputText.insert(root.position, text)
                            root.position += 1
                        }
                    }
                }

                Grid {
                    anchors { top: inputRect.bottom; left: leftButtons.right }
                    id: buttonGrid
                    width: childrenRect.width
                    height: childrenRect.height
                    columns: 10
                    Repeater {
                        model: characters
                        delegate: buttonComponent
                    }
                }

                function updatePosition(row, col)
                {
                    // Take in account the fact that Repeater is a child of Grid
                    var childrenCount = buttonGrid.children.length - 1;
                    // Translate to index
                    var colCount = buttonGrid.columns;
                    var childIndex = row * colCount + col;

                    if (col < 0)
                    {
                        // Move left
                        currentCol = 0;
                        leftButtons.forceActiveFocus()
                    }
                    else if (col >= 0 && col < colCount && childIndex < childrenCount)
                    {
                        buttonGrid.children[childIndex].forceActiveFocus();
                        currentRow = row;
                        currentCol = col;
                    }
                    else if (col >= colCount)
                    {
                        // Move right
                        currentCol = colCount - 1;
                        rightButtons.forceActiveFocus()
                    }
                }

                NavColumn {
                    id: leftButtons

                    anchors { top: buttonGrid.top; left: content.left; }
                    onFocusChanged: if (activeFocus) focusIndex = content.currentRow

                    ImageButton {
                        id: okButton

                        height: root.cellSize.height
                        width: 2 * root.cellSize.width
                        source: "button.png"
                        focusSource: "button_focus.png"

                        label: "Done"

                        onClicked: root.close()
                    }

                    ImageButton {
                        id: capsButton
                        height: root.cellSize.height
                        width: 2 * root.cellSize.width
                        source: keyboard.isUppercase ? "button.png" : "button.png"
                        focusSource: keyboard.isUppercase ? "button_focus.png" : "button_focus.png"
                        label: "Caps"

                        onClicked: { keyboard.switchCase() }
                    }

                    ImageButton {
                        id: shiftButton
                        height: root.cellSize.height
                        width: 2 * root.cellSize.width
                        source: keyboard.isUppercase ? "button.png" : "button.png"
                        focusSource: keyboard.isUppercase ? "button_focus.png" : "button_focus.png"
                        label: "Shift"

                        onClicked: { keyboard.switchSymbols() }
                    }

                    Navigation.onRight: content.updatePosition(focusIndex, 0)


                }

                NavColumn {
                    id: rightButtons
                    anchors { top: buttonGrid.top; left: buttonGrid.right; }
                    onFocusChanged: if (activeFocus) focusIndex = content.currentRow

                    ImageButton {
                        id: backButton
                        height: root.cellSize.height
                        width: 2 * root.cellSize.width
                        source: "button.png"
                        focusSource: "button_focus.png"

                        label: "Back"

                        onClicked: inputText.backspace()
                    }

                    ImageButton {
                        id: spaceButton
                        height: root.cellSize.height
                        width: 2 * root.cellSize.width
                        source: "button.png"
                        focusSource: "button_focus.png"

                        label: "Space"

                        onClicked: inputText.insertText(" ")
                    }

                    ImageButton {
                        id: symbolsButton
                        height: root.cellSize.height
                        width: 2 * root.cellSize.width
                        source: keyboard.isUppercase ? "button.png" : "button.png"
                        focusSource: keyboard.isUppercase ? "button_focus.png" : "button_focus.png"
                        label: "Symbols"

                        onClicked: { keyboard.switchSymbols() }
                    }

                    Navigation.onLeft: content.updatePosition(focusIndex, buttonGrid.columns - 1)
                }
            }


            Component {
                id: buttonComponent

                ImageButton {
                    source: "button.png"
                    focusSource: "button_focus.png"
                    height: root.cellSize.height
                    width: root.cellSize.width

                    label: keyboard.isSymbols ? model.symbol : keyboard.isUppercase ? model.uppercase : model.lowercase

                    Navigation.onOk: inputText.insertText(label)
                }
            }

            ListModel {
                id: characters

                ListElement { uppercase: "Q"; lowercase: "q"; symbol: "1" }
                ListElement { uppercase: "W"; lowercase: "w"; symbol: "2" }
                ListElement { uppercase: "E"; lowercase: "e"; symbol: "3" }
                ListElement { uppercase: "R"; lowercase: "r"; symbol: "4" }
                ListElement { uppercase: "T"; lowercase: "t"; symbol: "5" }
                ListElement { uppercase: "Y"; lowercase: "y"; symbol: "6" }
                ListElement { uppercase: "U"; lowercase: "u"; symbol: "7" }
                ListElement { uppercase: "I"; lowercase: "i"; symbol: "8" }
                ListElement { uppercase: "O"; lowercase: "o"; symbol: "9" }
                ListElement { uppercase: "P"; lowercase: "p"; symbol: "0" }

                ListElement { uppercase: "A"; lowercase: "a"; symbol: "!" }
                ListElement { uppercase: "S"; lowercase: "s"; symbol: "\"" }
                ListElement { uppercase: "D"; lowercase: "d"; symbol: "#" }
                ListElement { uppercase: "F"; lowercase: "f"; symbol: "$" }
                ListElement { uppercase: "G"; lowercase: "g"; symbol: "%" }
                ListElement { uppercase: "H"; lowercase: "h"; symbol: "&" }
                ListElement { uppercase: "J"; lowercase: "j"; symbol: "/" }
                ListElement { uppercase: "K"; lowercase: "k"; symbol: "(" }
                ListElement { uppercase: "L"; lowercase: "l"; symbol: ")" }
                ListElement { uppercase: "\'"; lowercase: "\'"; symbol: "=" }

                ListElement { uppercase: "Z"; lowercase: "z"; symbol: "+" }
                ListElement { uppercase: "X"; lowercase: "x"; symbol: "?" }
                ListElement { uppercase: "C"; lowercase: "c"; symbol: "\\" }
                ListElement { uppercase: "V"; lowercase: "v"; symbol: "*" }
                ListElement { uppercase: "B"; lowercase: "b"; symbol: "<" }
                ListElement { uppercase: "N"; lowercase: "n"; symbol: ">" }
                ListElement { uppercase: "M"; lowercase: "m"; symbol: "£" }
                ListElement { uppercase: ","; lowercase: ","; symbol: ";" }
                ListElement { uppercase: "."; lowercase: "."; symbol: ":" }
                ListElement { uppercase: "-"; lowercase: "_"; symbol: "€" }
            }
        }
    }

    Component.onCompleted: {
        // Reparent to root item
        var p = parent;
        var newParent = p;
        while (newParent)
        {
            p = newParent;
            newParent = p.parent;
        }
        parent = p;
    }


}
