import QtQuick 2.2
import QtQml.Models 2.1
import Sonetta 0.1
import Navigation 0.1

import "../common"

FocusScope {
    id: root

    function reset()
    {
        selection.currentIndex = 0
    }

    onActiveFocusChanged: {
        if (activeFocus)
        {
            selection.focus = true
        }
    }

    BackIndicator {
        id: backIndicator
        anchors {
            top: root.top
            topMargin: ui.misc.globalPadding
            left: root.left
        }

        width: 60
        rotation: 180
    }

    MenuView {
        id: selection

        focus: true
        anchors {
            left: backIndicator.right
            leftMargin: ui.misc.globalPadding
            bottom: root.bottom
            top: root.top
        }

        width: 270

        model: ["Tracks", "Artists", "Albums"]

        Keys.forwardTo:  Nav { onRight: pager.focus = true }
    }

    PageView {
        id: pager

        anchors {
            left: selection.right
            top: root.top
            right: root.right
            bottom: root.bottom
            leftMargin: ui.misc.globalPadding
        }

        currentIndex: selection.currentIndex

        model: ObjectModel {
            TrackView {
                id: tracks
                focus: true
                model: search.tracks

                onTrackPlayed: player.queue.updateContext(search.tracks.list, modelIndex)

                Connections {
                    target: search

                    onQueryChanged: tracks.currentIndex = 0
                }
            }

            ArtistView {
                id: artists
                focus: true
                model: search.artists

                Connections {
                    target: search

                    onQueryChanged: artists.currentIndex = 0
                }
            }

            AlbumView {
                id: albums
                focus: true
                model: search.albums

                Connections {
                    target: search

                    onQueryChanged: albums.currentIndex = 0
                }
            }
        }

        Keys.forwardTo: Nav { onLeft: selection.focus = true }
    }

    Keys.forwardTo: Nav { onLeft: UI.pop() }
}
