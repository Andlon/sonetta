#pragma once

#include "../models/playlistcontainermodel.h"
#include "../models/playlistmodel.h"
#include "../models/tracklistmodel.h"
#include "../models/albummodel.h"
#include "../utilities/session.h"
namespace Sonetta {

class QuickTrackListModel : public TrackListModel {
    Q_OBJECT
public:
    explicit QuickTrackListModel(QObject * parent = 0)
        :   TrackListModel(getCurrentSession(), parent) { }
};

class QuickPlaylistContainerModel : public PlaylistContainerModel {
    Q_OBJECT
public:
    explicit QuickPlaylistContainerModel(QObject * parent = 0)
        :   PlaylistContainerModel(getCurrentSession(), parent) { }
};

class QuickPlaylistModel : public PlaylistModel {
    Q_OBJECT
public:
    explicit QuickPlaylistModel(QObject * parent = 0)
        :   PlaylistModel(getCurrentSession(), parent) { }
};

class QuickAlbumModel : public AlbumModel {
    Q_OBJECT
public:
    explicit QuickAlbumModel(QObject * parent = 0)
        :   AlbumModel(getCurrentSession(), parent) { }
};

}
