import QtQuick 2.3

import "../../common"

FocusScope {
    id: root
    property alias model: history.model

    signal queryRequested(var query)

    FramelessSection {
        id: historySection
        header: "History - Recent Queries"
        anchors.fill: parent

        CollectionView {
            id: history
            anchors.fill: parent
            focus: true
            delegate: SimpleTextDelegate { text: model && model.modelData ? model.modelData : "" }

            onItemPressed: root.queryRequested(model.modelData)
        }
    }
}
