import QtQuick 2.3

import "../../common"

FocusScope {
    id: root
    property alias model: history.model

    signal queryRequested(var query)

    Section {
        id: historySection
        header: "History - Recent Queries"
        showFrame: false
        padding: 0
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
