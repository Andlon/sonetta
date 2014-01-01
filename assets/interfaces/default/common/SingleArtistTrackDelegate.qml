import QtQuick 2.2
import "Time.js" as Time

CollectionDelegate {
    id: del
    property int index

    states: [
        State {
            when: activeFocus
            PropertyChanges { target: indexLabel; color: ui.colors.highlightText }
        },
        State {
            when: !activeFocus
            PropertyChanges { target: indexLabel; color: ui.colors.standard }
        }
    ]

    transitions: Transition {
        ColorAnimation {
            duration: ui.misc.globalAnimationTime
            easing.type: Easing.InOutQuad
        }
    }

    height: childrenRect.height + ui.misc.globalPadding
    y: ui.misc.globalPadding / 2

    H4 {
        id: indexLabel
        anchors {
            left: parent.left
        }

        text: index + "."
        width: 50
        verticalAlignment: Text.AlignVCenter
        horizontalAlignment: Text.AlignRight
    }

    H4 {
        text: model.name
        color: indexLabel.color
        anchors {
            left: indexLabel.right
//            right: duration.left
            right: parent.right
            margins: ui.misc.globalPadding
        }
        verticalAlignment: Text.AlignVCenter
    }

//    H4 {
//        id: duration
//        color: indexLabel.color
//        anchors {
//            right: parent.right
//            margins: ui.misc.globalPadding
//        }
//        text: Time.formatTime(model.duration)
//        width: contentWidth
//        verticalAlignment: Text.AlignVCenter
//        horizontalAlignment: Text.AlignRight
//        elide: Text.ElideNone
//    }
}
