import QtQuick 2.0
import QtGraphicalEffects 1.0
import sonata 0.1

import "States.js" as States

FocusScope {
    id: root

    ArtistSynopsis {
        id: synopsis

        onBrowseDataChanged: {
            var source = portrait(Spotify.ImageSizeNormal);
            if (source)
                artistPortrait.source = "image://sp/" + source

            albumList.albumList = synopsis.albums;
        }
    }

    AlbumListModel {
        id: albumList
    }

    Rectangle {
        id: leftBackground
        color: ui.colors.background
        border.width: 2
        border.color: ui.colors.border
        radius: 25

        anchors.top: root.top
        anchors.bottom: root.bottom
        //        anchors.horizontalCenter: root.horizontalCenter
        anchors.left: root.left
        anchors.right: root.right
        anchors.margins: 100

        Image {
            id: artistPortrait
            anchors.left: parent.left
            anchors.top: artistHeader.bottom
            anchors.margins: 30

            height: 100
            width: sourceSize.height != 0 ? (sourceSize.width / sourceSize.height) * height : 0

            smooth: true
            antialiasing: true
        }

        Text {
            id: artistHeader
            anchors.top: parent.top
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.margins: 30

            color: ui.colors.headerText
            text: synopsis.name

            font.family: ui.fonts.headerFamily
            font.pointSize: ui.fonts.headerSize
        }

        Text {
            anchors.top: artistHeader.bottom
            anchors.left: artistPortrait.right
            anchors.bottom: artistPortrait.bottom
            anchors.right: parent.right
            anchors.margins: 30
            anchors.bottomMargin: 0

            font.family: ui.fonts.defaultFamily
            font.pointSize: ui.fonts.defaultSize
            color: ui.colors.text
            text: synopsis.biography

            elide: Text.ElideRight
            wrapMode: Text.WrapAtWordBoundaryOrAnywhere
            linkColor: color
        }

        Rectangle {
            id: divisor
            anchors.top: artistPortrait.bottom
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.margins: 20

            color: ui.colors.border
            height: 2
        }

        GridView {
            anchors.top: divisor.bottom
            anchors.bottom: parent.bottom
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.margins: 20
            width: determineWidth()

            cellHeight: 300
            cellWidth: 300

            model: filteredAlbumList
            clip: true
            boundsBehavior: Flickable.StopAtBounds
            cacheBuffer: 1000
            delegate: albumDelegate

            function determineWidth() {
                // Find optimum width
                var available = parent.width - 2 * anchors.margins;
                var attempt = 0;
                var desiredWidth;

                while (attempt <= available)
                {
                    desiredWidth = attempt;
                    attempt += cellWidth;
                }

                return desiredWidth;
            }
        }
    }

    VisualDataModel {
        id: filteredAlbumList
        model: albumList
        filterOnGroup: "available"

        groups: [
            VisualDataGroup { name: "available" }
        ]

        items.includeByDefault: true
        items.onChanged: {
            var insertedSeries = inserted.length;

            for (var i = 0; i < insertedSeries; ++i)
            {
                var insertedCount = inserted[i].count;
                var begin = inserted[i].index;
                var last = begin + insertedCount;

                for (var j = begin; j < last; ++j)
                {
                    if (items.get(j).model.isAvailable)
                        items.addGroups(j, 1, "available");
                }
            }

        }

        delegate: albumDelegate
    }

    Component.onCompleted: {
        var state = ui.state;
        synopsis.artistBrowse = spotify.browse(state.artist)
    }

    Component {
        id: albumDelegate
        Item {
            width: GridView.view.cellWidth - 10
            height: GridView.view.cellHeight - 10

            Image {
                id: cover
                anchors.fill: parent

                source: "image://sp/" + model.normalCoverUri
                visible: true

                sourceSize.width: cover.width
                sourceSize.height: cover.height
                asynchronous: true

                Rectangle {
                    anchors.bottom: cover.bottom
                    anchors.left: cover.left
                    anchors.right: cover.right
                    color: "#dd000000"

                    height: childrenRect.height + 10

                    Column {
                        anchors.left: parent.left
                        anchors.right: parent.right
                        y: 5
                        anchors.margins: 5
                        spacing: 2

                        Text {
                            id: title

                            color: ui.colors.headerText
                            font.family: ui.fonts.defaultFamily
                            font.pointSize: ui.fonts.defaultSize
                            anchors.left: parent.left
                            anchors.right: parent.right
                            elide: Text.ElideRight

                            text: model.name
                        }

                        Text {
                            id: year

                            color: ui.colors.headerText
                            font.family: ui.fonts.defaultFamily
                            font.pointSize: ui.fonts.defaultSize - 5
                            anchors.left: parent.left
                            anchors.right: parent.right
                            elide: Text.ElideRight

                            text: model.year
                        }
                    }
                }
            }

            MouseArea {
                anchors.fill: parent
                onClicked: parent.browseArtist()
            }

            Keys.onReturnPressed: browseArtist()

            function browseArtist()
            {
                ui.setState(States.createAlbumBrowse(model.album));
            }
        }
    }


}
