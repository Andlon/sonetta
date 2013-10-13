import QtQuick 2.1
import Sonetta 0.1

CollectionView {
    id: root

    delegate: CollectionDelegate {
        height: 75
        Text {
            anchors {
                fill: parent
                leftMargin: ui.misc.globalPadding
                rightMargin: ui.misc.globalPadding
            }

            text: modelData
            font: ui.fonts.h4
            color: (activeFocus && isCurrentItem) ? ui.colors.highlightText : ui.colors.standard
            verticalAlignment: Text.AlignVCenter
            elide: Text.ElideRight

            Behavior on color {
                ColorAnimation { duration: ui.misc.globalAnimationTime; easing.type: Easing.InOutQuint }
            }
        }
    }
}
