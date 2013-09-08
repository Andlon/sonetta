#pragma once

#include "../models/playlistcontainermodel.h"
#include "../models/playlistmodel.h"
#include "../utilities/session.h"
namespace Sonetta {

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

}
