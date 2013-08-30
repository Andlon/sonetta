import QtQuick 2.1
import QtQml.Models 2.1
import sonata 0.1
import "../common"

Page {
    id: resultPage

    property alias trackModel: trackView.model
    property alias albumModel: albumView.model
    property alias artistModel: artistView.model
    property alias currentTrackIndex: trackView.currentIndex
    property alias currentTrackItem: trackView.currentItem
    property alias currentAlbumIndex: albumView.currentIndex
    property alias currentAlbumItem: albumView.currentItem
    property alias currentArtistIndex: artistView.currentIndex
    property alias currentArtistItem: artistView.currentItem


    Item {
        id: menuContainer
        anchors.left: parent.left
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.topMargin: ui.misc.verticalContentMargin
        anchors.bottomMargin: ui.misc.verticalContentMargin
        anchors.leftMargin: 20
        width: 300

        TextMenu {
            id: menu
            anchors.fill: parent
            stickySelection: true

            selectionDelegate: Image { source: "../images/leftBarTile.png"; fillMode: Image.Tile }

            Item {
                width: menu.width
                height: 60
                Text {
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.left: parent.left
                    anchors.margins: 20
                    text: "Tracks"
                    font: ui.fonts.standard
                    color: ui.colors.defaultText
                }
            }

            Item {
                width: menu.width
                height: 60
                Text {
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.left: parent.left
                    anchors.margins: 20
                    text: "Artists"
                    font: ui.fonts.standard
                    color: ui.colors.defaultText
                }
            }

            Item {
                width: menu.width
                height: 60
                Text {
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.left: parent.left
                    anchors.margins: 20
                    text: "Albums"
                    font: ui.fonts.standard
                    color: ui.colors.defaultText
                }
            }

            Item {
                width: menu.width
                height: 60
                Text {
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.left: parent.left
                    anchors.margins: 20
                    text: "Playlists"
                    font: ui.fonts.standard
                    color: ui.colors.defaultText
                }
            }
        }

        Navigation.onRight: resultView.forceActiveFocus()
    }

    PageView {
        id: resultView
        focus: true
        clip: true
        currentIndex: menu.selectedIndex < count ? menu.selectedIndex : count - 1

        Navigation.onLeft: menu.forceActiveFocus()

        anchors {
            right: parent.right
            left: menuContainer.right
            top: parent.top
            bottom: parent.bottom
            topMargin: ui.misc.verticalContentMargin
            bottomMargin: ui.misc.verticalContentMargin
            leftMargin: 20
            rightMargin: 20
        }

        Page {
            id: trackResults
            TrackCollectionView {
                id: trackView
                anchors.fill: parent
                focus: true
                clip: true
            }
        }

        Page {
            id: artistResults
            ArtistCollectionView {
                id: artistView
                anchors.fill: parent
                focus: true
                clip: true
            }
        }

        Page {
            id: albumResults
            AlbumCollectionView {
                id: albumView
                anchors.fill: parent
                clip: true
            }
        }

        onActiveFocusChanged: {
            if (activeFocus)
            {
                if (currentIndex == 0)
                    trackView.forceActiveFocus()
                else if (currentIndex == 1)
                    artistResults.forceActiveFocus()
                else if (currentIndex == 2)
                    albumView.forceActiveFocus()
            }
        }

    }

    function resetIndices()
    {
        currentTrackIndex = 0
        currentAlbumIndex = 0
    }
}
