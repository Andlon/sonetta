import QtQuick 2.0
import sonata 0.1
import QtGraphicalEffects 1.0
import "States.js" as States

FocusScope {
    id: root

    property bool showPlaylist: false

    onShowPlaylistChanged: {
        if (showPlaylist)
            playlistView.forceActiveFocus()
        else
            containerView.forceActiveFocus()
    }

    PlaylistContainerModel {
        id: container
        container: spotify.rootContainer
    }

    PlaylistModel {
        id: playlist
    }

    TrackInfo {
        id: trackInfo
    }

    MediaView {
        id: containerView
        visible: !root.showPlaylist

        showMenu: false

        anchors.bottom: root.bottom
        anchors.top: root.top
        anchors.horizontalCenter: root.horizontalCenter
        anchors.margins: 100
        width: 700

        listDelegate: playlistDelegate
        listHighlight: playlistHighlight
        listBackgroundColor: Qt.lighter(ui.colors.background, 1.3)
        model: container

        focus: true

        listHeader: "Playlists"
        menuHeader: "Details"

        Keys.onReturnPressed: {
            playlist.playlist = listCurrentItem.playlist
            showPlaylist = true
        }
    }

    MediaView {
        id: playlistView
        visible: root.showPlaylist

        anchors.fill: root
        anchors.margins: 100

        listDelegate: trackDelegate
        listHighlight: trackHighlight
        listBackgroundColor: Qt.lighter(ui.colors.background, 1.3)
        model: playlist

        listHeader: playlist.name
        menuHeader: "Track details"
        menuDelegate: trackMenu

        onListCurrentIndexChanged: {
            if (listCurrentItem)
                trackInfo.track = listCurrentItem.track
        }

        Keys.onReturnPressed: {
            spotify.play(listCurrentItem.track)
            player.setContext(playlist.playlist, listCurrentIndex)
        }

        Keys.onPressed:
        {
            if (event.key === Qt.Key_Backspace)
            {
                root.showPlaylist = false;
                event.accepted = true
            }
        }
    }

    Component {
        id: playlistHighlight

        Rectangle {
            color: ui.colors.headerText
            width: containerView.listWidth
            radius: 10
        }
    }

    Component {
        id: trackHighlight

        Rectangle {
            color: ui.colors.headerText
            width: playlistView.listWidth
            radius: 10
        }
    }

    Component {
        id: playlistDelegate

        Item {
            height: 40
            width: containerView.listWidth

            property var playlist: model.playlist

            Text {
                color: ui.colors.text
                //                color: ui.colors.headerText
                font.pointSize: ui.fonts.defaultSize
                font.family: ui.fonts.defaultFamily
                anchors.verticalCenter: parent.verticalCenter
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.margins: 10
                anchors.leftMargin: 20
                anchors.rightMargin: 20

                text: model.name
            }
        }
    }

    Component {
        id: trackDelegate

        Item {
            height: 40
            width: playlistView.listWidth

            property var track: model.track

            Loader
            {
                anchors.fill: parent
                sourceComponent: Item {
                    Text {
                        id: name
                        font.family: ui.fonts.defaultFamily
                        font.pointSize: ui.fonts.defaultSize
                        anchors.verticalCenter: parent.verticalCenter
                        anchors.left: parent.left
                        anchors.margins: 10
                        anchors.leftMargin: 20
                        color: ui.colors.text
                        //                color: ui.colors.headerText

                        text: model.name
                        width: 500

                        elide: Text.ElideRight
                    }

                    Text {
                        id: artists
                        font.family: ui.fonts.defaultFamily
                        font.pointSize: ui.fonts.defaultSize
                        anchors.verticalCenter: parent.verticalCenter
                        anchors.right: parent.right
                        anchors.left: name.right
                        anchors.rightMargin: 20
                        color: ui.colors.text
                        //                color: ui.colors.headerText

                        text: model.artistNames.join(", ")
                        width: 300

                        elide: Text.ElideRight
                    }
                }
            }
        }
    }

    Component {
        id: trackMenu

        Item {

        Button {
            text: "Browse artist"
            anchors.top: parent.top
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.margins: 20

            onClicked: {
                var item = playlistView.listCurrentItem
                if (item && item)
                    ui.setState(States.createArtistBrowse(trackInfo.artistAt(0)))
            }
        }

        }
    }
}
