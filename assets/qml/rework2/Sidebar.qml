import QtQuick 2.1

Item {
    id: root

    Image {
        source: "images/dark.png"
        anchors.fill: parent
        fillMode: Image.Tile
    }

    Image {
        source: "images/sonetta.png"
        anchors {
            top: root.top
            horizontalCenter: root.horizontalCenter
            topMargin: 20
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
