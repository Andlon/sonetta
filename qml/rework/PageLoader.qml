import QtQuick 2.0
import QtQml.Models 2.1

GridView {
    id: root

    property int animationDuration: 600

    cellHeight: root.height
    cellWidth: root.width

    interactive: false

    model: pageUrls
    delegate: delegate

    ListModel {
        id: pageUrls
    }

    property string direction

    function load(url)
    {
        var properties = {}
        properties.url = url

        pageUrls.clear()
        pageUrls.append(properties)
    }

    Component {
        id: delegate
        Loader {
            width: root.width
            height: root.height
            source: model.url

            x: {
                if (root.direction === "right")
                    return root.width
                else if (root.direction === "left")
                    return -root.width
                else
                    return 0;
            }
        }
    }

    Transition {
        id: rightInTransition
        SmoothedAnimation { property: "x"; to: 0; duration: animationDuration; velocity: -1 }
    }

    Transition {
        id: rightOutTransition
        SmoothedAnimation { property: "x"; to: -root.width; duration: animationDuration; velocity: -1 }
    }

    Transition {
        id: leftInTransition
        SmoothedAnimation { property: "x"; to: 0; duration: animationDuration; velocity: -1 }
    }

    Transition {
        id: leftOutTransition
        SmoothedAnimation { property: "x"; to: root.width; duration: animationDuration; velocity: -1 }
    }

    onDirectionChanged:
    {
        if (direction === "right")
        {
            add = rightInTransition
            remove = rightOutTransition
        }
        else if (direction === "left")
        {
            add = leftInTransition
            remove = leftOutTransition
        }
        else if (direction === "")
        {
            add = undefined
            remove = undefined
        }
    }
}
