import QtQuick 2.0
import com.bitfraction.sonata 0.1

FocusScope {
    default property alias data: row.data
    property alias add: row.add
    property alias effectiveLayoutDirection: row.effectiveLayoutDirection
    property alias layoutDirection: row.layoutDirection
    property alias move: row.move
    property alias populate: row.populate
    property alias spacing: row.spacing
    property int focusIndex: 0

    height: row.childrenRect.height
    width: row.childrenRect.width

    Row {
        id: row
        focus: true
    }

    onFocusChanged: {
        if (activeFocus)
            row.children[focusIndex].forceActiveFocus()
    }

    onFocusIndexChanged: {
        if (focusIndex >= 0 && focusIndex < row.children.length)
        {
            // Valid range, force focus
            row.children[focusIndex].forceActiveFocus()
        }
        else if (focusIndex < 0)
            focusIndex = 0;
        else if (focusIndex >= children.length)
            focusIndex = children.length - 1
    }

    Navigation.onLeft: { console.log("Left!"); decrementFocusIndex() }
    Navigation.onRight: { console.log("Right!"); incrementFocusIndex() }

    function incrementFocusIndex()
    {
        ++focusIndex;
    }

    function decrementFocusIndex()
    {
        --focusIndex;
    }

    Component.onCompleted: {
        if (focusIndex >= 0 && focusIndex < row.children.length)
            row.children[focusIndex].forceActiveFocus()
    }
}
