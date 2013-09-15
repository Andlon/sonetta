import QtQuick 2.1
import Sonetta 0.1

import "../common"

FocusScope
{
    VirtualKeyboardInput {
        anchors.centerIn: parent
        focus: true
    }

    SearchEngine {
        id: engine
    }
}
