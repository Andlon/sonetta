import QtQuick 2.3

import "../../common"
import "../../common/Time.js" as Time

TrackView {
    id: root
    primaryBackgroundPattern: "medium"

    delegate: DoubleRowTrackDelegate {
        onHeightChanged: root.delegateHeight = height
    }
}
