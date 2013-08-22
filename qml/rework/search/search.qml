import QtQuick 2.1
import "../common"
import sonata 0.1
import QtQml.Models 2.1

FocusScope {
    id: root

    state: "query"
    states: [
        State {
            name: "query"
            PropertyChanges { target: pageView; currentIndex: 0 }
            StateChangeScript { script: updateFocus() }
        },
        State {
            name: "result"
            PropertyChanges { target: pageView; currentIndex: 1 }
            StateChangeScript { script: updateFocus() }
        }
    ]

    SpotifySearch {
        id: searchEngine
        session: spotify

        onQueryChanged: resultPage.resetIndices()
    }

    Image {
        id: searchSymbol
        source: "../images/search.png"
        sourceSize: Qt.size(header.height, header.height)
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.margins: 20
        opacity: 0.30
    }

    Text {
        id: header
        anchors.right: searchSymbol.left
        anchors.rightMargin: 20
        anchors.verticalCenter: searchSymbol.verticalCenter
        font: ui.fonts.h1
        color: ui.colors.defaultText
        text: "search"
        opacity: 0.60
    }

    PageView {
        id: pageView
        anchors.fill: parent

        Page {
            id: queryPage
            anchors.topMargin: ui.misc.verticalContentMargin
            anchors.bottomMargin: ui.misc.verticalContentMargin
            anchors.leftMargin: 50
            anchors.rightMargin: 50

            Column {
                anchors.centerIn: parent
                spacing: 50
                PlaceholderInputField {
                    id: searchField
                    placeholder: "enter search term..."
                    width: keyboard.width
                    height: 80
                    font: ui.fonts.h1
                    horizontalAlignment: Text.AlignHCenter
                    color: ui.colors.defaultText
                    input.readOnly: true
                    input.selectByMouse: false
                    input.cursorVisible: true
                    anchors.horizontalCenter: parent.horizontalCenter
                }

                VirtualKeyboard {
                    id: keyboard
                    anchors.horizontalCenter: searchField.horizontalCenter
                    focus: true
                    onCharacter: searchField.append(c)
                    onBackspace: searchField.backspace()
                    wrapNavigationLeft: false

                    Keys.onPressed: {
                        searchField.append(event.text)
                        event.accepted = true
                    }

                    onEnter: root.performSearch()
                }
            }
        }

        ResultPage {
            id: resultPage
            trackModel: searchEngine.tracks
            albumModel: searchEngine.albums
            artistModel: searchEngine.artists
        }
    }

    function performSearch()
    {
        searchEngine.search(searchField.text)
        state = "result"
    }

    function updateFocus()
    {
        if (state === "query")
            queryPage.forceActiveFocus()
        else if (state === "result")
            resultPage.forceActiveFocus()
        else
            console.log("search.qml: Invalid state for updateFocus()")
    }

    Navigation.onBack: {
        if (state === "result")
            state = "query"
        else
            event.accepted = false
    }
}
