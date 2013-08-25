#pragma once

#include "abstracttrackcollectionmodel.h"

namespace Sonetta {

class PlaylistModel : public AbstractTrackCollectionModel
{
    Q_OBJECT
public:
    explicit PlaylistModel(const Spotinetta::Session * session, QObject *parent = 0);

};

}
