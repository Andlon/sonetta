import QtQuick 2.0
import com.bitfraction.sonata 0.1

Image {
    id: root
    width: 800
    height: 800

    source: "abstract_background.png"

    Login {
        anchors.centerIn: parent

        // Only show when logged out
        visible: opacity !== 0
        opacity: spotify.connectionState === Spotify.LoggedOutState ? 1 : 0

        Behavior on opacity {
            SmoothedAnimation { velocity: -1; duration: 1000; }
        }

        focus: true

        onLoginSubmitted: spotify.login(username, password)
    }


    MainSection {
        anchors.top: root.top
        anchors.bottom: root.bottom
        anchors.left: root.left
        anchors.right: root.right

        // Only show when logged in
        visible: opacity !== 0
        opacity: spotify.connectionState === Spotify.LoggedInState ? 1 : 0

        Behavior on opacity {
            SmoothedAnimation { velocity: -1; duration: 1000; }
        }

        onVisibleChanged: if (visible) focus = true
    }

    Navigation.onNext: sonata.next()
    Navigation.onPrevious: sonata.prev()
    Navigation.onPlay: spotify.play()
    Navigation.onPause: spotify.pause()
    Navigation.onPlayPause: playPause()
    Keys.onSpacePressed: playPause()

    function playPause()
    {
        if (spotify.playbackState === Spotify.PlayState)
            spotify.pause();
        else
            spotify.play();
    }

}
