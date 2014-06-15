import QtQuick 2.3

import "../../common"

CollectionView {
    delegate: Pattern {
        height: 100
        Text {
            text: model.name
            color: UI.colors.text
        }
    }
}
