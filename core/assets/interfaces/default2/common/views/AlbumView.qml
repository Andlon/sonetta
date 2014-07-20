import QtQuick 2.3

import "../"

CollectionView {
    id: root
    delegate: AlbumDelegate {
        onHeightChanged: root.delegateHeight = height
    }
}
