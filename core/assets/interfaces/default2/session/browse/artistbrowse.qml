import QtQuick 2.3
import QtQml.Models 2.1
import Sonetta 0.1
import Navigation 0.1
import "../../common"

FocusScope {
    id: root

    property alias artist: synopsis.artist

    states: [
        State {
            name: "hits"
            when: menu.currentIndex === 0
            PropertyChanges { target: subtitle; text: "Top tracks" }
            //            PropertyChanges { target: tracks; opacity: 1 }
            //            PropertyChanges { target: albums; opacity: 0 }
            //            PropertyChanges { target: artists; opacity: 0 }
        },
        State {
            name: "albums"
            when: menu.currentIndex === 1
            PropertyChanges { target: subtitle; text: "Albums" }
            //            PropertyChanges { target: tracks; opacity: 0 }
            //            PropertyChanges { target: albums; opacity: 1 }
            //            PropertyChanges { target: artists; opacity: 0 }
        },
        State {
            name: "artists"
            when: menu.currentIndex === 2
            PropertyChanges { target: subtitle; text: "Similar artists" }
            //            PropertyChanges { target: tracks; opacity: 0 }
            //            PropertyChanges { target: albums; opacity: 0 }
            //            PropertyChanges { target: artists; opacity: 1 }
        }
    ]

    Section {
        header: "Selection"
        contentHeight: portrait.height
        anchors {
            right: headerSection.left
            left: root.left
            top: root.top
            bottom: menu.top
            rightMargin: UI.globalSpacing
            bottomMargin: UI.globalSpacing
        }

        SpotifyImage {
            id: portrait
            uri: synopsis.largePortraitUri
            anchors.fill: parent
            fillMode: Image.PreserveAspectCrop
            horizontalAlignment: Image.AlignHCenter
            verticalAlignment: Image.AlignVCenter
        }
    }

    Menu {
        id: menu
        focus: true
        anchors {
            left: root.left
            bottom: root.bottom
            right: headerSection.left
            rightMargin: UI.globalSpacing
        }

        MenuTextItem {
            text: "Top tracks"
        }

        MenuTextItem {
            text: "Albums"
        }

        MenuTextItem {
            text: "Similar artists"
        }

        Navigation.onRight: pager.focus = true
    }

    Section {
        id: headerSection
        header: "Artist"
        width: root.width * UI.browse.listFraction
        contentHeight: headerColumn.height
        anchors {
            top: root.top
            right: root.right
        }

        Column {
            id: headerColumn
            height: childrenRect.height
            anchors {
                left: parent.left
                right: parent.right
            }

            Text {
                id: header
                font: UI.browse.artistHeader
                color: UI.colors.text
                text: synopsis.name
                anchors {
                    left: parent.left
                    right: parent.right
                }
            }

            Text {
                id: subtitle
                font: UI.browse.artistHeader
                color: UI.colors.label
                anchors {
                    left: parent.left
                    right: parent.right
                }
            }
        }
    }



    PageView {
        id: pager
        currentIndex: menu.currentIndex
        clip: true
        anchors {
            top: headerSection.bottom
            right: headerSection.right
            left: headerSection.left
            bottom: root.bottom
            topMargin: UI.globalSpacing
        }

        model: ObjectModel {
            TrackView {
                id: tracks
                model: synopsis.hits

                delegate: NumberedDoubleRowTrackDelegate {
                    onHeightChanged: tracks.delegateHeight = height
                }
            }

            AlbumView {
                id: albums
                model: synopsis.albums
            }

            ArtistView {
                id: artists
                model: synopsis.similarArtists
            }
        }

        Navigation.onLeft: menu.focus = true
    }

    ArtistSynopsis {
        id: synopsis
        browseType: ArtistSynopsis.StandardBrowse
    }
}
