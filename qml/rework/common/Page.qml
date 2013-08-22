import QtQuick 2.0

FocusScope {
    width: parent ? parent.parent ? parent.parent.width : 0 : 0
    height: parent ? parent.parent ? parent.parent.height : 0 : 0
}
