QT += quick multimedia network

CONFIG += c++11

win32:QMAKE_POST_LINK += deploy.bat
unix:QMAKE_POST_LINK += sh deploy.sh

# Add rpath so that deployed libs can be found in libs/ subdirectory on Linux
unix:!mac {
    LIBS += -Wl,-rpath=\\\$$ORIGIN/libs
}


RCC_DIR = "build/rcc"
UI_DIR = "build/uic"
MOC_DIR = "build/moc"
OBJECTS_DIR = "build/obj"

CONFIG(debug, debug|release) {
    DESTDIR = "debug"
}
CONFIG(release, debug|release) {
    DESTDIR = "release"
}

INCLUDEPATH -= "$$PWD"

QML_IMPORT_PATH += "$$PWD/qml/modules"

include("dependencies/spotinetta/link.pri")


HEADERS += \
    src/navigation/navigation.h \
    src/navigation/navigationnativeeventfilter.h \
    src/navigation/lircremote.h \
    src/models/abstracttrackcollectionmodel.h \
    src/models/abstractalbumcollectionmodel.h \
    src/models/albumlistmodel.h \
    src/models/tracklistmodel.h \
    src/models/abstractartistcollectionmodel.h \
    src/models/artistlistmodel.h \
    src/player.h \
    src/uistatecoordinator.h \
    src/application.h \
    src/imageprovider.h \
    src/utilities/session.h \
    src/models/playlistmodel.h \
    src/models/playlistcontainermodel.h \
    src/quick/quicktrackinfo.h \
    src/quick/enums.h \
    src/quick/models.h \
    src/audiooutput.h \
    src/searchengine.h \
    src/utilities/predictioncollection.h \
    src/models/queuemodel.h \
    src/utilities/persistentplaylistindex.h \
    src/utilities/qobjectscopedpointer.h \
    src/settings.h \
    src/utilities/mosaicgenerator.h \
    src/quick/quickmosaicgenerator.h \
    src/models/albummodel.h \
    src/quick/quickfactory.h

SOURCES += \
    src/navigation/navigation.cpp \
    src/navigation/navigationnativeeventfilter.cpp \
    src/navigation/lircremote.cpp \
    src/main.cpp \
    appkey.c \
    src/models/abstracttrackcollectionmodel.cpp \
    src/models/abstractalbumcollectionmodel.cpp \
    src/models/albumlistmodel.cpp \
    src/quick/quicktrackinfo.cpp \
    src/models/tracklistmodel.cpp \
    src/models/abstractartistcollectionmodel.cpp \
    src/models/artistlistmodel.cpp \
    src/player.cpp \
    src/uistatecoordinator.cpp \
    src/application.cpp \
    src/imageprovider.cpp \
    src/models/playlistmodel.cpp \
    src/models/playlistcontainermodel.cpp \
    src/audiooutput.cpp \
    src/searchengine.cpp \
    src/utilities/predictioncollection.cpp \
    src/models/queuemodel.cpp \
    src/utilities/persistentplaylistindex.cpp \
    src/settings.cpp \
    src/utilities/mosaicgenerator.cpp \
    src/models/albummodel.cpp

OTHER_FILES += \
    assets/qml/default/main.qml \
    assets/qml/default/controller.qml \
    assets/qml/default/main/MainMenuList.qml \
    assets/qml/default/main/Sidebar.qml \
    assets/qml/default/common/VirtualKeyboard.js \
    assets/qml/default/common/Time.js \
    assets/qml/default/common/States.js \
    assets/qml/default/common/VirtualKeyboard.qml \
    assets/qml/default/login/login.qml \
    assets/qml/default/login/LoginField.qml \
    assets/qml/default/playlists/playlists.qml \
    assets/qml/default/common/CollectionView.qml \
    assets/qml/default/common/CollectionDelegate.qml \
    assets/qml/default/common/states.js \
    assets/qml/default/playlists/ContainerPage.qml \
    assets/qml/default/playlists/PlaylistPage.qml \
    assets/qml/default/NowPlaying.qml \
    assets/qml/default/common/Selectable.qml \
    assets/qml/default/search/search.qml \
    assets/qml/default/nowplaying/nowplaying.qml \
    assets/qml/default/common/VirtualKeyboardInput.qml \
    assets/qml/default/search/QueryPage.qml \
    assets/qml/default/search/ResultsPage.qml \
    assets/qml/default/common/TrackView.qml \
    assets/qml/default/common/SpotifyImage.qml \
    assets/qml/default/common/H1.qml \
    assets/qml/default/common/H2.qml \
    assets/qml/default/common/H3.qml \
    assets/qml/default/common/H4.qml \
    assets/qml/default/common/Label.qml \
    assets/qml/default/common/MenuView.qml \
    assets/qml/default/common/ArtistView.qml \
    assets/qml/default/common/AlbumView.qml \
    assets/qml/default/nowplaying/Controls.qml \
    assets/qml/default/nowplaying/Control.qml \
    assets/qml/default/common/CollectionContext.qml \
    assets/qml/default/common/BackIndicator.qml \
    assets/qml/default/common/VerticalScrollbar.qml \
    assets/qml/default/session/session.qml \
    assets/qml/default/common/Mosaic.qml \
    assets/qml/default/common/Menu.qml \
    assets/qml/default/common/MenuAction.qml \
    assets/qml/default/main/MainMenuEntry.qml \
    assets/qml/default/common/Pattern.qml \
    assets/qml/default/common/CollectionHighlight.qml \
    assets/qml/default/nowplaying/Playback.qml \
    assets/qml/default/nowplaying/Trackbar.qml \
    assets/qml/default/explore/explore.qml \
    assets/qml/default/common/StackView.qml \
    assets/qml/default/explore/AlbumBrowse.qml \
    assets/qml/default/common/UI.qml \
    assets/qml/default/common/Page.qml \
    assets/qml/default/common/PageView.qml

