import QtQuick 2.3

import "../"

CollectionView {
    id: root
    delegate: ArtistDelegate {
        onHeightChanged: root.delegateHeight = height
    }
}
