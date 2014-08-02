import QtQuick 2.3
import QtQml.Models 2.1
import Navigation 0.1
import Sonetta 0.1

import "../common"
import "explore"

FocusScope {
    id: root

    readonly property int topMargin: UI.pageTopMargin + UI.globalSpacing
    readonly property int indentation: 3 * UI.globalSpacing
    readonly property string childPattern: "dark"
    readonly property color childBorderColor: UI.colors.light

    signal artistBrowseRequested(var artist)
    signal albumBrowseRequested(var album)

    states: [
        State {
            name: "history"
            when: searchMenu.currentIndex == 0
            PropertyChanges { target: pager; currentIndex: 0 }
        },
        State {
            name: "search"
            when: searchMenu.currentIndex == 1
            PropertyChanges { target: pager; currentIndex: 1 }
        },
        State {
            name: "tracks"
            when: searchMenu.currentIndex == 2
            PropertyChanges { target: pager; currentIndex: 2 }
        },
        State {
            name: "artists"
            when: searchMenu.currentIndex == 3
            PropertyChanges { target: pager; currentIndex: 3 }
        },
        State {
            name: "albums"
            when: searchMenu.currentIndex == 4
            PropertyChanges { target: pager; currentIndex: 4 }
        }
    ]

    FramelessSection {
        id: searchMenuSection

        anchors {
            left: parent.left
            top: parent.top
            topMargin: root.topMargin
            margins: UI.globalSpacing
        }
        contentHeight: searchMenu.height
        contentWidth: searchMenu.width
        header: "Search"

        Menu {
            id: searchMenu
            focus: true
            pattern: "dark"

            MenuInputItem {
                id: query
                label: "Query"

                onInputChanged: performSearch(input)
            }

            MenuChildTextItem {
                id: tracks
                text: "Tracks"
                pattern: root.childPattern
                border.color: root.childBorderColor
            }

            MenuChildTextItem {
                id: artists
                text: "Artists"
                pattern: root.childPattern
                border.color: root.childBorderColor
            }

            MenuChildTextItem {
                id: albums
                text: "Albums"
                pattern: root.childPattern
                border.color: root.childBorderColor
                lastChild: true
            }

            Navigation.onDown: {
                currentIndex = -1
                topLists.currentIndex = 0
                topLists.focus = true
            }
        }

        KeyNavigation.right: pager
    }

    FramelessSection {
        id: topListsSection

        anchors {
            top: searchMenuSection.bottom
            left: root.left
            margins: UI.globalSpacing
        }

        contentHeight: topLists.height
        contentWidth: topLists.width
        header: "Top Lists"

        Menu {
            id: topLists
            focus: true
            currentIndex: -1
            MenuTextItem {
                text: "Top Tracks"
            }

            MenuTextItem {
                text: "Top Albums"
            }

            MenuTextItem {
                text: "Top Artists"
            }

            Navigation.onUp: {
                searchMenu.currentIndex = searchMenu.count - 1
                currentIndex = -1
                searchMenu.focus = true
            }
        }

        KeyNavigation.right: pager
    }

    PageView {
        id: pager
        anchors {
            left: searchMenuSection.right
            top: root.top
            right: root.right
            bottom: root.bottom
            margins: UI.globalSpacing
            topMargin: root.topMargin
        }

        clip: true
        model: ObjectModel {
            SearchHistory {
                id: searchHistory

                focus: true
                width: pager.width
                height: pager.height
                model: search.history

                onQueryRequested: performSearch(query)
            }

            TrackResults {
                id: trackResults

                focus: true
                width: pager.width
                height: pager.height
                model: search.tracks

                onAlbumBrowseRequested: root.albumBrowseRequested(album)
                onArtistBrowseRequested: root.artistBrowseRequested(artist)
            }

            ArtistResults {
                id: artistResults

                focus: true
                width: pager.width
                height: pager.height
                model: search.artists

                onBrowseRequested: root.artistBrowseRequested(artist)
            }

            AlbumResults {
                id: albumResults

                focus: true
                width: pager.width
                height: pager.height
                model: search.albums

                onBrowseRequested: root.albumBrowseRequested(album)
            }
        }

        KeyNavigation.left: searchMenu.currentIndex >= 0 ? searchMenu : topLists
    }

    function performSearch(query) {
        search.go(query)
        searchMenu.currentIndex = 1
        searchMenu.focus = true
    }
}
