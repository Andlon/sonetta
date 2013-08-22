import QtQuick 2.0
import com.bitfraction.sonata 0.1

FocusScope {
    default property alias data: col.data
    property alias add: col.add
    property alias move: col.move
    property alias populate: col.populate
    property alias spacing: col.spacing
    property int focusIndex: 0

    height: col.childrenRect.height
    width: col.childrenRect.width

    Column {
        anchors.fill: parent
        id: col
        focus: true
    }

    onActiveFocusChanged: {
        if (activeFocus)
            col.children[focusIndex].forceActiveFocus()
    }

    onFocusIndexChanged: {
        if (focusIndex >= 0 && focusIndex < col.children.length)
        {
            // Valid range, force focus
            col.children[focusIndex].forceActiveFocus()
        }
        else if (focusIndex < 0)
            focusIndex = 0;
        else if (focusIndex >= children.length)
            focusIndex = col.children.length - 1
    }

    function incrementFocusIndex()
    {
        ++focusIndex;
    }

    function decrementFocusIndex()
    {
        --focusIndex;
    }

    Navigation.onUp: { decrementFocusIndex() }
    Navigation.onDown: { incrementFocusIndex() }

    Component.onCompleted: {
        if (activeFocus && focusIndex >= 0 && focusIndex < col.children.length)
            col.children[focusIndex].forceActiveFocus()
    }
}
