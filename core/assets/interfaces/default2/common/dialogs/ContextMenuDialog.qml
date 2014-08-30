import QtQuick 2.3
import QtQml.Models 2.1
import Navigation 0.1

import ".."

VirtualDialog {
    id: root

    /*
     *  Use the following syntax for setting the items used by the context menu:
     *  actions: [ [ "identifier", "Display text" ], [ "enqueue", "Add to queue" ] ]
     */
    property var actions
    signal actionSelected(var id)

    property Component context

    property string menuHeader

    dialog: FocusScope {
        focus: true
        property var actions: root.actions

        Loader {
            id: contextLoader
            sourceComponent: context
        }

        FramelessSection {
            anchors.centerIn: parent
            contentHeight: menu.height
            contentWidth: menu.width
            header: root.menuHeader

            Menu {
                id: menu
                focus: true
                model: contextActions
                delegate: menuDelegate
            }
        }

        ListModel {
            id: contextActions
        }

        onActionsChanged: repopulateActions()

        function repopulateActions() {
            contextActions.clear()

            if (actions instanceof Array) {
                for (var i = 0; i < actions.length; ++i) {
                    var action = actions[i];
                    if (action instanceof Array && action.length === 2) {
                        var id = action[0];
                        var display = action[1];
                        contextActions.append({ "id": id, "display": display });
                    } else {
                        console.error("ContextMenuDialog: Actions must confirm to 2-element arrays.");
                    }
                }
            } else {
                console.error("ContextMenuDialog: Expected an array of actions.");
            }
        }

        Component {
            id: menuDelegate
            MenuTextItem {
                text: model && model.display ? model.display : ""

                Navigation.onOk: root.actionSelected(model.id)
            }
        }
    }
}
