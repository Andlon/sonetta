import QtQuick 2.0
import com.bitfraction.sonata 0.1

import "time.js" as Time

FocusScope {
    id: root

    focus: true

    property bool playlistActive: false

    SpPlaylistContainerWatcher {
        id: container
        container: spotify.rootContainer
    }

    SpPlaylistWatcher {
        id: playlistWatcher
        playlist: container.playlistAt(0)
    }

    Rectangle {
        anchors.centerIn: parent
        width: root.width * 0.9
        height: root.height * 0.9
        //        color: "#88000000"
        color: "#272727"
        border.width: 3
        border.color: "#7e7e7e"

        FocusScope {
            id: optionsContainer
            width: activeFocus ? 400 : 0
            visible: width > 0
            anchors { top: parent.top; bottom: parent.bottom; right: parent.right; margins: parent.border.width }
            clip: true

            Behavior on width {
                NumberAnimation { }
            }

            Rectangle {
                anchors.fill: parent
                color: Qt.darker(parent.parent.color, 1.2)
            }

            TrackOptions {
                id: trackOptions
                width: 400
                focus: false
                anchors { fill: parent }

                Navigation.onLeft: tracks.forceActiveFocus()
                Component.onCompleted: playlists.forceActiveFocus()
                onTrackEnqueued: tracks.forceActiveFocus()
            }
        }

        FocusScope {
            id: playlistsContainer
            width: activeFocus ? 400 : 0
            visible: width > 0
            anchors { left: parent.left; top: parent.top;
                bottom: parent.bottom; margins: parent.border.width;}

            clip: true

            Rectangle {
                anchors.fill: parent
                color: Qt.darker(parent.parent.color, 1.2)
            }

            Behavior on width {
                NumberAnimation { }
            }

            Text {
                id: playlistsHeader
                font.family: "Roboto"
                font.pointSize: 20
                font.bold: true
                color: "White"
                anchors.top: parent.top
                anchors.topMargin: 20
                anchors.horizontalCenter: parent.horizontalCenter
                text: "Playlists"
                font.capitalization: Font.AllUppercase
            }

            PlaylistView {
                id: playlists
                focus: true
                anchors { bottom: parent.bottom; left: parent.left; right: parent.right;
                    top: playlistsHeader.bottom; topMargin: 20; bottomMargin: 20 }
                model: container.model
                delegate: playlistDelegate

                Navigation.onRight: {
                    if (!playlistWatcher.isValid && currentItem)
                        playlistWatcher.playlist = currentItem.playlist;
                    tracks.forceActiveFocus()
                }
            }

        }

        FocusScope {
            id: tracksContainer
            anchors { left: playlistsContainer.right; top: parent.top;
                bottom: parent.bottom; right: optionsContainer.left; topMargin: 20; bottomMargin: 20 }

            Text {
                id: tracksHeader
                font.family: "Roboto"
                font.pointSize: 20
                font.bold: true
                color: "White"
                anchors.top: parent.top
                anchors.horizontalCenter: parent.horizontalCenter
                text: playlistWatcher.name
                font.capitalization: Font.AllUppercase
            }

            PlaylistView {
                id: tracks
                anchors { bottom: parent.bottom; top: tracksHeader.bottom; left: parent.left; right: parent.right; topMargin: 20 }
                model: playlistWatcher.model
                delegate: trackDelegate

                Navigation.onLeft: playlists.forceActiveFocus()
                Navigation.onRight: trackOptions.forceActiveFocus()

                onCurrentItemChanged: {
                    if (currentItem)
                        trackOptions.track = currentItem.track
                    else
                        trackOptions.track = undefined
                }
            }
        }

        Behavior on width {
            NumberAnimation { duration: 400 }
        }
    }

    Component {
        id: playlistDelegate
        Text {
            property var playlist: model.playlist

            color: spotify.isEqual(playlist, playlistWatcher.playlist) ? "#ffa800" : "#ffffff"
            font.pointSize: 16
            text: model.name
            width: playlists.width - x
            x: 30
            height: 40
            verticalAlignment: Text.AlignVCenter

            Navigation.onOk: {
                playlistWatcher.playlist = playlist;
                tracks.forceActiveFocus()
            }
        }
    }

    Component {
        id: trackDelegate
        Loader {
            id: trackLoader
            width: tracks.width - x
            x: 20
            asynchronous: false

            property var track: model.track
            property bool isLoaded: model.isLoaded ? true : false

            sourceComponent: Item {
//                spacing: 15
                height: childrenRect.height
                width: parent.width
                property color textColor: spotify.isEqual(spotify.currentTrack, trackLoader.track) ? "#ffa800" : "#ffffff"

                Text {
                    color: textColor
                    font.pointSize: 14
                    text: trackLoader.isLoaded ? model.name : "Loading..."
                    height: 40
                    verticalAlignment: Text.AlignVCenter
                    elide: Text.ElideRight
                    anchors.left: parent.left
                    anchors.right: duration.left
                    anchors.leftMargin: 30
                    anchors.rightMargin: 30
                }

                Text {
                    id: duration
                    color: "#afafaf"
                    font.pointSize: 14
                    text: model.duration ? Time.formatTime(model.duration) : "0:00"
                    width: 50
                    height: 40
                    verticalAlignment: Text.AlignVCenter
                    elide: Text.ElideRight
                    anchors.horizontalCenter: parent.horizontalCenter
                }

                Text {
                    color: textColor
                    font.pointSize: 14
                    property string artistNames: trackLoader.isLoaded ? model.artistNames.join(", ") : "Loading..."
                    text: artistNames
                    height: 40
                    verticalAlignment: Text.AlignVCenter
                    elide: Text.ElideRight
                    anchors.left: duration.right
                    anchors.right: parent.right
                    anchors.leftMargin: 30
                    anchors.rightMargin: 30
                }



//                Text {
//                    color: textColor
//                    font.pointSize: 14
//                    property string albumName: model.albumName ? model.albumName : "Loading..."
//                    text: albumName
//                    width: parent.width * 0.3
//                    x: 10
//                    height: 40
//                    verticalAlignment: Text.AlignVCenter
//                    elide: Text.ElideRight
//                }
            }

            Navigation.onOk: {
                spotify.play(model.track)
                sonata.setContext(playlistWatcher.playlist, model.index)
            }

            Keys.onPressed: {
                if (event.key === Qt.Key_Q)
                    sonata.enqueue(track)
            }

            Navigation.onRecord: sonata.enqueue(track)
        }
    }
}
