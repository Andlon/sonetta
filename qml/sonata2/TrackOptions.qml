import QtQuick 2.0
import com.bitfraction.sonata 0.1

FocusScope {
    id: root
    property var track
    clip: true

    signal trackEnqueued

    SpTrackInfo {
        id: trackInfo
        track: root.track ? root.track : undefined
    }    

    /* For Artist browsing testing */

    SpTrackWatcher {
        id: trackWatcher
        track: root.track
        onArtistsChanged: browseWatcher.artistBrowse = spotify.browse(artistAt(0))
    }

    SpArtistBrowseWatcher {
        id: browseWatcher

        onLoaded: console.log("Loaded! Biography: " + biography)
    }

    /* End of artist browsing testing */

    Text {
        id: detailsLabel
        font.family: "Roboto"
        font.pointSize: 20
        font.bold: true
        color: "White"
        text: "Track details"
        font.capitalization: Font.AllUppercase
        anchors.top: root.top
        anchors.topMargin: 30
        anchors.horizontalCenter: root.horizontalCenter
    }

    Grid {
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: detailsLabel.bottom
        anchors.topMargin: 20
        anchors.margins: 30
        clip: true
        columns: 2

        Text {
            font.family: "Roboto"
            font.pointSize: 16
            color: "White"
            text: "Title: "
            elide: Text.ElideRight
        }

        Text {
            font.family: "Roboto"
            font.pointSize: 16
            color: "White"
            text: trackInfo.isValid ? trackInfo.name : "N/A"
            elide: Text.ElideRight
        }

        Text {
            font.family: "Roboto"
            font.pointSize: 16
            color: "White"
            text: "Artist: "
            elide: Text.ElideRight
        }

        Text {
            font.family: "Roboto"
            font.pointSize: 16
            color: "White"
            text: trackInfo.isValid ? trackInfo.artistNames.join(", ") : "N/A"
            elide: Text.ElideRight
        }
    }

    Text {
        id: optionsLabel
        font.family: "Roboto"
        font.pointSize: 16
        font.bold: true
        color: "White"
        text: "Track options"
        font.capitalization: Font.AllUppercase
        anchors.bottom: options.top
        anchors.bottomMargin: 10
        anchors.horizontalCenter: root.horizontalCenter
    }

    NavColumn {
        id: options
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottomMargin: 50
        focus: true

        OptionEntry {
            label: "Queue track"
            onClicked: { sonata.enqueue(trackInfo.track); root.trackEnqueued() }
        }

        OptionEntry {
            label: "Browse album"
        }

        OptionEntry {
            label: "Browse artist"
        }

        onActiveFocusChanged: if (activeFocus) focusIndex = 0
    }
}
