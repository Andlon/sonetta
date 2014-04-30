import QtQuick 2.2
import Sonetta 0.1
import "../common"

Page {
    id: root
    page: "explore"

    PageView {
        id: stack
        focus: true
        clip: true
        vertical: false
        anchors {
            topMargin: ui.misc.topSectionHeight
            fill: parent
        }

        model: ListModel {}

        delegate: Loader {
            focus: true
            asynchronous: true

            Component.onCompleted: setSource(model.source, model.properties)
        }

        property int usedCount: 0

        function push(source, properties)
        {
            model.insert(usedCount + 1, { source: source, properties: properties });
            usedCount += 1
            sync()
        }

        function pop()
        {
            if (usedCount > 0)
                usedCount -= 1
            sync()
        }

        function sync()
        {
            currentIndex = usedCount
        }

        function immediateSync()
        {
            sync()
            positionViewAtIndex(currentIndex)
        }

        Component.onCompleted: model.insert(0, { source: "tophits.qml", properties: {} })
    }

    Connections {
        target: UI

        onTransition: {
            if (current.page === root.page)
            {
                if (type === "push")
                {
                    if (current.parameters.type === "album")
                    {
                        pushAlbum(current.parameters.album)
                    }
                    else if (current.parameters.type === "artist")
                    {
                        pushArtist(current.parameters.artist)
                    }
                }
                else if (prev.page === root.page && type === "pop")
                {
                    stack.pop()
                }

                if (prev.page !== root.page)
                {
                    stack.immediateSync()
                }
            }
        }
    }

    function pushAlbum(album)
    {
        var properties = { album: album }
        var source = Qt.resolvedUrl("AlbumBrowse.qml")
        stack.push(source, properties)
    }

    function pushArtist(artist)
    {
        var properties = { artist: artist }
        var source = Qt.resolvedUrl("ArtistBrowse.qml")
        stack.push(source, properties)
    }
}
