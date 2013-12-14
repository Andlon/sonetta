import QtQuick 2.2
import QtQml.Models 2.1
import Sonetta 0.1

import "../common"
import "../common/States.js" as States

FocusScope {
    focus: true
    PageView {
        id: pager
        anchors {
            fill: parent
            topMargin: ui.misc.topSectionHeight
        }

        focus: true
        currentIndex: {
            var state = ui.state
            if (state.playlists.stage === "container")
                return 0
            else if (state.playlists.stage === "playlist")
                return 1

            return 0
        }

        model: ObjectModel {
            ContainerPage {
                id: view
                width: pager.cellWidth
                height: pager.cellHeight
                focus: true
            }

            PlaylistPage {
                width: pager.cellWidth
                height: pager.cellHeight
            }
        }
    }
}
