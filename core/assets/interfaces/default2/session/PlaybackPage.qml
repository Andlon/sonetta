import QtQuick 2.3
import Sonetta 0.1
import Navigation 0.1

import "../common"
import "playback"

FocusScope {
    id: root

    signal albumBrowseRequested(var album)
    signal artistBrowseRequested(var artist)

    TrackInfo {
        id: track
        track: player.track
    }

    Section {
        id: coverSection
        header: "Album Art"
        contentWidth: cover.width
        anchors {
            top: root.top
            left: root.left
            bottom: controlsSection.top
            margins: UI.globalSpacing
        }

        SpotifyImage {
            id: cover
            anchors {
                top: parent.top
                bottom: parent.bottom
            }

            width: height
            fillMode: Image.PreserveAspectFit
            uri: track.largeCoverUri
        }
    }

    Section {
        id: controlsSection
        header: "Controls"
        contentHeight: controls.height
        anchors {
            bottom: root.bottom
            left: root.left
            right: coverSection.right
            margins: UI.globalSpacing
            rightMargin: 0
        }


        PlaybackControls {
            id: controls
            focus: true
            anchors {
                left: parent.left
                right: parent.right
            }

            KeyNavigation.right: queue
        }
    }

    Section {
        id: nowplayingSection
        header: "Now Playing"
        contentHeight: nowplaying.height
        anchors {
            left: coverSection.right
            top: root.top
            right: root.right
            margins: UI.globalSpacing
        }

        CurrentTrackInformation {
            id: nowplaying
            anchors {
                left: parent.left
                right: parent.right
            }

            trackName: track.name
            albumName: track.albumName
            artistName: track.artistNames.join(', ')
            albumYear: track.albumYear
        }
    }

    Section {
        id: queueSection
        header: "Coming Up"
        anchors {
            top: nowplayingSection.bottom
            left: coverSection.right
            right: root.right
            bottom: root.bottom
            margins: UI.globalSpacing
        }

        QueueView {
            id: queue
            anchors.fill: parent
            model: player.queue

            KeyNavigation.left: controls

            onAlbumBrowseRequested: root.albumBrowseRequested(album)
            onArtistBrowseRequested: root.artistBrowseRequested(artist)
        }
    }
}
