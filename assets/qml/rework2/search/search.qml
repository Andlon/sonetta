import QtQuick 2.1
import QtQml.Models 2.1
import Sonetta 0.1
import "../common"

PageView {
    id: root
    // Apparently this is necessary, no idea why
    focus: true

    model: ObjectModel {
        QueryPage {
            width: root.cellWidth
            height: root.cellHeight
        }
    }

}
