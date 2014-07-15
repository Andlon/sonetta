import QtQuick 2.3
import Sonetta 0.1

FocusScope {
    id: root
    width: 300
    height: childrenRect.height

    property alias playlist: mosaic.playlist

    Mosaic {
        id: mosaic
        singleCoverSize: MosaicGenerator.Large
        collageCoverSize: MosaicGenerator.Normal
        height: width
        placeholder: "../../images/disc_placeholder_large.png"

        anchors {
            top: root.top
            left: root.left
            right: root.right
        }
    }
}
