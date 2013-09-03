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
    src/quicktrackinfo.h \
    src/models/abstracttrackcollectionmodel.h \
    src/models/abstractalbumcollectionmodel.h \
    src/models/albumlistmodel.h \
    src/models/albumbrowsemodel.h \
    src/quicksearch.h \
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
    src/quick/enums.h \
    src/quick/models.h

SOURCES += \
    src/navigation/navigation.cpp \
    src/navigation/navigationnativeeventfilter.cpp \
    src/navigation/lircremote.cpp \
    src/main.cpp \
    src/quicktrackinfo.cpp \
    appkey.c \
    src/models/abstracttrackcollectionmodel.cpp \
    src/models/abstractalbumcollectionmodel.cpp \
    src/models/albumlistmodel.cpp \
    src/models/albumbrowsemodel.cpp \
    src/quicksearch.cpp \
    src/models/tracklistmodel.cpp \
    src/models/abstractartistcollectionmodel.cpp \
    src/models/artistlistmodel.cpp \
    src/player.cpp \
    src/uistatecoordinator.cpp \
    src/application.cpp \
    src/imageprovider.cpp \
    src/models/playlistmodel.cpp \
    src/models/playlistcontainermodel.cpp

OTHER_FILES += \
    qml/sonata2/main.qml \
    qml/sonata2/Login.qml \
    qml/sonata2/ImageButton.qml \
    qml/sonata2/Button.qml \
    qml/sonata2/MainSection.qml \
    qml/sonata2/playlistSection.qml \
    qml/sonata2/time.js \
    qml/sonata2/CurrentTrack.qml \
    qml/sonata2/stop_hover.png \
    qml/sonata2/stop.png \
    qml/sonata2/sonata.png \
    qml/sonata2/play_hover.png \
    qml/sonata2/play.png \
    qml/sonata2/paused_hover.png \
    qml/sonata2/paused.png \
    qml/sonata2/background_temporary.jpg \
    qml/sonata2/abstract_background.png \
    qml/sonata2/Trackbar.qml \
    qml/sonata2/VirtualKeyboard.qml \
    qml/sonata2/NavRow.qml \
    qml/sonata2/NavColumn.qml \
    qml/sonata2/VirtualKeyboard.js \
    qml/sonata2/InputField.qml \
    qml/sonata2/PlaylistView.qml \
    qml/sonata2/PlaylistOptions.qml \
    qml/sonata2/OptionEntry.qml \
    qml/sonata2/TrackOptions.qml \
    qml/rework/main.qml \
    qml/rework/common/VirtualKeyboard.qml \
    qml/rework/common/VirtualKeyboard.js \
    qml/rework/common/PlaceholderInputField.qml \
    qml/rework/login/login.qml \
    qml/rework/States.js \
    qml/rework/common/LoadingIndicator.qml \
    qml/rework/home/home.qml \
    qml/rework/home/HomeEntry.qml \
    qml/rework/QuickMenu.qml \
    qml/rework/search/search.qml \
    qml/rework/PageLoader.qml \
    qml/rework/common/PageView.qml \
    qml/rework/common/TrackCollectionView.qml \
    qml/rework/common/Page.qml \
    qml/rework/NowPlaying.qml \
    qml/rework/common/Time.js \
    qml/rework/common/TextMenu.qml \
    qml/rework/search/ResultPage.qml \
    qml/rework/common/AlbumCollectionView.qml \
    qml/rework/common/CollectionViewHighlight.qml \
    qml/rework/common/ArtistCollectionView.qml \
    assets/qml/rework2/main.qml \
    assets/qml/rework2/controller.qml \
    assets/qml/rework2/common/VirtualKeyboard.js \
    assets/qml/rework2/common/Time.js \
    assets/qml/rework2/common/States.js \
    assets/qml/rework2/common/VirtualKeyboard.qml \
    assets/qml/rework2/login/login.qml \
    assets/qml/rework2/login/LoginField.qml \
    assets/qml/rework2/Sidebar.qml
