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
            bottom: parent.bottom
            left: parent.left
            right: parent.right
            top: trackHeader.bottom
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

            delegate: trackDelegate

            Navigation.onDown: incrementCurrentIndex()
            Navigation.onUp: decrementCurrentIndex()
            Navigation.onOk: if (currentItem) spotify.play(currentItem.track)
        }

        style: ScrollViewStyle {
            corner: Item { }
            scrollBarBackground: Item { }
        }
    }

    Image {
        id: trackHeader
        source: "../images/lightDarkTile.png"
        fillMode: Image.Tile
        height: 60
        width: list.width

        anchors {
            top: parent.top
            left: parent.left
        }

        Row {
            anchors {
                verticalCenter: parent.verticalCenter
                left: parent.left
                right: parent.right
                margins: 20
            }

            property font font: { var f = Qt.font(ui.fonts.standard); f.bold = true; return f }

            spacing: 20
            Text {
                font: parent.font
                color: ui.colors.defaultText
                text: "Track"
                width: 500
                elide: Text.ElideRight
            }

            Text {
                font: parent.font
                color: ui.colors.defaultText
                text: "Artist"
                elide: Text.ElideRight
                width: 300
            }

            Text {
                font: parent.font
                color: ui.colors.defaultText
                elide: Text.ElideRight
                width: 400
                text: "Album"
            }
        }
    }

    Component {
        id: trackDelegate

        Item {
            id: track
            property string name: model.name ? model.name : "Loading..."
            property string artists: model.artistNames ? model.artistNames.join(", ") : ""
            property string album: model.albumName ? model.albumName : ""
            property var track: model.track
            width: list.width
            height: 60

            Image {
                visible: model.index % 2 == 0
                source: "../images/leftBarTile.png"
                fillMode: Image.Tile
                cache: true
                asynchronous: true

                // Make the alternating background appear behind the highlight
                anchors.fill: track
                parent: list.contentItem
                z: -1
            }

            Row {
                anchors {
                    verticalCenter: parent.verticalCenter
                    left: parent.left
                    right: parent.right
                    margins: 20
                }

                spacing: 20
                Text {
                    font: ui.fonts.standard
                    color: ui.colors.defaultText
                    text: name
                    width: 500
                    elide: Text.ElideRight
                }

                Text {
                    font: ui.fonts.standard
                    color: ui.colors.defaultText
                    text: artists
                    elide: Text.ElideRight
                    width: 300
                }

                Text {
                    font: ui.fonts.standard
                    color: ui.colors.defaultText
                    elide: Text.ElideRight
                    text: album
                }
            }
        }
    }
}
