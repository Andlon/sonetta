import QtQuick 2.2
import Sonetta 0.1
import "../common"
import "../common/Time.js" as Time

FocusScope {
    focus: true
    property alias album: albumModel.album

    SpotifyImage {
        id: cover
        anchors {
            left: parent.left
            right: tracks.left
            top: parent.top

            rightMargin: ui.misc.globalPadding
        }

        fillMode: Image.PreserveAspectFit
        uri: albumModel.largeCoverUri
    }

    H2 {
        id: name

        anchors {
            left: parent.left
            right: tracks.left
            top: cover.bottom
            rightMargin: ui.misc.globalPadding
            topMargin: ui.misc.globalPadding
        }

        horizontalAlignment: Text.AlignHCenter

        text: albumModel.name + " (" + albumModel.year + ")"
    }

    H2 {
        id: artist
        anchors {
            left: parent.left
            right: tracks.left
            top: name.bottom
            rightMargin: ui.misc.globalPadding
        }

        horizontalAlignment: Text.AlignHCenter

        text: albumModel.artistName
    }

    TrackView {
        id: tracks
        focus: true
        model: albumModel
        anchors {
            right: parent.right
            bottom: parent.bottom
            top: parent.top
            left: parent.horizontalCenter
        }

        onTrackPlayed: player.queue.updateContext(albumModel.album, modelIndex)

        delegate: CollectionDelegate {
            id: del
            height: childrenRect.height + ui.misc.globalPadding
            y: ui.misc.globalPadding / 2

            H4 {
                id: albumIndex
                anchors {
                    left: parent.left
                    margins: ui.misc.globalPadding
                }

                text: model.albumIndex + "."
                width: 50
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignRight
            }

            H4 {
                text: model.name
                anchors {
                    left: albumIndex.right
                    right: duration.left
                    margins: ui.misc.globalPadding
                }
                verticalAlignment: Text.AlignVCenter
            }

            H4 {
                id: duration
                anchors {
                    right: parent.right
                    margins: ui.misc.globalPadding
                }
                text: Time.formatTime(model.duration)
                width: contentWidth
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignRight
                elide: Text.ElideNone
            }
        }
    }

    AlbumModel {
        id: albumModel
    }
}
