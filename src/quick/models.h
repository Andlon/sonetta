#pragma once

#include "../models/playlistcontainermodel.h"
#include "../utilities/session.h"
namespace Sonetta {

class QuickPlaylistContainerModel : public PlaylistContainerModel {
    Q_OBJECT
public:
    explicit QuickPlaylistContainerModel(QObject * parent = 0)
        :   PlaylistContainerModel(getCurrentSession(), parent) { }
};

}
