import QtQuick 2.1
import Sonetta 0.1

CollectionView {
    id: root

    signal trackPlayed(var modelIndex)

    contextModel: ListModel {
        ListElement {
            display: "Play"
            name: "play"
        }

        ListElement {
            display: "Queue"
            name: "queue"
        }
    }

    delegate: CollectionDelegate {
        id: delegateRoot
        height: 100

        property string name: model ? model.name : ""
        property string artists: model ? model.artistNames.join(", ") : ""
        property bool isExplicit: model ? model.isExplicit !== undefined ? model.isExplicit : true : true
        property color textColor:  {
            if (isCurrentItem && delegateRoot.activeFocus && !contextActive)
                return ui.colors.highlightText
            else if (isExplicit)
                return ui.colors.standard
            else
                return ui.colors.label
        }

        Behavior on textColor {
            ColorAnimation { duration: ui.misc.globalAnimationTime; easing.type: Easing.InOutQuint }
        }

        Column {
            anchors {
                verticalCenter: parent.verticalCenter
                left: parent.left
                right: parent.right
                leftMargin: ui.misc.globalPadding
                rightMargin: ui.misc.globalPadding
            }

            H4 {
                anchors {
                    left: parent.left
                    right: parent.right
                }

                color: textColor
                text: name
            }

            Text {
                anchors {
                    left: parent.left
                    right: parent.right
                }

                text: artists
                color: textColor
                font: ui.fonts.standard
                elide: Text.ElideRight
            }
        }
    }

    Navigation.onRecord: {
        if (currentItem)
        {
            player.enqueue(currentItem.internalModel.track)
        }
    }

    Keys.onPressed:
    {
        if (event.key === Qt.Key_Q && currentItem)
        {
            player.enqueue(currentItem.internalModel.track)
            event.accepted = true
        }
    }

    onItemPressed: {
        player.play(data.track)
        trackPlayed(data.index)
    }

    onContextPressed: {
        if (name == "play")
        {
            player.play(data.track)
            trackPlayed(data.index)
        }
        else if (name == "queue")
        {
            player.enqueue(data.track)
        }
    }
}
