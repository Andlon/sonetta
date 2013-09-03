import QtQuick 2.1
import Sonetta 0.1

Item {
    id: root

    Sidebar {
        width: 170

        anchors {
            left: root.left
            bottom: root.bottom
            top: root.top
        }
    }

    PlaylistContainerModel {

    }

}
