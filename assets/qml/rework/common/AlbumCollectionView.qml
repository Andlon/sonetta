import QtQuick 2.1
import QtQuick.Layouts 1.0
import QtQuick.Controls 1.0
import QtQuick.Controls.Styles 1.0

import sonata 0.1

FocusScope {
    id: root

    property alias model: list.model
    property alias cacheBuffer: list.cacheBuffer
    property alias currentIndex: list.currentIndex
    property alias currentItem: list.currentItem

    onActiveFocusChanged: if (activeFocus) list.forceActiveFocus()

    ScrollView {
        anchors {
            fill: parent
        }

        ListView {
            id: list

            anchors.fill: parent
            boundsBehavior: Flickable.StopAtBounds
            highlight: CollectionViewHighlight { }
            highlightFollowsCurrentItem: true
            highlightRangeMode: ListView.ApplyRange
            highlightMoveDuration: 150
            preferredHighlightBegin: height / 4
            preferredHighlightEnd: 3 * height / 4

            focus: true
            currentIndex: 0

            delegate: albumDelegate

            Navigation.onDown: incrementCurrentIndex()
            Navigation.onUp: decrementCurrentIndex()
        }

        style: ScrollViewStyle {
            corner: Item { }
            scrollBarBackground: Item { }
        }
    }

    Component {
        id: albumDelegate

        Item {
            id: album
            width: list.width
            height: 120

            property string cover: model.normalCoverUri ? model.normalCoverUri : ""
            property string name: model.name ? model.name : ""
            property string artist: model.artistName ? model.artistName : ""
            property int year: model.year ? model.year : 0

            Image {
                visible: model.index % 2 == 0
                source: "../images/leftBarTile.png"
                fillMode: Image.Tile
                cache: true
                asynchronous: true

                // Make the alternating background appear behind the highlight
                anchors.fill: album
                parent: list.contentItem
                z: -1
            }

            Image {
                id: coverImage
                anchors {
                    left: album.left
                    margins: 10
                    verticalCenter: parent.verticalCenter
                }
                source: cover ? "image://sp/" + cover : ""
                height: 100
                width: 100
            }

            Column {
                anchors {
                    left: coverImage.right
                    right: parent.right
                    verticalCenter: parent.verticalCenter
                    margins: 20
                }
                height: childrenRect.height

                Text {
                    font: ui.fonts.h3
                    color: ui.colors.defaultText
                    text: name + " (" + year + ")"
                }

                Text {
                    font: ui.fonts.standard
                    color: ui.colors.defaultText
                    text: artist
                }
            }
        }
    }
}
