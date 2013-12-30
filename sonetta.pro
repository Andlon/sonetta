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
    src/settings.h \
    src/utilities/mosaicgenerator.h \
    src/quick/quickmosaicgenerator.h \
    src/models/albummodel.h \
    src/quick/quickfactory.h \
    src/navigation/lircclient.h \
    src/utilities/pointers.h

SOURCES += \
    src/navigation/navigation.cpp \
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
    src/models/albummodel.cpp \
    src/navigation/lircclient.cpp

OTHER_FILES += \
    assets/interfaces/default/main.qml \
    assets/interfaces/default/controller.qml \
    assets/interfaces/default/main/MainMenuList.qml \
    assets/interfaces/default/main/Sidebar.qml \
    assets/interfaces/default/common/VirtualKeyboard.js \
    assets/interfaces/default/common/Time.js \
    assets/interfaces/default/common/States.js \
    assets/interfaces/default/common/VirtualKeyboard.qml \
    assets/interfaces/default/login/login.qml \
    assets/interfaces/default/login/LoginField.qml \
    assets/interfaces/default/playlists/playlists.qml \
    assets/interfaces/default/common/CollectionView.qml \
    assets/interfaces/default/common/CollectionDelegate.qml \
    assets/interfaces/default/common/states.js \
    assets/interfaces/default/playlists/ContainerPage.qml \
    assets/interfaces/default/playlists/PlaylistPage.qml \
    assets/interfaces/default/NowPlaying.qml \
    assets/interfaces/default/common/Selectable.qml \
    assets/interfaces/default/search/search.qml \
    assets/interfaces/default/nowplaying/nowplaying.qml \
    assets/interfaces/default/common/VirtualKeyboardInput.qml \
    assets/interfaces/default/search/QueryPage.qml \
    assets/interfaces/default/search/ResultsPage.qml \
    assets/interfaces/default/common/TrackView.qml \
    assets/interfaces/default/common/SpotifyImage.qml \
    assets/interfaces/default/common/H1.qml \
    assets/interfaces/default/common/H2.qml \
    assets/interfaces/default/common/H3.qml \
    assets/interfaces/default/common/H4.qml \
    assets/interfaces/default/common/Label.qml \
    assets/interfaces/default/common/MenuView.qml \
    assets/interfaces/default/common/ArtistView.qml \
    assets/interfaces/default/common/AlbumView.qml \
    assets/interfaces/default/nowplaying/Control.qml \
    assets/interfaces/default/common/CollectionContext.qml \
    assets/interfaces/default/common/BackIndicator.qml \
    assets/interfaces/default/common/VerticalScrollbar.qml \
    assets/interfaces/default/session/session.qml \
    assets/interfaces/default/common/Mosaic.qml \
    assets/interfaces/default/common/Menu.qml \
    assets/interfaces/default/common/MenuAction.qml \
    assets/interfaces/default/main/MainMenuEntry.qml \
    assets/interfaces/default/common/Pattern.qml \
    assets/interfaces/default/common/CollectionHighlight.qml \
    assets/interfaces/default/nowplaying/Playback.qml \
    assets/interfaces/default/nowplaying/Trackbar.qml \
    assets/interfaces/default/explore/explore.qml \
    assets/interfaces/default/common/StackView.qml \
    assets/interfaces/default/explore/AlbumBrowse.qml \
    assets/interfaces/default/common/UI.qml \
    assets/interfaces/default/common/Page.qml \
    assets/interfaces/default/common/PageView.qml \
    assets/interfaces/default/explore/tophits.qml

