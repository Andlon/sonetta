import QtQuick 2.1
import QtQuick.Controls 1.0
import QtQuick.Controls.Styles 1.0

import sonata 0.1

FocusScope {
    id: root

    property alias model: grid.model
    property alias cacheBuffer: grid.cacheBuffer
    property alias currentIndex: grid.currentIndex
    property alias currentItem: grid.currentItem

    onActiveFocusChanged: if (activeFocus) grid.forceActiveFocus()

    ScrollView {
        anchors.fill: parent
        GridView {
            id: grid
            anchors.fill: parent
            boundsBehavior: Flickable.StopAtBounds
            highlight: CollectionViewHighlight { }
            highlightFollowsCurrentItem: true
            highlightMoveDuration: 150
            cellWidth: 400
            cellHeight: 290
            focus: true
            currentIndex: 0

            delegate: artistDelegate

            Navigation.onDown: moveCurrentIndexDown()
            Navigation.onUp: moveCurrentIndexUp()
            Navigation.onLeft: moveCurrentIndexLeft()
            Navigation.onRight: moveCurrentIndexRight()
        }

        style: ScrollViewStyle {
            corner: Item { }
            scrollBarBackground: Item { }
        }
    }

    Component {
        id: artistDelegate

        Item {
            id: artist
            width: grid.cellWidth
            height: grid.cellHeight

            property string artistName: model.name ? model.name : ""
            property string portrait: model.largePortraitUri ? model.largePortraitUri : ""

            Image {
                visible: model.index % 2 == 0
                source: "../images/leftBarTile.png"
                fillMode: Image.Tile
                cache: true
                asynchronous: true

                // Make the alternating background appear behind the highlight
                anchors.fill: artist
                parent: grid.contentItem
                z: -1
            }

            Image {
                id: portraitImage
                anchors {
                    left: parent.left
                    right: parent.right
                    top: artistNameText.bottom
                    bottom: parent.bottom
                    margins: 20
                }

                fillMode: Image.PreserveAspectCrop

                width: 250
                height: 250
                source: "image://sp/" + portrait
            }

            Text {
                id: artistNameText
                anchors {
                    top: parent.top
                    horizontalCenter: parent.horizontalCenter
                    margins: 20
                }

                font: ui.fonts.h3
                color: ui.colors.defaultText
                text: artistName
                elide: Text.ElideRight
            }
        }
    }
}
