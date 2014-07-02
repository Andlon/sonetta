import QtQuick 2.3
import Navigation 0.1
import "../../common"

FocusScope {
    id: root
    height: col.height + col.anchors.topMargin + col.anchors.bottomMargin

    onActiveFocusChanged: {
        if (activeFocus) {
            switch (state) {
            case "playSelected":
                state = "playFocused"
                break
            case "nextSelected":
                state = "nextFocused"
                break
            case "prevSelected":
                state = "prevFocused"
                break
            default:
                state = "playFocused"
                return
            }
        }
        else {
            switch (state) {
            case "playFocused":
                state = "playSelected"
                break
            case "nextFocused":
                state = "nextSelected"
                break
            case "prevFocused":
                state = "prevSelected"
                break
            default:
                state = "playSelected"
                return
            }
        }
    }

    states: [
        State {
            name: "selected"
            PropertyChanges { target: highlight; color: UI.colors.currentUnfocused }
        },
        State {
            name: "playSelected"
            extend: "selected"
            PropertyChanges { target: play; selected: true }
            PropertyChanges { target: next; selected: false }
            PropertyChanges { target: prev; selected: false }
            AnchorChanges { target: highlight;
                anchors.top: play.top;
                anchors.bottom: play.bottom;
                anchors.left: play.left;
                anchors.right: play.right }
        },
        State {
            name: "nextSelected"
            extend: "selected"
            PropertyChanges { target: play; selected: false }
            PropertyChanges { target: next; selected: true }
            PropertyChanges { target: prev; selected: false }
            AnchorChanges { target: highlight;
                anchors.top: next.top;
                anchors.bottom: next.bottom;
                anchors.left: next.left;
                anchors.right: next.right }
        },
        State {
            name: "prevSelected"
            extend: "selected"
            PropertyChanges { target: play; selected: false }
            PropertyChanges { target: next; selected: false }
            PropertyChanges { target: prev; selected: true }
            AnchorChanges { target: highlight;
                anchors.top: prev.top;
                anchors.bottom: prev.bottom;
                anchors.left: prev.left;
                anchors.right: prev.right }
        },
        State {
            name: "playFocused"
            extend: "playSelected"
            PropertyChanges { target: highlight; color: UI.colors.focus }
            PropertyChanges { target: play; focus: true }
        },
        State {
            name: "nextFocused"
            extend: "nextSelected"
            PropertyChanges { target: highlight; color: UI.colors.focus }
            PropertyChanges { target: next; focus: true }
        },
        State {
            name: "prevFocused"
            extend: "prevSelected"
            PropertyChanges { target: highlight; color: UI.colors.focus }
            PropertyChanges { target: prev; focus: true }
        }
    ]
    state: "playSelected"

    transitions: Transition {
        ColorAnimation { property: "color"; duration: UI.timing.highlightMove }
        AnchorAnimation { duration: UI.timing.highlightMove }
    }

    Box {
        anchors.fill: parent
    }

    Column {
        id: col
        anchors {
            left: root.left
            right: root.right
            top: root.top
            margins: UI.globalSpacing
        }

        height: childrenRect.height
        spacing: UI.globalSpacing / 2

        Item {
            id: controls

            height: childrenRect.height
            focus: true
            anchors {
                left: parent.left
                right: parent.right
            }

            Rectangle {
                id: highlight
                border.width: 1
                border.color: UI.colors.dark
            }

            ColoredIcon {
                id: play
                source: player.playing ? "../../images/playback/pause.svg" : "../../images/playback/play.svg"
                anchors {
                    top: controls.top
                    left: controls.left
                }

                height: 96
                width: 96
                unselectedColor: UI.colors.label

                Navigation.onRight: root.state = "prevFocused"
                Navigation.onOk: player.playPause()
            }

            ColoredIcon {
                id: prev
                source: "../../images/playback/rewind.svg"
                anchors {
                    top: controls.top
                    left: play.right
                }

                height: 96
                width: 96
                unselectedColor: UI.colors.label

                Navigation.onLeft: root.state = "playFocused"
                Navigation.onRight: root.state = "nextFocused"
            }

            ColoredIcon {
                id: next
                source: "../../images/playback/fastforward.svg"
                anchors {
                    top: controls.top
                    left: prev.right
                }

                height: 96
                width: 96
                unselectedColor: UI.colors.label

                Navigation.onLeft: root.state = "prevFocused"
                Navigation.onOk: player.next()
            }

            ColoredIcon {
                id: volume
                source: "../../images/playback/volume.svg"
                anchors {
                    verticalCenter: play.verticalCenter
                    right: repeat.left
                }

                height: 72
                width: 72

                unselectedColor: UI.colors.label
                selectedColor: UI.colors.label
            }

            ColoredIcon {
                id: repeat
                source: "../../images/playback/repeat.svg"
                anchors {
                    verticalCenter: play.verticalCenter
                    right: shuffle.left
                }

                height: 72
                width: 72

                unselectedColor: UI.colors.label
                selectedColor: UI.colors.label
            }

            ColoredIcon {
                id: shuffle
                source: "../../images/playback/shuffle.svg"
                anchors {
                    verticalCenter: play.verticalCenter
                    right: controls.right
                }

                unselectedColor: UI.colors.label
                selectedColor: UI.colors.label

                height: 72
                width: 72
            }
        }

        Trackbar {
            id: trackbar
            anchors {
                left: parent.left
                right: parent.right
            }

            fillColor: UI.colors.currentUnfocused
            barColor: UI.colors.medium
        }
    }
}
