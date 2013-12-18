import QtQuick 2.2
import QtQml.Models 2.1

FocusScope {
    width: 400
    height: 400

    readonly property alias count: repeater.count

    Repeater {
        id: repeater
        anchors {
            fill: parent
        }

        model: ListModel { }

        delegate: Loader {
            width: repeater.width
            height: repeater.height

            Component.onCompleted: setSource(model.source, model.properties)
        }
    }

    function push(source, properties)
    {
        var element = {}
        element.source = source
        element.properties = properties
        repeater.model.append(element)
        repeater.itemAt(count - 1).focus = true
    }

    function pop()
    {
        if (count > 0)
            repeater.model.remove(count - 1)
    }

    function clear()
    {
        repeater.model.clear()
    }


}
