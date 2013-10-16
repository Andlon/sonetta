import QtQuick 2.1
import Sonetta 0.1

import "Time.js" as Time

CollectionView {
    id: root

    signal trackPlayed(var modelIndex)

    property bool singleRowLayout: true

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

    delegate: singleRowLayout ? singleRow : dualRow

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

    Component {
        id: dualRow
        CollectionDelegate {
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
    }

    Component {
        id: singleRow
        CollectionDelegate {
            id: delegateRoot

            property string name: model ? model.name : ""
            property string artists: model ? model.artistNames.join(", ") : ""
            property string duration: model ? Time.formatTime(model.duration) : "--/--"
            property bool isExplicit: model ? model.isExplicit !== undefined ? model.isExplicit : true : true
            property color textColor:  {
                if (isCurrentItem && delegateRoot.activeFocus && !contextActive)
                    return ui.colors.highlightText
                else if (isExplicit)
                    return ui.colors.standard
                else
                    return ui.colors.label
            }

            height: childrenRect.height + ui.misc.globalPadding

            Behavior on textColor {
                ColorAnimation { duration: ui.misc.globalAnimationTime; easing.type: Easing.InOutQuint }
            }

            H4 {
                id: trackName
                anchors {
                    left: parent.left
                    leftMargin: ui.misc.globalPadding
                }

                width: parent.width * 0.5

                y: ui.misc.globalPadding / 2

                color: textColor
                text: name
            }

            Label {
                id: durationDisplay
                anchors {
                    left: trackName.right
                    leftMargin: ui.misc.globalPadding
                    verticalCenter: trackName.verticalCenter
                }

                font: ui.fonts.standard
                text: duration
            }

            H4 {
                anchors {
                    verticalCenter: trackName.verticalCenter
                    left: durationDisplay.right
                    right: parent.right
                    rightMargin: ui.misc.globalPadding
                    leftMargin: ui.misc.globalPadding
                }

                text: artists
                color: textColor
            }
        }
    }
}
