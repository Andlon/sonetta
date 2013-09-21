import QtQuick 2.1
import Sonetta 0.1

CollectionView {
    id: root

    signal trackPlayed(var modelIndex)

    delegate: CollectionDelegate {
        height: 100
        width: view.width

        property string name: model ? model.name : ""
        property string artists: model ? model.artistNames.join(", ") : ""

        Column {
            anchors {
                verticalCenter: parent.verticalCenter
                left: parent.left
                right: parent.right
                leftMargin: ui.misc.globalPadding
                rightMargin: ui.misc.globalPadding
            }

            Text {
                anchors {
                    left: parent.left
                    right: parent.right
                }

                text: name
                color: ui.colors.standard
                font: ui.fonts.h4
                elide: Text.ElideRight
            }

            Text {
                anchors {
                    left: parent.left
                    right: parent.right
                }

                text: artists
                color: ui.colors.standard
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
