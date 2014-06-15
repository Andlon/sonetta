import QtQuick 2.3
import Sonetta 0.1

import "../../common"

CollectionView {
    delegate: Item {
        id: delegateRoot
        height: trackName.height + UI.globalSpacing
        focus: true

        FocusText {
            id: trackName
            text: model ? model.name : ""
            color: delegateRoot.activeFocus ? UI.colors.focusText : UI.colors.text
            font: UI.fonts.standard
            elide: Text.ElideRight
            hasFocus: delegateRoot.activeFocus

            anchors {
                left: parent.left
                right: parent.right
                top: parent.top
                margins: UI.globalSpacing
                topMargin: UI.globalSpacing / 2
            }
        }
    }
}
