import QtQuick 2.3
import QtQml.Models 2.1
import Navigation 0.1

import ".."

FocusScope {
    id: root
    // This works, but as it's not documented it MAY break in the future
    // ... but let's hope not
    default property alias menuItems: objects.children
    property bool navigationEnabled: true

    property alias currentIndex: view.currentIndex
    property alias currentItem: view.currentItem
    property alias count: view.count

    property string pattern: "dark"
    property int indentation: UI.menu.indentation

    height: view.contentItem.childrenRect.height
    width: UI.menu.defaultWidth

    ListView {
        id: view
        focus: true

        anchors.fill: root
        highlightMoveDuration: UI.timing.highlightMove
        currentIndex: 0
        highlight: CollectionHighlight {
            list: view
            indentation: view.currentItem && view.currentItem.indentation ? view.currentItem.indentation : 0
        }

        model: ObjectModel { id: objects }

        Navigation.onDown: {
            if (navigationEnabled && root.currentIndex < root.count - 1)
                view.incrementCurrentIndex()
            else
                event.accepted = false
        }
        Navigation.onUp: {
            if (navigationEnabled && root.currentIndex > 0)
                view.decrementCurrentIndex()
            else
                event.accepted = false
        }
    }
}
