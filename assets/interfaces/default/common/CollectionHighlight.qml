import QtQuick 2.2

Item {
    id: highlightContainer

    property var currentItem
    property bool contextEnabled: false
    property bool contextActive: false
    readonly property real indicatorWidth: contextEnabled ? contextIndicator.width + anchors.rightMargin : 0

    Behavior on height {
        SmoothedAnimation { duration: ui.misc.globalAnimationTime; velocity: -1 }
    }

    Rectangle {
        id: highlight
        color: ui.colors.highlight
        height: currentItem.highlightHeight ? currentItem.highlightHeight : currentItem.height
        width: currentItem ? Math.min(currentItem.width, ui.misc.globalPadding / 2) : 0

        states: [
            State {
                when: currentItem && currentItem.activeFocus && !contextActive
                PropertyChanges { target: highlight; width: currentItem.width }
            }
        ]

        Behavior on width {
            SmoothedAnimation { duration: ui.misc.globalAnimationTime; velocity: -1 }
        }
    }

    Image {
        id: contextIndicator
        visible: contextEnabled && currentItem.activeFocus
        anchors {
            verticalCenter: highlight.verticalCenter
            right: parent.right
            rightMargin: ui.misc.globalPadding
        }

        height: 35
        width: 35
        sourceSize.height: height
        sourceSize.width: width
        source: "../images/contextArrow.svg"
        cache: true

        states: [
            State {
                when: contextActive
                PropertyChanges { target: contextIndicator; rotation: -180 }
            },
            State {
                when: !contextActive
                PropertyChanges { target: contextIndicator; rotation: 0 }
            }
        ]

        transitions: Transition {
            RotationAnimator {
                duration: ui.misc.globalAnimationTime
            }
        }

        Behavior on rotation {
            RotationAnimation {
                duration: ui.misc.globalAnimationTime
            }
        }

        SequentialAnimation {
            id: contextIndicatorAnimation
            loops: Animation.Infinite
            running: contextEnabled && currentItem.activeFocus
            alwaysRunToEnd: true
            OpacityAnimator {
                target: contextIndicator
                from: 0.2
                to: 1.0
                duration: 500
                easing.type: Easing.InOutQuad
            }
            OpacityAnimator {
                target: contextIndicator
                from: 1.0
                to: 0.2
                duration: 500
                easing.type: Easing.InOutQuad
            }
        }
    }
}
