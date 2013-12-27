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

        model: ListModel {
        }

        delegate: Loader {
            focus: true

            Component.onCompleted: setSource(model.source, model.properties)
        }

        function push(source, properties)
        {
            model.append({ source: source, properties: properties });
            sync()
        }

        function clear()
        {
            if (count > 1)
            {
                model.remove(1, count - 1)
            }
            sync()
        }

        function sync()
        {
            currentIndex = count - 1
        }

        Component.onCompleted: push("tophits.qml", {})
    }

    Connections {
        target: UI

        onTransition: {
            if (current.page === root.page)
            {
                if (type === "push")
                {
                    root.rebuildStack(prev.parameters.browseInstances)
                }
                else if (type === "pop")
                {
                    root.rebuildStack(current.parameters.browseInstances)
                }
                else if (type === "reset")
                {
                    root.rebuildStack(undefined)
                }

                if (current.parameters.type === "album")
                {
                    if (type === "push")
                    {
                        pushAlbum(current.parameters.album)
                    }

                    if (prev.page !== root.page)
                    {
                        stack.positionViewAtEnd()
                    }
                }
            }
        }
    }

    function rebuildStack(instances)
    {
        if (instances)
        {
            var i = 1;
            var j = 0

            while (i < stack.count && j < instances.length
                   && stack.model.get(i).source === instances[j].source)
            {
                ++i
                ++j
            }

            while (j < instances.length)
            {
                while (i < stack.count)
                {
                    stack.model.remove(i)
                }
                var source = instances[j].source
                var properties = instances[j].properties

                stack.push(source, properties)
                ++j
            }

            stack.currentIndex = j
        }
        else
        {
            stack.clear()
        }

        commitInstances(instances)
    }

    function pushAlbum(album)
    {
        var properties = { album: album }
        var source = Qt.resolvedUrl("AlbumBrowse.qml")
        stack.push(source, properties)
        updateInstances(UI.parameters.browseInstances, { source: source, properties: properties })
    }

    function updateInstances(instances, instance)
    {
        if (instances)
        {
            instances.push(instance)
        }
        else
        {
            instances = [ instance ]
        }

        commitInstances(instances)
    }

    function commitInstances(instances)
    {
        var param = UI.parameters
        param.browseInstances = instances
        UI.update(root.page, param)
    }
}
