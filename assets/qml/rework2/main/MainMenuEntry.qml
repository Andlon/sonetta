import QtQuick 2.1
import Sonetta 0.1

import "../common"
import "../common/States.js" as States

Item {
    id: root

    property string page
    property bool selected: ui.state.page === page

    height: 150
    anchors {
        left: parent.left
        right: parent.right
    }

    Text {
        anchors {
            centerIn: parent
            margins: ui.misc.globalPadding
        }

        text: States.getLabel(page)
        color: ui.colors.standard
        font: ui.fonts.h4
    }
}
