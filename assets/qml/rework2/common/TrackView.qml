import QtQuick 2.1
import Sonetta 0.1

CollectionView {
    id: root

    signal trackPlayed(var modelIndex)

    delegate: CollectionDelegate {
        height: 100

        property string name: model ? model.name : ""
        property string artists: model ? model.artistNames.join(", ") : ""
        property bool isExplicit: model ? model.isExplicit !== undefined ? model.isExplicit : true : true
        property color textColor: isExplicit || (isCurrentItem && root.activeFocus) ? ui.colors.standard : ui.colors.label

        Behavior on textColor {
            ColorAnimation { duration: ui.misc.globalAnimationTime }
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

    onItemPressed: {
        player.play(data.track)
        trackPlayed(data.index)
    }

    Navigation.onRecord: {
        if (currentItem)
        {
            player.enqueue(currentItem.internalModel.track)
        }
    }
}
