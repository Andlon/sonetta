import QtQuick 2.2
import Sonetta 0.1
import "../common"

FocusScope {
    focus: true

    MenuView {
        id: choices

        focus: true
        anchors {
            fill: parent
            topMargin: ui.misc.topSectionHeight
        }

        width: 270

        model: ["Log out", "Exit"]

        onItemPressed: {
            if (currentIndex == 0)
            {
                // Log out
                session.logout()
            }
            else if (currentIndex == 1)
            {
                // Exit
                Qt.quit()
            }
        }
    }
}
