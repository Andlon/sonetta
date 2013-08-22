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
INCLUDEPATH += "$$PWD/libspotify/include"
INCLUDEPATH += "$$PWD/include"

QML_IMPORT_PATH += "$$PWD/qml/modules"

win32:LIBS += -L$$PWD/libspotify/lib -llibspotify
unix:LIBS += -L$$PWD/libspotify/lib -lspotify

HEADERS += \
    include/Sonata/sptrack.h \
    include/Sonata/splink.h \
    include/Sonata/definitions.h \
    include/Sonata/spartist.h \
    include/Sonata/spplaylist.h \
    include/Sonata/spplaylistcontainer.h \
    include/Sonata/spplaylistwatcher.h \
    include/Sonata/spplaylistcontainerwatcher.h \
    include/Sonata/sptrackwatcher.h \
    include/Sonata/spartistwatcher.h \
    include/Sonata/spalbum.h \
    include/Sonata/spimage.h \
    include/Sonata/spimagewatcher.h \
    include/Sonata/spartistbrowse.h \
    include/Sonata/spartistbrowsewatcher.h \
    include/Sonata/spotifyenum.h \
    include/Sonata/spotifysession.h \
    src/libsonata/spotifyevent.h \
    src/libsonata/spfactory.h \
    src/libsonata/output/spdefaultoutput.h \
    src/libsonata/spcore.h \
    src/libsonata/output/spaudiooutput.h \
    src/libsonata/output/circularbuffer.h \
    src/libsonata/spplaylistcontainermodel.h \
    src/libsonata/spplaylistmodel.h \
    src/navigation/navigation.h \
    src/navigation/navigationnativeeventfilter.h \
    src/navigation/lircremote.h \
    src/sonataplayer.h \
    src/sonataapplication.h \
    src/uistatemanagement.h \
    src/sonataquickview.h \
    src/quicktrackinfo.h \
    src/playlistmodel.h \
    src/playlistcontainermodel.h \
    src/quickartistsynopsis.h \
    src/quicklinker.h \
    src/spotifyimageprovider.h \
    include/Sonata/spalbumwatcher.h \
    src/models/abstracttrackcollectionmodel.h \
    src/models/abstractalbumcollectionmodel.h \
    src/models/albumlistmodel.h \
    include/Sonata/spalbumbrowse.h \
    include/Sonata/spalbumbrowsewatcher.h \
    src/models/albumbrowsemodel.h \
    include/Sonata/spsearch.h \
    include/Sonata/spsearchwatcher.h \
    src/quicksearch.h \
    src/models/tracklistmodel.h \
    src/models/abstractartistcollectionmodel.h \
    src/models/artistlistmodel.h \
    include/Spotinetta/basicobject.h \
    include/Spotinetta/track.h \
    include/Spotinetta/definitions.h \
    include/Spotinetta/album.h \
    include/Spotinetta/artist.h \
    include/Spotinetta/search.h \
    include/Spotinetta/link.h \
    include/Spotinetta/image.h \
    include/Spotinetta/session.h \
    include/Spotinetta/error.h \
    src/spotinetta/events.h

SOURCES += \
    src/libsonata/spcore.cpp \
    src/libsonata/spfactory.cpp \
    src/libsonata/output/spdefaultoutput.cpp \
    src/libsonata/output/spaudiooutput.cpp \
    src/libsonata/output/circularbuffer.cpp \
    src/libsonata/spobjects/spimage.cpp \
    src/libsonata/spobjects/sptrack.cpp \
    src/libsonata/spobjects/splink.cpp \
    src/libsonata/spobjects/spartist.cpp \
    src/libsonata/spobjects/spplaylist.cpp \
    src/libsonata/spobjects/spplaylistcontainer.cpp \
    src/libsonata/spobjects/spalbum.cpp \
    src/libsonata/spobjects/spartistbrowse.cpp \
    src/libsonata/watchers/spimagewatcher.cpp \
    src/libsonata/watchers/spartistbrowsewatcher.cpp \
    src/libsonata/watchers/spplaylistcontainerwatcher.cpp \
    src/libsonata/watchers/sptrackwatcher.cpp \
    src/libsonata/watchers/spartistwatcher.cpp \
    src/libsonata/watchers/spplaylistwatcher.cpp \
    src/navigation/navigation.cpp \
    src/navigation/navigationnativeeventfilter.cpp \
    src/navigation/lircremote.cpp \
    src/sonataplayer.cpp \
    src/sonataapplication.cpp \
    src/sonataquickview.cpp \
    src/uistatemanagement.cpp \
    src/main.cpp \
    src/libsonata/spotifysession.cpp \
    src/quicktrackinfo.cpp \
    src/playlistmodel.cpp \
    src/playlistcontainermodel.cpp \
    appkey.c \
    src/quickartistsynopsis.cpp \
    src/quicklinker.cpp \
    src/spotifyimageprovider.cpp \
    src/libsonata/watchers/spalbumwatcher.cpp \
    src/models/abstracttrackcollectionmodel.cpp \
    src/models/abstractalbumcollectionmodel.cpp \
    src/models/albumlistmodel.cpp \
    src/libsonata/spobjects/spalbumbrowse.cpp \
    src/libsonata/watchers/spalbumbrowsewatcher.cpp \
    src/models/albumbrowsemodel.cpp \
    src/libsonata/spobjects/spsearch.cpp \
    src/libsonata/watchers/spsearchwatcher.cpp \
    src/quicksearch.cpp \
    src/models/tracklistmodel.cpp \
    src/models/abstractartistcollectionmodel.cpp \
    src/models/artistlistmodel.cpp \
    src/spotinetta/objects/track.cpp \
    src/spotinetta/objects/album.cpp \
    src/spotinetta/objects/artist.cpp \
    src/spotinetta/objects/search.cpp \
    src/spotinetta/objects/link.cpp \
    src/spotinetta/objects/image.cpp \
    src/spotinetta/session.cpp

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
    qml/rework/common/ArtistCollectionView.qml
