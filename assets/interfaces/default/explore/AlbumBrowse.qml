import QtQuick 2.2
import QtQuick.Layouts 1.1
import Sonetta 0.1
import "../common"
import "../common/Time.js" as Time

FocusScope {
    id: root
    focus: true
    property alias album: albumModel.album

    transitions: Transition {
        ColorAnimation {
            duration: ui.misc.globalAnimationTime
            easing.type: Easing.InOutQuad
        }
    }

    SpotifyImage {
        id: cover
        anchors {
            left: tracks.right
            right: parent.right
            top: parent.top
            bottom: parent.bottom

            leftMargin: ui.misc.globalPadding
        }

        fillMode: Image.PreserveAspectFit
        verticalAlignment: Image.AlignTop
        uri: albumModel.largeCoverUri
    }

    RowLayout {
        id: albumAndYear
        spacing: 0
        anchors
        {
            left: tracks.left
            right: tracks.right
            top: parent.top
            leftMargin: ui.misc.globalPadding
        }

        Text {
            text: albumModel.name
            color: ui.colors.standard
            font: ui.fonts.h2
            clip: true
            elide: Text.ElideRight

            // This is a semi-ugly hack, but it's the only way I managed intended behavior...
            Layout.preferredWidth: implicitWidth + 5
            Layout.maximumWidth: albumAndYear.width - year.contentWidth
        }

        Text {
            id: year
            color: ui.colors.label
            font: ui.fonts.h2
            text: " (" + albumModel.year + ")"
            Layout.minimumWidth: contentWidth
            Layout.fillWidth: true
        }
    }

    RowLayout {
        id: byArtist
        spacing: 0
        anchors {
            left: albumAndYear.left
            right: tracks.right
            top: albumAndYear.bottom
            topMargin: ui.misc.globalPadding / 2
        }

        Text {
            color: ui.colors.label
            font: ui.fonts.h2
            text: "by "
            Layout.minimumWidth: implicitWidth
        }

        Text {
            text: albumModel.artistName
            color: ui.colors.standard
            font: ui.fonts.h2
            elide: Text.ElideRight
            Layout.fillWidth: true
        }
    }

    TrackView {
        id: tracks
        focus: true
        model: albumModel
        width: root.width * 0.5

        anchors {
            bottom: parent.bottom
            top: byArtist.bottom
            left: parent.left

            topMargin: 2 * ui.misc.globalPadding
        }

        onTrackPlayed: player.queue.updateContext(albumModel.album, modelIndex)

        delegate: SingleArtistTrackDelegate {
            index: model ? model.albumIndex : 0
        }
    }

    AlbumModel {
        id: albumModel
    }
}
