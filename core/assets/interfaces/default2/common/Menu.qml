import QtQuick 2.3
import QtQml.Models 2.1
import Navigation 0.1

import "."

ListView {
    id: root

    // This works, but as it's not documented it MAY break in the future
    // ... but let's hope not
    default property alias menuItems: objects.children
    property bool navigationEnabled: true

    height: contentItem.childrenRect.height
    width: UI.menu.defaultWidth
    highlightMoveDuration: UI.timing.highlightMove
    currentIndex: 0
    highlight: Rectangle { color: UI.colors.focus }

    model: ObjectModel { id: objects }

    Navigation.onDown: {
        if (navigationEnabled && root.currentIndex < root.count - 1)
            root.incrementCurrentIndex()
        else
            event.accepted = false
    }
    Navigation.onUp: {
        if (navigationEnabled && root.currentIndex > 0)
            root.decrementCurrentIndex()
        else
            event.accepted = false
    }
}
