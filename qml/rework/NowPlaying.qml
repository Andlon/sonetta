import QtQuick 2.1
import sonata 0.1
import "common/Time.js" as Time

Item {
    visible: trackInfo.isLoaded
    TrackInfo {
        id: trackInfo
        track: spotify.currentTrack

        onDataUpdated: {
            var url = trackInfo.albumCoverUri(Spotify.ImageSizeNormal)
            console.log(url)
            cover.source = "image://sp/" + url
        }
    }

    Row {
        spacing: 20
        Image {
            id: cover
            width: 140
            height: 140
        }

        Column {
            spacing: 5
            anchors.verticalCenter: cover.verticalCenter
            Text {
                font: ui.fonts.h2
                color: ui.colors.defaultText
                text: trackInfo.name + " (" + Time.formatTime(spotify.position) + " / " + Time.formatTime(trackInfo.duration) + ")"
            }

            Text {
                font: ui.fonts.h3
                color: ui.colors.defaultText
                text: trackInfo.artistNames.join(", ")
            }
        }
    }


}
