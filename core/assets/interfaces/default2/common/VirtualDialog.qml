import QtQuick 2.2
import QtGraphicalEffects 1.0
import Navigation 0.1

import "."
import "Dialog.js" as Dialog

FocusScope {
    id: root

    default property Component dialog: null

    signal activated
    signal activating
    signal deactivated
    signal deactivating

    z: 10
    states: [
        State {
            name: "activating"
            PropertyChanges { target: loader; active: true }
            PropertyChanges { target: root; visible: true }
            StateChangeScript { script: { root.activating();  root.forceActiveFocus() } }
        },
        State {
            name: "active"
            PropertyChanges { target: loader; active: true }
            PropertyChanges { target: root; visible: true }
            PropertyChanges { target: Dialog.contentItem(); visible: false }
            StateChangeScript { script: root.activated() }
        },
        State {
            name: "deactivating"
            PropertyChanges { target: loader; active: true }
            PropertyChanges { target: Dialog.contentItem(); visible: true }
            StateChangeScript { script: { root.deactivating(); Dialog.contentItem().forceActiveFocus() } }
        },
        State {
            name: "inactive"
            PropertyChanges { target: root; visible: false }
            PropertyChanges { target: loader; active: false }
        }
    ]
    state: "inactive"

    transitions: Transition {
        from: "deactivating"
        to: "inactive"
        // Make this part of transition so that it's not run on initial state
        ScriptAction { script: root.deactivated() }
    }

    Loader {
        id: loader
        anchors.fill: parent
        focus: true
        sourceComponent: Item {
            // Need to replicate states here because this item is not
            // accessible from root's scope
            states: [
                State {
                    name: "activating"
                    when: root.state === "activating"
                    PropertyChanges { target: blur; radius: UI.dialog.blurRadius }
                    PropertyChanges { target: brightness; brightness: UI.dialog.brightnessDelta }
                    PropertyChanges { target: dialogLoader; scale: 1; opacity: 1 }
                },
                State {
                    name: "active"
                    when: root.state === "active"
                    PropertyChanges { target: blur; radius: UI.dialog.blurRadius }
                    PropertyChanges { target: brightness; brightness: UI.dialog.brightnessDelta }
                    PropertyChanges { target: dialogLoader; scale: 1; opacity: 1 }
                },
                State {
                    name : "deactivating"
                    when: root.state === "deactivating"
                    PropertyChanges { target: blur; radius: 0 }
                    PropertyChanges { target: brightness; brightness: 0 }
                    PropertyChanges { target: dialogLoader; scale: 0; opacity: 0 }
                },
                State {
                    name: "inactive"
                    when: root.state === "inactive"
                    PropertyChanges { target: blur; radius: 0 }
                    PropertyChanges { target: brightness; brightness: 0 }
                    PropertyChanges { target: dialogLoader; scale: 0; opacity: 0 }
                }
            ]
            state: "inactive"

            transitions: [
                Transition {
                    from: "inactive"
                    to: "activating"
                    SmoothedAnimation { targets: [blur, brightness]; properties: "radius,brightness"; duration: UI.timing.dialogPopup; velocity: -1 }
                    SequentialAnimation {
                        SmoothedAnimation { target: dialogLoader; properties: "scale,opacity"; duration: UI.timing.dialogPopup; velocity: -1 }
                        ScriptAction { script: m.advanceState() }
                    }
                },
                Transition {
                    from: "active"
                    to: "deactivating"
                    SmoothedAnimation { targets: [blur, brightness]; properties: "radius,brightness"; duration: UI.timing.dialogPopup; velocity: -1 }
                    SequentialAnimation {
                        SmoothedAnimation { target: dialogLoader; properties: "scale,opacity"; duration: UI.timing.dialogPopup; velocity: -1 }
                        ScriptAction { script: m.advanceState() }
                    }
                }
            ]

            FastBlur {
                id: blur
                anchors.fill: parent
                source: brightness
            }

            BrightnessContrast {
                id: brightness
                source: Dialog.contentItem()
                anchors.fill: parent
                visible: false
            }

            Loader {
                id: dialogLoader
                focus: true
                anchors.fill: parent
                sourceComponent: root.dialog
                scale: 0
            }
        }
    }

    QtObject {
        id: m

        function advanceState() {
            // Advance state when animations are finished
            if (root.state == "activating")
                root.state = "active"
            else if (root.state == "deactivating")
                root.state = "inactive"
        }
    }

    function activate() {
        if (root.state == "inactive")
            root.state = "activating"
    }

    function deactivate() {
        if (root.state == "active")
            root.state = "deactivating"
    }

    Component.onCompleted: {
        // Parent item to root
        root.parent = Dialog.rootItem()
        anchors.fill = root.parent
    }

    Navigation.onBack: deactivate()

    Keys.onPressed: {
        // Block all keys unless it's Esc
        if (event.key === Qt.Key_Escape)
            deactivate()
        else
            event.accepted = true
    }
}
