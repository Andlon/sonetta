import QtQuick 2.1
import QtQml.Models 2.1
import Sonetta 0.1
import "../common"

PageView {
    id: root
    // Apparently this is necessary, no idea why
    focus: true

    currentIndex: {
        var state = ui.state
        if (state.search.stage === "query")
            return 0
        else if (state.search.stage === "results")
            return 1

        return 0
    }

    model: ObjectModel {
        QueryPage {
            width: root.cellWidth
            height: root.cellHeight
        }

        ResultsPage {
            width: root.cellWidth
            height: root.cellHeight
        }
    }

}
