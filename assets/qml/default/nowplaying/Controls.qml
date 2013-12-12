import QtQuick 2.2
import QtQml.Models 2.1
import Sonetta 0.1

ListView {
    id: list

    property int controlWidth: 56
    property int controlHeight: 56
    height: controlHeight + ui.misc.globalPadding
    width: childrenRect.width
    orientation: ListView.Horizontal
    boundsBehavior: Flickable.StopAtBounds
    interactive: false

    model: ListModel {
        ListElement {
            // Previous
            playingSource: "../images/playback/next.svg"
            pausedSource: "../images/playback/next.svg"
            rotation: 180
        }

        ListElement {
            // Play/Pause
            playingSource: "../images/playback/pause.svg"
            pausedSource: "../images/playback/play.svg"
            rotation: 0
        }

        ListElement {
            // Next
            playingSource: "../images/playback/next.svg"
            pausedSource: "../images/playback/next.svg"
            rotation: 0
        }
    }

    delegate: FocusScope {
        height: control.height + ListView.view.highlightItem.height + ui.misc.globalPadding / 2
        width: control.width
        Control {
            width: controlWidth
            height: controlHeight
            focus: true
            id: control
            source: player.playing ? model.playingSource : model.pausedSource
            rotation: model.rotation
        }
    }

    highlightMoveDuration: ui.misc.globalAnimationTime
    highlightFollowsCurrentItem: true
    highlight: Rectangle {
        color: ui.colors.highlight
        height: ui.misc.globalPadding / 2
        width: list.currentItem.width
        y: list.currentItem.height - height
    }

    Navigation.onRight: {
        if (currentIndex < count - 1)
        {
            incrementCurrentIndex()
        }
        else
        {
            event.accepted = false
        }
    }

    Navigation.onLeft: {
        if (currentIndex >= 1)
        {
            decrementCurrentIndex()
        }
        else
        {
            event.accepted = false
        }
    }

    Navigation.onOk: {
        if (currentIndex >= 0 && currentIndex < count)
        {
            if (currentIndex == 0)
            {
                // Pressed previous
            }
            else if (currentIndex == 1)
            {
                // Pressed play/pause
                player.playPause()
            }
            else if (currentIndex == 2)
            {
                // Pressed next
                player.next()
            }
        }
    }
}



//    Row {
//        anchors.horizontalCenter: parent.horizontalCenter
//        width: childrenRect.width
//        height: childrenRect.height

//        Control {
//            id: prev
//            width: controlWidth
//            height: controlHeight
//            focus: true

//            source: "../images/playback/next.svg"
//            rotation: 180

//            Navigation.onRight: playPause.focus = true
//        }

//        Control {
//            id: playPause
//            width: controlWidth
//            height: controlHeight
//            source: player.playing ? "../images/playback/pause.svg" : "../images/playback/play.svg"

//            onPressed: player.playPause()
//            Navigation.onRight: next.focus = true
//            Navigation.onLeft: prev.focus = true
//        }

//        Control {
//            id: next
//            width: controlWidth
//            height: controlHeight
//            source: "../images/playback/next.svg"

//            onPressed: player.next()
//            Navigation.onLeft: playPause.focus = true
//        }
//    }
