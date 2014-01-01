import QtQuick 2.2
import QtQml.Models 2.1
import QtQuick.Layouts 1.1
import Sonetta 0.1

import "../common"

FocusScope {
    id: root
    focus: true

    property alias artist: synopsis.artist

    ArtistSynopsis {
        id: synopsis
        browseType: ArtistSynopsis.StandardBrowse
    }

    SpotifyImage {
        id: portrait

        anchors {
            left: menu.left
            top: root.top
            right: menu.right
        }

        height: {
            var maxHeight = root.height - menu.height - ui.misc.globalPadding
            var preferredHeight = (width / sourceSize.width) * sourceSize.height
            return Math.min(maxHeight, preferredHeight)
        }

        uri: synopsis.largePortraitUri
        fillMode: Image.PreserveAspectFit
    }

    Menu {
        id: menu
        focus: true
        width: root.width / 3

        anchors {
            left: root.left
            top: portrait.bottom
            topMargin: ui.misc.globalPadding
        }

        MenuAction { text: "Top hits" }
        MenuAction { text: "Albums" }
        MenuAction { text: "Similar artists" }
        MenuAction { text: "Biography" }

        KeyNavigation.right: pages
    }

    Text {
        id: artistName
        anchors {
            top: parent.top
            left: pages.left
            right: pages.right

            leftMargin: ui.misc.globalPadding
            rightMargin: ui.misc.globalPadding
        }

        text: synopsis.name
        font: ui.fonts.h2
        color: ui.colors.standard
    }

    Text {
        id: label
        anchors {
            top: artistName.bottom
            left: artistName.left
            right: artistName.right
        }

        property string label

        font: ui.fonts.h2
        color: ui.colors.label

        states: [
            State {
                name: "hits"
                when: pages.currentIndex == 0
                PropertyChanges { target: label; label: "Top hits" }
            },
            State {
                name: "albums"
                when: pages.currentIndex == 1
                PropertyChanges { target: label; label: "Albums" }
            },
            State {
                name: "similars"
                when: pages.currentIndex == 2
                PropertyChanges { target: label; label: "Similar artists" }
            },
            State {
                name: "biography"
                when: pages.currentIndex == 3
                PropertyChanges { target: label; label: "Biography" }
            }
        ]

        transitions: Transition {
            SequentialAnimation {
                alwaysRunToEnd: true
                SmoothedAnimation {
                    target: label
                    property: "opacity"
                    to: 0
                    duration: ui.misc.globalAnimationTime
                    easing.type: Easing.InOutQuad
                    velocity: -1
                }
                ScriptAction { script: label.text = label.label }
                SmoothedAnimation {
                    target: label
                    property: "opacity"
                    to: 1
                    duration: ui.misc.globalAnimationTime
                    easing.type: Easing.InOutQuad
                    velocity: -1
                }
            }
        }

        Component.onCompleted: label.text = label.label
    }

    PageView {
        id: pages
        clip: true
        anchors {
            top: label.bottom
            right: parent.right
            left: menu.right
            bottom: parent.bottom
            leftMargin: ui.misc.globalPadding
            topMargin: 2 * ui.misc.globalPadding
        }

        currentIndex: menu.currentIndex

        model: ObjectModel {
            TrackView {
                id: topHits
                model: synopsis.hits
            }

            AlbumView {
                id: albums
                model: synopsis.albums
            }

            ArtistView {
                id: similarArtists
                model: synopsis.similarArtists
            }

            Text {
                id: biography
                font: ui.fonts.h4
                color: ui.colors.standard
                linkColor: color
                text: synopsis.biography.replace(/<(?:.|\n)*?>/gm, '')
                wrapMode: Text.Wrap
                textFormat: Text.PlainText
            }
        }

        KeyNavigation.left: menu
    }
}
