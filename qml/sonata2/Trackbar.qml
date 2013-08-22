import QtQuick 2.0

Item {
    id: root
    width: 200
    height: 25

    property alias backgroundColor: body.color
    property alias foregroundColor: tracker.color
    property real position: 0
    property alias border: body.border

    property int trackerWidth: 25
    property int trackerMargins: 3

    signal clicked(real pos)

    onPositionChanged:
    {
        // Make sure position is within bounds
        if (position > 1)
            position = 1;
        else if (position < 0)
            position = 0;
    }


    Rectangle {
        id: body
        anchors.fill: parent
        radius: height / 2
        smooth: true
    }

    Rectangle {
        id: tracker
        anchors.top: body.top
        anchors.bottom: body.bottom
        anchors.margins: root.trackerMargins
        radius: height / 2
        smooth: true

        width: root.trackerWidth

        x: area.drag.active ? x : root.position * (body.width - width - 2 * root.trackerMargins) + root.trackerMargins;

        Behavior on x {
            NumberAnimation { alwaysRunToEnd: false }
        }
    }

    MouseArea {
        id: area
        anchors.fill: body
        drag.target: tracker
        drag.axis: Drag.XAxis
        drag.maximumX: body.width - root.trackerMargins
        drag.minimumX: root.trackerMargins

        drag.onActiveChanged: {
            if (!drag.active)
            {
                root.clicked(tracker.x / body.width);
            }
        }

        onClicked:
        {
            if (mouse.x > root.trackerMargins && mouse.x < body.width - root.trackerMargins)
            {
                var pos = mouse.x / body.width;
                root.clicked(pos);
            }
        }

    }
}
