import QtQuick 2.2
import Sonetta 0.1

CollectionView {
    id: root

    delegate: CollectionDelegate {
        height: 75

        states: [
            State {
                when: root.activeFocus && isCurrentItem
                PropertyChanges { target: label; color: ui.colors.highlightText }
            },
            State {
                when: !root.activeFocus || !isCurrentItem
                PropertyChanges { target: label; color: ui.colors.standard }
            }
        ]

        transitions: Transition {
            ColorAnimation {
                duration: ui.misc.globalAnimationTime
                easing.type: Easing.InOutQuad
            }
        }

        Text {
            id: label
            anchors {
                fill: parent
                leftMargin: ui.misc.globalPadding
                rightMargin: ui.misc.globalPadding
            }

            text: modelData
            font: ui.fonts.h4
            verticalAlignment: Text.AlignVCenter
            elide: Text.ElideRight

            Behavior on color {
                ColorAnimation { duration: ui.misc.globalAnimationTime; easing.type: Easing.InOutQuint }
            }
        }
    }
}
