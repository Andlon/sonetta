import QtQuick 2.0

Item {
    id: root

    QtObject {
        id: m

        property var prevObject
        property var currentObject
        property var nextObject

        property var component

        function createObject()
        {
            if (m.currentObject)
                m.currentObject.destroy()
            m.currentObject = component.createObject(root, {"width": root.width, "height": root.height });
            m.currentObject.focus = true
        }

        function updateGeometry()
        {
            var count = root.children.length;

            for (var i = 0; i < count; ++i)
            {
                root.children[i].width = root.width;
                root.children[i].height = root.height;
            }
        }
    }

    function load(file)
    {
        m.component = Qt.createComponent(file);

        if (m.component.status === Component.Ready)
            m.createObject();
        else
            m.component.statusChanged.connect(m.createObject());
    }

    function loadRight(file)
    {

    }

    function loadLeft(file)
    {

    }

    onHeightChanged: m.updateGeometry()
    onWidthChanged: m.updateGeometry()
}
