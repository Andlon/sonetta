import QtQuick 2.2
import Sonetta 0.1

FocusScope {
    id: root
    focus: true

    property string page: ""

    signal enter(var parameters)
    signal leave

    Connections {
        target: UI

        onEnteringPage: {
            if (page === root.page)
            {
                root.enter(parameters)
            }
        }

        onLeavingPage: {
            if (page === root.page)
            {
                root.leave()
            }
        }
    }
}
