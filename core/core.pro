QT += quick multimedia network

CONFIG += c++11

win32:QMAKE_POST_LINK += $$PWD/../deploy.bat
unix:QMAKE_POST_LINK += bash $$PWD/../deploy.sh

# Add rpath so that deployed libs can be found in libs/ subdirectory on Linux
unix:!mac {
    LIBS += -Wl,-rpath=\\\$$ORIGIN/libs
}

TARGET = sonetta

RCC_DIR = "build/rcc"
UI_DIR = "build/uic"
MOC_DIR = "build/moc"
OBJECTS_DIR = "build/obj"

CONFIG(debug, debug|release) {
    DESTDIR = "$$PWD/../debug"
}
CONFIG(release, debug|release) {
    DESTDIR = "$$PWD/../release"
}

INCLUDEPATH -= "$$PWD"
QML_IMPORT_PATH += "$$PWD/assets/modules"

include("../dependencies/spotinetta/link.pri")

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
    src/utilities/pointers.h \
    src/navigation/windowsnavigationfilter.h \
    src/navigation/navigationcontroller.h \
    src/quick/quickartistsynopsis.h \
    src/quick/quickglobalstatemachine.h \
    include/track.h \
    include/artist.h \
    include/album.h \
    include/metadataprovider.h \
    include/userdataprovider.h \
    include/user.h \
    include/playlist.h \
    include/playlistcontainer.h

