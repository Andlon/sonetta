import QtQuick 2.0
import com.bitfraction.sonata 0.1

FocusScope {
    width: 400
    height: 400

    Loader {
        id: loader
        anchors.top: parent.top
        anchors.bottom: currentTrackContainer.top
        anchors.left: parent.left
        anchors.right: parent.right
        focus: true

        sourceComponent: mainMenu

        Keys.onPressed:
        {
            if (event.key === Qt.Key_Backspace)
            {
                sonata.section = Sonata.MainSection;
                event.accept();
            }

        }
        Navigation.onBack: sonata.section = Sonata.MainSection
    }

    Rectangle {
        id: currentTrackContainer
        color: "#272727"
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom

        height: currentTrack.height + 1.5 * currentTrack.anchors.margins + line.height

        Rectangle {
            id: line
            height: 3
            color: "#7e7e7e"
            anchors {
                top: parent.top
                left: parent.left
                right: parent.right
            }
        }

        CurrentTrack {
            id: currentTrack
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.bottom: parent.bottom
            anchors.margins: 30
        }
    }

    Connections {
        target: sonata
        onSectionChanged: {
            if (sonata.section === Sonata.MainSection)
                loader.sourceComponent = mainMenu
            else if (sonata.section === Sonata.PlaylistSection)
                loader.source = "playlistSection.qml"
        }
    }

    Component {
        id: mainMenu
        FocusScope {
            focus: true
            Rectangle {
                visible: sonata.section === Sonata.MainSection
                height: 100
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.verticalCenter: parent.verticalCenter
                color: "#66000000"

                gradient: Gradient {
                    GradientStop { position: 0; color: "#33000000" }
                    GradientStop { position: 0.3; color: "#66000000" }
                    GradientStop { position: 0.7; color: "#66000000" }
                    GradientStop { position: 1; color: "#33000000" }
                }

                PathView {
                    id: view
                    anchors.fill: parent
                    model: menuEntries

                    focus: true

                    currentIndex: 1

                    preferredHighlightBegin: 0.5
                    preferredHighlightEnd: 0.5
                    highlightRangeMode: PathView.StrictlyEnforceRange
                    snapMode: PathView.SnapToItem

                    path: Path {
                        startX: 0
                        startY: view.height / 2
                        PathLine {
                            x: view.width
                            relativeY: 0
                        }
                    }

                    Navigation.onRight: incrementCurrentIndex()
                    Navigation.onLeft: decrementCurrentIndex()
                    Navigation.onOk: updateSection()

                    delegate: Text {
                        text: model.title
                        color: PathView.isCurrentItem ? "#ffffff" : "#9c9c9c"
                        font.pointSize: 33
                        font.bold: true
                        font.family: "Roboto"
                        font.capitalization: Font.AllUppercase
                    }

                    function updateSection() {
                        if (view.currentIndex === 1)
                            sonata.section = Sonata.PlaylistSection
                        else if (view.currentIndex === 3)
                            spotify.logout()
                    }
                }
            }
        }
    }


    ListModel {
        id: menuEntries
        ListElement {
            title: "Search"
        }

        ListElement {
            title: "Playlists"
        }

        ListElement {
            title: "Top Lists"
        }

        ListElement {
            title: "Logout"
        }
    }
}
