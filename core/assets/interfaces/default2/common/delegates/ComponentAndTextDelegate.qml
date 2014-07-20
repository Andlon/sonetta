import QtQuick 2.3
import Sonetta 0.1

import QtGraphicalEffects 1.0

import "../../common"

Item {
    id: delegateRoot
    height: imageLoader.height + UI.globalSpacing
    focus: true

    property alias text: textLabel.text
    property alias component: imageLoader.sourceComponent

    states: [
        State {
            when: delegateRoot.activeFocus
            PropertyChanges { target: filter; desaturation: 0.0 }
        },
        State {
            when: !delegateRoot.activeFocus
            PropertyChanges { target: filter; desaturation: 1.0 }
        }
    ]

    transitions: Transition {
        SmoothedAnimation { property: "desaturation"; duration: UI.timing.highlightMove; velocity: -1 }
    }

    Loader {
        id: imageLoader
        anchors {
            left: parent.left
            top: parent.top
            margins: UI.globalSpacing / 2
        }

        width: UI.views.imageSize.width
        height: UI.views.imageSize.height
    }

    Desaturate {
        id: filter
        anchors.fill: imageLoader
        source: imageLoader
    }

    FocusText {
        id: textLabel
        font: UI.fonts.standard
        elide: Text.ElideRight
        hasFocus: delegateRoot.activeFocus

        anchors {
            verticalCenter: imageLoader.verticalCenter
            left: imageLoader.right
            right: parent.right
            margins: UI.globalSpacing
        }
    }
}
