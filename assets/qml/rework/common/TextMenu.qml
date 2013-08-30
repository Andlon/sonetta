import QtQuick 2.1
import QtQml.Models 2.1
import sonata 0.1

ListView {
    id: root
    width: 200
    height: 200
    model: items
    highlightFollowsCurrentItem: true

    default property alias children: items.children
    property alias selectionDelegate: selectionLoader.sourceComponent
    property bool stickySelection: false
    property int selectedIndex: 0
    property var selectedItem: selectionLoader.validateSelectionIndex() ? items.children[selectedIndex] : undefined

    signal selected

    ObjectModel {
        id: items
    }

    highlight: Image {
        visible: root.activeFocus
        cache: true
        source: "../images/highlightTile.png"
        fillMode: Image.Tile
        width: root.currentItem.width
        height: root.currentItem.height
    }

    Loader {
        id: selectionLoader
        visible: root.stickySelection
        parent: root.contentItem
        anchors.fill: selectedItem
        z: -1

        function validateSelectionIndex()
        {
            return selectedIndex >= 0 && selectedIndex < root.count
        }
    }

    Navigation.onDown: incrementCurrentIndex()
    Navigation.onUp: decrementCurrentIndex()
    Navigation.onOk: {
        selectedIndex = currentIndex
        selected()
    }
}
