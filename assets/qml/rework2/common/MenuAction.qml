import QtQuick 2.1
import Sonetta 0.1

Item {
    id: root
    property alias text: label.text
    property bool toggleable: false
    property bool toggled: false

    height: 75
    width: parent.width

    property int index: 0

    property bool __tag: true

    Image {
        id: image
        x: root.x
        y: root.y
        width: root.width
        height: root.height

        z: -3
        parent: root.parent.parent

        source: "../images/dark.png"
        fillMode: Image.Tile
    }

    Rectangle {
        y: root.y
        x: root.x
        width: root.width

        parent: root.parent.parent
        z: -2

        height: 2
        color: ui.colors.light
        visible: index !== 0
    }

    Text {
        id: label

        anchors {
            left: parent.left
            right: toggleIndicator.left
            leftMargin: ui.misc.globalPadding
            rightMargin: ui.misc.globalPadding
            verticalCenter: parent.verticalCenter
        }

        font: ui.fonts.h4
        color: (activeFocus && isCurrentItem) ? ui.colors.highlightText : ui.colors.standard
        verticalAlignment: Text.AlignVCenter
        elide: Text.ElideRight
        height: contentHeight

        Behavior on color {
            ColorAnimation { duration: ui.misc.globalAnimationTime; easing.type: Easing.InOutQuint }
        }
    }

    Rectangle {
        id: toggleIndicator
        anchors {
            verticalCenter: parent.verticalCenter
            right: parent.right
            rightMargin: ui.misc.globalPadding
        }

        visible: root.toggleable
        rotation: 45
        width: height
        height: 20
        antialiasing: true
        border.color: ui.colors.light
        border.width: 1

        states: [
            State {
                when: !root.toggled
                PropertyChanges {
                    target: toggleIndicator
                    opacity: 0
                }
            },
            State {
                when: root.toggled && !root.activeFocus
                PropertyChanges {
                    target: toggleIndicator
                    opacity: 1
                    color: ui.colors.highlight
                }
            },
            State {
                when: root.toggled && root.activeFocus
                PropertyChanges {
                    target: toggleIndicator
                    opacity: 1
                    color: ui.colors.standard
                }
            }
        ]

        transitions: Transition {
            ColorAnimation { properties: "color"; duration: ui.misc.globalAnimationTime }
        }
    }

    Navigation.onOk: {
        if (toggleable)
        {
            toggled = !toggled
        }
    }
}
