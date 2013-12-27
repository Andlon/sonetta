import QtQuick 2.2

Item {
    property QtObject model: internalModel
    property var modelData: internalModelData
    property bool isCurrentItem: internalIsCurrentItem
    focus: true
}
