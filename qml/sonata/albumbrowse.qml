import QtQuick 2.0
import sonata 0.1
import "States.js" as States

FocusScope {
    id: root

    property alias albumBrowse: albumModel.albumBrowse

    AlbumBrowseModel {
        id: albumModel
    }

    Rectangle {
        id: background
        color: ui.colors.background
        border.width: 2
        border.color: ui.colors.border
        radius: 25

        anchors.fill: parent
        anchors.margins: 30
    }

    ListView {
        id: trackView
        anchors.fill: background
        anchors.margins: 30
        delegate: trackDelegate
        model: albumModel
        currentIndex: 0
        highlight: trackHighlight


        boundsBehavior: Flickable.StopAtBounds
        focus: true
        clip: true
    }

    Component.onCompleted: {
        var state = ui.state;
        root.albumBrowse = spotify.browse(state.album)

        trackView.forceActiveFocus()
    }

    Component {
        id: trackHighlight

        Rectangle {
            color: ui.colors.headerText
            width: trackView.width
            height: 40
            radius: 10
        }
    }

    Component {
        id: trackDelegate

        Item {
            height: 40
            width: trackView.width

            property var track: model.track

            Loader {
                anchors.fill: parent
                sourceComponent: Item {
                    Text {
                        id: name
                        font.family: ui.fonts.defaultFamily
                        font.pointSize: ui.fonts.defaultSize
                        anchors.verticalCenter: parent.verticalCenter
                        anchors.left: parent.left
                        anchors.margins: 10
                        anchors.leftMargin: 20
                        color: ui.colors.text
                        //                color: ui.colors.headerText

                        text: model.name
                        width: 500

                        elide: Text.ElideRight
                    }

                    Text {
                        id: artists
                        font.family: ui.fonts.defaultFamily
                        font.pointSize: ui.fonts.defaultSize
                        anchors.verticalCenter: parent.verticalCenter
                        anchors.right: parent.right
                        anchors.left: name.right
                        anchors.rightMargin: 20
                        color: ui.colors.text

                        text: model.artistNames.join(", ")
                        width: 300

                        elide: Text.ElideRight
                    }
                }
            }

            Keys.onReturnPressed:
            {
                spotify.play(track)
                player.setContext(root.albumBrowse, model.index)
            }
        }
    }

}
