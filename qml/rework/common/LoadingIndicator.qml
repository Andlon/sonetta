import QtQuick 2.0

Image {
    id: root
    source: "../images/loading.png"
    scale: visible ? 1 : 0
    opacity: visible ? 1 : 0

    RotationAnimation {
        target: root
        running: root.visible
        loops: Animation.Infinite
        from: 0
        to: 360
        property: "rotation"
        duration: 3000
    }

    Behavior on opacity {
        NumberAnimation { duration: 400; }
    }

    Behavior on scale {
        NumberAnimation { duration: 400; }
    }
}
