QT += quick multimedia network

CONFIG += c++11

win32:QMAKE_POST_LINK += deploy.bat
unix:QMAKE_POST_LINK += sh deploy.sh


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
    src/models/albumbrowsemodel.h \
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
    src/quick/quicksearch.h \
    src/quick/quicktrackinfo.h \
    src/quick/enums.h \
    src/quick/models.h \
    src/audiooutput.h \
    src/searchengine.h \
    src/utilities/predictioncollection.h \
    src/models/queuemodel.h \
    src/utilities/persistentplaylistindex.h \
    src/utilities/qobjectscopedpointer.h

SOURCES += \
    src/navigation/navigation.cpp \
    src/navigation/navigationnativeeventfilter.cpp \
    src/navigation/lircremote.cpp \
    src/main.cpp \
    appkey.c \
    src/models/abstracttrackcollectionmodel.cpp \
    src/models/abstractalbumcollectionmodel.cpp \
    src/models/albumlistmodel.cpp \
    src/models/albumbrowsemodel.cpp \
    src/quick/quicktrackinfo.cpp \
    src/quick/quicksearch.cpp \
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
    src/utilities/persistentplaylistindex.cpp

OTHER_FILES += \
    assets/qml/rework2/main.qml \
    assets/qml/rework2/controller.qml \
    assets/qml/rework2/main/MenuEntry.qml \
    assets/qml/rework2/main/MainMenuList.qml \
    assets/qml/rework2/main/Sidebar.qml \
    assets/qml/rework2/common/VirtualKeyboard.js \
    assets/qml/rework2/common/Time.js \
    assets/qml/rework2/common/States.js \
    assets/qml/rework2/common/VirtualKeyboard.qml \
    assets/qml/rework2/login/login.qml \
    assets/qml/rework2/login/LoginField.qml \
    assets/qml/rework2/playlists/playlists.qml \
    assets/qml/rework2/common/CollectionView.qml \
    assets/qml/rework2/common/CollectionDelegate.qml \
    assets/qml/rework2/common/states.js \
    assets/qml/rework2/common/PageView.qml \
    assets/qml/rework2/playlists/ContainerPage.qml \
    assets/qml/rework2/playlists/PlaylistPage.qml \
    assets/qml/rework2/NowPlaying.qml \
    assets/qml/rework2/common/Selectable.qml \
    assets/qml/rework2/search/search.qml \
    assets/qml/rework2/nowplaying/nowplaying.qml \
    assets/qml/rework2/discover/discover.qml \
    assets/qml/rework2/common/VirtualKeyboardInput.qml \
    assets/qml/rework2/search/QueryPage.qml \
    assets/qml/rework2/search/ResultsPage.qml \
    assets/qml/rework2/common/TrackView.qml \
    assets/qml/rework2/common/SpotifyImage.qml \
    assets/qml/rework2/common/H1.qml \
    assets/qml/rework2/common/H2.qml \
    assets/qml/rework2/common/H3.qml \
    assets/qml/rework2/common/H4.qml \
    assets/qml/rework2/common/Label.qml \
    assets/qml/rework2/common/MenuView.qml \
    assets/qml/rework2/common/ArtistView.qml