SOURCES += \
    $$PWD/../appkey.c \
    src/navigation/navigation.cpp \
    src/main.cpp \
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
    src/navigation/lircclient.cpp \
    src/navigation/navigationcontroller.cpp \
    src/quick/quickartistsynopsis.cpp \
    src/quick/quickglobalstatemachine.cpp

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
    assets/interfaces/default/explore/tophits.qml \
    assets/interfaces/default/explore/ArtistBrowse.qml \
    assets/interfaces/default/common/SingleArtistTrackDelegate.qml \
    assets/interfaces/default2/main.qml \
    assets/interfaces/default2/common/visual/Pattern.qml \
    assets/interfaces/default2/images/artist_placeholder.png \
    assets/interfaces/default2/images/dark.png \
    assets/interfaces/default2/images/light.png \
    assets/interfaces/default2/images/medium.png \
    assets/interfaces/default2/images/sonetta.png \
    assets/interfaces/default2/images/sonetta_large.png \
    assets/interfaces/default2/images/spotify.png \
    assets/interfaces/default2/images/contextArrow.svg \
    assets/interfaces/default2/images/keys/backspace.png \
    assets/interfaces/default2/images/keys/next.png \
    assets/interfaces/default2/images/keys/prev.png \
    assets/interfaces/default2/images/keys/return.png \
    assets/interfaces/default2/images/keys/shiftdown.png \
    assets/interfaces/default2/images/keys/shiftup.png \
    assets/interfaces/default2/images/keys/space.png \
    assets/interfaces/default2/images/keys/symbols.png \
    assets/interfaces/default2/images/playback/next.svg \
    assets/interfaces/default2/images/playback/pause.svg \
    assets/interfaces/default2/images/playback/play.svg \
    assets/interfaces/default2/common/UI.qml \
    assets/interfaces/default2/common/Pattern.qml \
    assets/interfaces/default2/common/qmldir \
    assets/interfaces/default2/common/MenuItem.qml \
    assets/interfaces/default2/common/MenuTextItem.qml \
    assets/interfaces/default2/login/Credentials.qml \
    assets/interfaces/default2/common/VirtualKeyboardDialog.qml \
    assets/interfaces/default2/common/VirtualDialog.qml \
    assets/interfaces/default2/common/VirtualKeyboard.qml \
    assets/interfaces/default2/common/VirtualKeyboard.js \
    assets/interfaces/default2/common/Menu.qml \
    assets/interfaces/default2/common/MenuInputItem.qml \
    assets/interfaces/default2/common/Dialog.js \
    assets/interfaces/default2/common/MenuCheckableItem.qml \
    assets/interfaces/default2/login/Login.qml \
    assets/interfaces/default2/session/Session.qml \
    assets/interfaces/default2/session/MainMenuItem.qml \
    assets/interfaces/default2/session/MainMenu.qml \
    assets/interfaces/default2/common/PageView.qml \
    assets/interfaces/default2/session/PlaybackPage.qml \
    assets/interfaces/default2/session/PlaylistsPage.qml \
    assets/interfaces/default2/session/NowPlaying.qml \
    assets/interfaces/default2/common/Time.js \
    assets/interfaces/default2/common/SpotifyImage.qml \
    assets/interfaces/default2/session/ExplorePage.qml \
    assets/interfaces/default2/session/SettingsPage.qml \
    assets/interfaces/default2/session/SessionStateController.qml \
    assets/interfaces/default2/session/SessionStateController.js \
    assets/interfaces/default2/common/CollectionView.qml \
    assets/interfaces/default2/common/VerticalScrollbar.qml \
    assets/interfaces/default2/session/playlists/PlaylistContainer.qml \
    assets/interfaces/default2/session/playlists/Mosaic.qml \
    assets/interfaces/default2/common/FocusText.qml \
    assets/interfaces/default2/common/CollectionHighlight.qml \
    assets/interfaces/default2/session/playlists/PlaylistInformation.qml \
    assets/interfaces/default2/common/Box.qml \
    assets/interfaces/default2/common/keyboard/VirtualKeyboard.js \
    assets/interfaces/default2/common/keyboard/VirtualKeyboard.qml \
    assets/interfaces/default2/common/keyboard/VirtualKeyboardDialog.qml \
    assets/interfaces/default2/common/menu/Menu.qml \
    assets/interfaces/default2/common/menu/MenuCheckableItem.qml \
    assets/interfaces/default2/common/menu/MenuInputItem.qml \
    assets/interfaces/default2/common/menu/MenuItem.qml \
    assets/interfaces/default2/common/menu/MenuTextItem.qml \
    assets/interfaces/default2/common/views/CollectionHighlight.qml \
    assets/interfaces/default2/common/views/CollectionView.qml \
    assets/interfaces/default2/common/views/PageView.qml \
    assets/interfaces/default2/common/views/VerticalScrollbar.qml \
    assets/interfaces/default2/common/delegates/SingleRowTrackDelegate.qml \
    assets/interfaces/default2/session/playback/CurrentTrackInformation.qml \
    assets/interfaces/default2/common/SimpleHighlight.qml \
    assets/interfaces/default2/session/playback/PlaybackControls.qml \
    assets/interfaces/default2/session/playback/ColoredIcon.qml \
    assets/interfaces/default2/session/playback/Trackbar.qml \
    assets/interfaces/default2/session/playback/QueueView.qml \
    assets/interfaces/default2/common/Section.qml \
    assets/interfaces/default2/common/menu/MenuChildTextItem.qml \
    assets/interfaces/default2/common/views/TrackView.qml \
    assets/interfaces/default2/common/delegates/SimpleTextDelegate.qml \
    assets/interfaces/default2/common/delegates/ArtistDelegate.qml \
    assets/interfaces/default2/common/delegates/ComponentAndTextDelegate.qml \
    assets/interfaces/default2/common/views/ArtistView.qml \
    assets/interfaces/default2/session/explore/TrackResults.qml \
    assets/interfaces/default2/session/explore/ArtistResults.qml \
    assets/interfaces/default2/common/delegates/AlbumDelegate.qml \
    assets/interfaces/default2/common/views/AlbumView.qml \
    assets/interfaces/default2/session/explore/AlbumResults.qml \
    assets/interfaces/default2/session/explore/SearchHistory.qml \
    assets/interfaces/default2/common/dialogs/ContextMenuDialog.qml \
    assets/interfaces/default2/session/BrowsePage.qml \
    assets/interfaces/default2/session/browse/artistbrowse.qml \
    assets/interfaces/default2/session/browse/albumbrowse.qml \
    assets/interfaces/default2/common/FramelessSection.qml \
    assets/interfaces/default2/common/delegates/NumberedDoubleRowTrackDelegate.qml
