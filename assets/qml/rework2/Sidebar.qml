import QtQuick 2.1
import Sonetta 0.1
import "common"

FocusScope {
    id: root

    Image {
        source: "images/dark.png"
        anchors.fill: parent
        fillMode: Image.Tile
    }

    Image {
        id: sonettaLogo
        source: "images/sonetta.png"
        anchors {
            top: root.top
            horizontalCenter: root.horizontalCenter
            topMargin: 20
        }
    }

    Column {
        anchors {
            left: root.left
            right: root.right
            top: sonettaLogo.bottom
            topMargin: ui.misc.globalPadding
        }

        height: childrenRect.height

        Selectable {
            height: 150
            anchors {
                left: parent.left
                right: parent.right
            }

            Text {
                anchors.centerIn: parent
                text: "now playing"
                color: ui.colors.standard
                font: ui.fonts.h4
            }
        }

        Selectable {
            height: 150
            anchors {
                left: parent.left
                right: parent.right
            }

            focus: true

            Text {
                anchors.centerIn: parent
                text: "playlists"
                color: ui.colors.standard
                font: ui.fonts.h4
            }

            selected: true
        }

        Selectable {
            height: 150
            anchors {
                left: parent.left
                right: parent.right
            }

            Text {
                anchors.centerIn: parent
                text: "search"
                color: ui.colors.standard
                font: ui.fonts.h4
            }
        }

        Selectable {
            height: 150
            anchors {
                left: parent.left
                right: parent.right
            }

            Text {
                anchors.centerIn: parent
                text: "discover"
                color: ui.colors.standard
                font: ui.fonts.h4
            }
        }
    }

//    Image {
//        anchors {
//            bottom: root.bottom
//            //left: root.left
//            //right: root.right
//            horizontalCenter: root.horizontalCenter
//            margins: ui.misc.globalPadding
//        }

//        source: "images/spotify.png"
//        //height: width / sourceSize.width * sourceSize.height
//    }
}
